#!/usr/bin/env python3
"""Switches a running simulator to every world of a host project and measures what set_daytime and set_weather do there.

Usage:
  PYTHONPATH=<connector>/python tools/sweep_sky.py <project dir> [--port 18600] [--log sim.log] [--restart CMD] [map ...]

With no maps every .umap under Content/Worlds is run. One line per world goes to stdout and --out: the sky system that
answered (read from the simulator's log when --log is given), the mean brightness of a forward camera at fixed exposure
(EV100 0, or the step from it that keeps noon between 40 and 200) at 12:00 and 00:00, and the mean at each weather type, or the error the simulator gave. --restart is run when
the simulator stops answering, so a world that kills it is reported and the sweep goes on.
"""

import argparse
import re
import subprocess
import sys
import time
from pathlib import Path

from flightforge import Compression, FlightForgeError, SensorType, Simulator
from flightforge.wire.Weather import Weather

WEATHERS = sorted((name for name in vars(Weather) if not name.startswith("_")), key=lambda name: getattr(Weather, name))
SETTLE_S = 3.0


def connect(port, timeout=10.0):
    try:
        return Simulator(port=port, timeout=timeout)
    except Exception:
        return None


def alive(port):
    sim = connect(port)
    if sim is None:
        return False
    try:
        sim.ping()
        return True
    except Exception:
        return False
    finally:
        sim.close()


def wait_alive(port, budget):
    deadline = time.time() + budget
    while time.time() < deadline:
        if alive(port):
            return True
        time.sleep(5.0)
    return False


def ticking(port):
    """A world is loaded and its clock moved over the last two seconds."""
    sim = connect(port)
    if sim is None:
        return False
    try:
        before = sim.time().sim_time
        time.sleep(2.0)
        return sim.time().sim_time > before and sim.fps() > 0.0
    except Exception:
        return False
    finally:
        sim.close()


def wait_ticking(port, budget):
    """Three good polls in a row: a first load can stall the game thread for minutes building textures."""
    deadline = time.time() + budget
    streak = 0
    while time.time() < deadline and streak < 3:
        streak = streak + 1 if ticking(port) else 0
        if streak == 0:
            time.sleep(3.0)
    return streak >= 3


def switch(port, level, budget):
    """None once the level runs, else why not."""
    for attempt in range(2):
        sim = connect(port)
        if sim is None:
            return "the simulator is not answering"
        try:
            before = sim.time().sim_time
            sim.switch_world(level)
            break
        except Exception as error:
            if attempt == 1 or not wait_ticking(port, budget):
                return f"switch_world failed: {error}"
        finally:
            sim.close()

    reset = False
    deadline = time.time() + budget
    while time.time() < deadline:
        time.sleep(3.0)
        sim = connect(port)
        if sim is None:
            continue
        try:
            now = sim.time().sim_time
            reset = reset or now < before
            if reset and now > 3.0 and sim.fps() > 0.0:
                return None if wait_ticking(port, budget) else f"loaded but stopped ticking within {budget:.0f} s"
        except Exception:
            pass
        finally:
            sim.close()
    if not alive(port):
        return "the simulator died loading it"
    return f"did not load within {budget:.0f} s" if not reset else f"loaded but never ticked within {budget:.0f} s"


def mean_after_settling(robot, camera):
    time.sleep(SETTLE_S)
    robot.rgb(camera)
    return float(robot.rgb(camera)[0].mean())


def set_exposure(robot, camera, ev):
    robot.set_rgb_camera_config(sensor_id=camera, exposure={"manual": True, "shutter_time": 1.0, "iso": 1600.0, "ev_compensation": float(ev)})


def calibrate(robot, camera):
    """The EV compensation, in 4 EV steps, that puts the frame's mean between 40 and 200; 0 when none does."""
    for ev in (0, -4, 4, -8, 8, -12, 12, -16):
        set_exposure(robot, camera, ev)
        if 40.0 <= mean_after_settling(robot, camera) <= 200.0:
            return ev
    set_exposure(robot, camera, 0)
    return 0


def measure(port):
    sim = Simulator(port=port, timeout=60.0)
    robot = None
    try:
        robot = sim.spawn_robot((0.0, 0.0, 2.0), "x500", label="sky_sweep")
        camera = robot.add_sensor(SensorType.RGB_CAMERA)
        robot.set_rgb_camera_config(sensor_id=camera, encoding={"compression": Compression.NONE})
        set_exposure(robot, camera, 0)
        result = {"start": mean_after_settling(robot, camera)}
        try:
            sim.set_daytime(12, 0)
        except FlightForgeError:
            pass
        result["ev"] = calibrate(robot, camera)

        for key, hour in (("noon", 12), ("midnight", 0)):
            try:
                sim.set_daytime(hour, 0)
                result[key] = mean_after_settling(robot, camera)
            except FlightForgeError as error:
                result[key] = str(error)
        try:
            sim.set_daytime(12, 0)
        except FlightForgeError:
            pass

        weather = {}
        for name in WEATHERS:
            try:
                sim.set_weather(name)
                weather[name] = mean_after_settling(robot, camera)
            except FlightForgeError as error:
                weather[name] = str(error)
        result["weather"] = weather
        try:
            sim.set_weather("SUNNY")
        except FlightForgeError:
            pass
        return result
    finally:
        if robot is not None:
            try:
                sim.remove_robot(robot)
            except Exception:
                pass
        sim.close()


def sky_systems(log_text):
    systems = []
    for line in re.findall(r"FlightForge sky: \d\d:\d\d through (.*)", log_text):
        line = line.strip()
        if line not in systems:
            systems.append(line)
    return systems


def number_or_error(value):
    return f"{value:.1f}" if isinstance(value, float) else f"error ({value})"


def describe(result, systems):
    noon, midnight = result["noon"], result["midnight"]
    if isinstance(noon, float):
        sky = ", ".join(systems) if systems else "found"
    else:
        sky = "none"
    daytime = f"noon {number_or_error(noon)} midnight {number_or_error(midnight)}"
    if isinstance(noon, float) and isinstance(midnight, float):
        daytime += f" ({'darker' if noon >= 2.0 * midnight + 10.0 else 'NOT clearly darker'} at night)"

    weather = result["weather"]
    errors = {value for value in weather.values() if isinstance(value, str)}
    if len(errors) == 1 and all(isinstance(value, str) for value in weather.values()):
        weather_text = f"error ({errors.pop()})"
    else:
        clear = weather.get("SUNNY")
        parts = []
        for name in WEATHERS:
            value = weather[name]
            if isinstance(value, float) and isinstance(clear, float) and name != "SUNNY":
                parts.append(f"{name} {value:.1f} ({value - clear:+.1f})")
            else:
                parts.append(f"{name} {number_or_error(value)}")
        weather_text = ", ".join(parts)
    return f"sky: {sky} | start {result['start']:.1f} at EV100 0; at EV100 {-result['ev']}: {daytime} | weather: {weather_text}"


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("project", type=Path)
    parser.add_argument("maps", nargs="*", help="package paths; default every .umap under Content/Worlds")
    parser.add_argument("--port", type=int, default=18600)
    parser.add_argument("--log", type=Path, help="the simulator's stdout log, to name the sky system that answered")
    parser.add_argument("--restart", help="shell command that brings the simulator back when it stops answering")
    parser.add_argument("--load-timeout", type=float, default=900.0, help="s per world")
    parser.add_argument("--out", type=Path, help="also append each line here")
    args = parser.parse_args()

    maps = args.maps or sorted(
        "/Game/" + str(path.relative_to(args.project / "Content").with_suffix("")) for path in (args.project / "Content" / "Worlds").rglob("*.umap")
    )

    def emit(line):
        print(line, flush=True)
        if args.out:
            with args.out.open("a") as out:
                out.write(line + "\n")

    for level in maps:
        if not alive(args.port):
            if not args.restart:
                emit(f"{level} | not run: the simulator is not answering and there is no --restart")
                continue
            subprocess.run(args.restart, shell=True, check=False)
            if not wait_alive(args.port, args.load_timeout):
                emit(f"{level} | not run: the simulator did not come back")
                continue

        offset = args.log.stat().st_size if args.log and args.log.exists() else 0
        started = time.time()
        failure = switch(args.port, level, args.load_timeout)
        load_s = time.time() - started
        if failure:
            emit(f"{level} | FAILED TO LOAD: {failure} ({load_s:.0f} s)")
            continue

        try:
            result = measure(args.port)
        except Exception as first:
            # a level that streams or opens another one can pull the robot away mid-measurement
            if not wait_ticking(args.port, args.load_timeout):
                emit(f"{level} | loaded in {load_s:.0f} s, measuring failed ({first}) and the world never settled")
                continue
            try:
                result = measure(args.port)
            except Exception as error:
                emit(f"{level} | loaded in {load_s:.0f} s, measuring failed twice: {first}; {error}")
                continue

        systems = []
        if args.log and args.log.exists():
            with args.log.open("rb") as log:
                log.seek(offset)
                systems = sky_systems(log.read().decode("utf-8", "replace"))
        emit(f"{level} | loaded in {load_s:.0f} s | {describe(result, systems)}")


if __name__ == "__main__":
    sys.exit(main())
