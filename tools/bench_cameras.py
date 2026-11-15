#!/usr/bin/env python3
"""Camera render cost: GPU, render- and game-thread ms per frame, achieved Hz and lockstep RTF, per camera type and rig.

  tools/bench_cameras.py -e stock|src [-w Warehouse,Valley,DeadSpruceForest] [-q low,high] [-o DIR]
                         [--phases rgb,stereo,...] [--cvars "A 1;B 0"] [--attach] [--snapshot]

Launches the host's editor in -game on each world (port 18600, cores off), spawns one x500 and runs each phase on it.
Cameras in the per-capture phases render every tick (capture_rate 0) and are read continuously, so the GPU ms per frame
over the robot alone is the GPU cost of one capture of that rig. Timings come from the plugin's FlightForgeStats log
lines (FlightForge.Bench.StatsInterval); console variables are switched at run time through -FlightForgeBenchCommands.
--snapshot instead saves one raw frame per camera type in lockstep for each --cvars setting, for image comparisons.
--profile adds a per-pass GPU breakdown of each free-running phase from the CSV profiler (r.GPUCsvStatsEnabled).
"""

import argparse
import json
import math
import os
import re
import resource
import shutil
import signal
import socket
import subprocess
import sys
import threading
import time

import numpy as np

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "mrs_flight_forge_connector", "python"))
from flightforge import Pose, SensorType, Simulator, quat_from_rpy  # noqa: E402

HOSTS = {"stock": os.path.expanduser("~/Data/mrs_flight_forge 5.7"), "src": os.path.expanduser("~/Data/mrs_flight_forge 5.7 src")}
WORLDS = {
    "Warehouse": "/Game/Worlds/Warehouse/Maps/Warehouse",
    "Valley": "/Game/Worlds/Valley/Maps/Valley",
    "DeadSpruceForest": "/Game/MWDeadSpruceForest/Maps/dead_spruce_forest",
}
FORBIDDEN_PORTS = {8551, 8600, *range(4000, 4011)}
PER_CAPTURE = ["rgb", "stereo", "fisheye", "depth", "instance", "realsense"]
ALL_PHASES = ["base", *PER_CAPTURE, "six_free", "six_lockstep"]
STATS = re.compile(rb"FlightForgeStats: t=([\d.]+) frames=(\d+) fps=([\d.]+) game=([\d.]+) render=([\d.]+) rhi=([\d.]+) gpu=([\d.]+)")
MOUNT = (0.25, 0.0, 0.0)


# launch //{


class Sim:
    def __init__(self, args, world, log):
        self.args, self.log, self.proc = args, log, None
        self.cmds = os.path.join(args.out, "bench_commands.txt")
        open(self.cmds, "w").close()
        if args.attach:
            return
        if args.port in FORBIDDEN_PORTS:
            sys.exit(f"port {args.port} belongs to the live simulator")
        with socket.socket() as s:
            if s.connect_ex(("127.0.0.1", args.port)) == 0:
                sys.exit(f"port {args.port} is already in use; a leftover editor would take half the requests")
        if shutil.disk_usage("/").free < 15e9:
            sys.exit("less than 15 GB free on /")
        host = HOSTS[args.engine]
        exec_cmds = ",".join(["FlightForge.Bench.StatsInterval 1", *(["r.GPUCsvStatsEnabled 1"] if args.profile else []), *args.exec_cmds])
        cmd = [os.path.join(host, "run-editor.sh"), WORLDS.get(world, world), "-game", "-RenderOffScreen", "-unattended", "-nopause", "-nosplash",
               "-stdout", "-AllowStdOutLogVerbosity", f"-FlightForgePort={args.port}", f"-FlightForgeGameModePort={args.port}",
               f"-FlightForgeDroneStartPort={args.port + 10}", f"-FlightForgeBenchCommands={self.cmds}", f"-ExecCmds={exec_cmds}", *args.extra]
        env = dict(os.environ, UE_EDITOR_BIN="UnrealEditor-Cmd")
        self.proc = subprocess.Popen(cmd, cwd=host, env=env, stdout=open(log, "wb"), stderr=subprocess.STDOUT,
                                     preexec_fn=lambda: resource.setrlimit(resource.RLIMIT_CORE, (0, 0)))

    def exec(self, line):
        with open(self.cmds, "a") as f:
            f.write(line + "\n")
        time.sleep(0.6)

    def alive(self):
        return self.proc is None or self.proc.poll() is None

    def stop(self):
        if self.proc is None or self.proc.poll() is not None:
            return self.proc.returncode if self.proc else None
        # run-editor.sh execs the editor, so the child is the editor itself
        exe = os.path.basename(os.readlink(f"/proc/{self.proc.pid}/exe")) if os.path.exists(f"/proc/{self.proc.pid}/exe") else ""
        if exe != "UnrealEditor-Cmd":
            print(f"pid {self.proc.pid} is {exe}, not the editor; not killed", flush=True)
            return None
        self.proc.send_signal(signal.SIGTERM)
        try:
            self.proc.wait(20)
        except subprocess.TimeoutExpired:
            self.proc.kill()
            self.proc.wait()
        return self.proc.returncode


def connect(sim, args, budget=2400):
    start = time.time()
    while time.time() - start < budget:
        if not sim.alive():
            sys.exit(f"the simulator exited with {sim.proc.returncode}" + (" (GPU watchdog)" if sim.proc.returncode == 3 else ""))
        try:
            client = Simulator(port=args.port, timeout=120.0)
            client.step_timeout = 120.0
            if client.fps() > 0:
                return client
            client.close()
        except Exception:
            pass
        time.sleep(3)
    sys.exit("the simulator did not answer")


# //}

# sensors //{


def read_fn(robot, kind):
    if kind == "rgb":
        return lambda sid: robot.rgb(sid, decode=False).stamp
    if kind == "stereo":
        return lambda sid: robot.stereo(sid, decode=False)[0].stamp
    if kind == "fisheye":
        return lambda sid: robot.fisheye(sid, decode=False).stamp
    if kind == "depth":
        return lambda sid: robot._image("GetDepthCameraDataRequest", "DepthCameraDataResponse", sid).stamp
    if kind == "instance":
        return lambda sid: robot._image("GetInstanceSegDataRequest", "InstanceSegDataResponse", sid).stamp
    raise ValueError(kind)


TYPES = {"rgb": SensorType.RGB_CAMERA, "stereo": SensorType.STEREO_CAMERA, "fisheye": SensorType.FISHEYE_CAMERA,
         "depth": SensorType.DEPTH_CAMERA, "instance": SensorType.INSTANCE_SEG_CAMERA}


NO_TAA = False


def add_camera(robot, kind, rate, yaw_deg=0.0, raw=False):
    pose = Pose(MOUNT, quat_from_rpy(0.0, 0.0, math.radians(yaw_deg)))
    sid = robot.add_sensor(TYPES[kind], pose=pose)
    fields = {"capture_rate": float(rate)}
    if raw:
        fields["encoding"] = {"compression": 0}
    if NO_TAA and kind in ("rgb", "stereo"):
        fields["enable_temporal_aa"] = False
    if kind == "stereo":
        robot.set_stereo_camera_config(sensor_id=sid, **fields)
    elif kind == "fisheye":
        robot.set_fisheye_camera_config(sensor_id=sid, **fields)
    else:
        robot.set_rgb_camera_config(sensor_id=sid, **fields)
    return sid


def rig(kind):
    return [("rgb", 0.0), ("depth", 0.0), ("instance", 0.0)] if kind == "realsense" else [(kind, 0.0)]


# //}

# phases //{


def readers(client, robot_id, cams, hz, until):
    """One thread and connection per camera, reading at hz; returns {sid: [stamps]}."""
    stamps = {sid: [] for _, sid in cams}
    errors = []

    def run(kind, sid):
        robot = client.robot(robot_id, own_connection=True)
        read = read_fn(robot, kind)
        next_t = time.time()
        while time.time() < until:
            try:
                stamps[sid].append((time.time(), read(sid)))
            except Exception as e:
                errors.append(str(e)[:120])
            next_t += 1.0 / hz
            time.sleep(max(0.0, next_t - time.time()))
        robot.close()

    threads = [threading.Thread(target=run, args=c) for c in cams]
    for t in threads:
        t.start()
    return threads, stamps, errors


def window(stamps, t0, t1):
    inside = [s for t, s in stamps if t0 <= t <= t1]
    return len(set(inside)) / (t1 - t0) if inside else 0.0


def profile_window(args, t0, t1):
    """CSV-profiles [t0, t1] of a phase; returns the per-pass GPU ms per frame, averaged."""
    if not args.profile:
        time.sleep(max(0.0, t1 - time.time()))
        return None
    time.sleep(max(0.0, t0 - time.time()))
    folder = os.path.join(HOSTS[args.engine], "Saved", "Profiling", "CSV")
    before = set(os.listdir(folder)) if os.path.isdir(folder) else set()
    args.sim.exec("csvprofile start")
    time.sleep(max(0.0, t1 - time.time()))
    args.sim.exec("csvprofile stop")
    for _ in range(60):
        fresh = sorted(set(os.listdir(folder)) - before) if os.path.isdir(folder) else []
        if fresh and "[HasHeaderRowAtEnd]" in open(os.path.join(folder, fresh[-1]), errors="replace").read():
            return gpu_passes(os.path.join(folder, fresh[-1]))
        time.sleep(0.5)
    print("no CSV profile for this phase", flush=True)
    return None


def gpu_passes(path):
    rows = [line.rstrip("\n").split(",") for line in open(path, errors="replace")]
    header = rows[0]
    def number(x):
        try:
            float(x)
            return True
        except ValueError:
            return False

    data = [r for r in rows[1:] if len(r) == len(header) and not r[0].startswith("[") and number(r[1])]
    out = {}
    for i, name in enumerate(header):
        if name.startswith("GPU/"):
            values = [float(r[i]) for r in data if number(r[i])]
            if values:
                out[name[4:]] = round(sum(values) / len(values), 3)
    if out:
        os.remove(path)
    return out


def run_phase(client, robot, name, args):
    for s in robot.sensors():
        robot.remove_sensor(s.id)
    time.sleep(1.0)
    rec = {"phase": name}
    if name == "base":
        t0 = time.time() + args.warmup
        rec["gpu_passes"] = profile_window(args, t0, t0 + args.duration)
        rec["t0"], rec["t1"] = t0, time.time()
        return rec

    kind, _, rate = name.partition("@")
    if kind in PER_CAPTURE:
        cams = [(k, add_camera(robot, k, float(rate or 0.0), y)) for k, y in rig(kind)]
        hz = float(rate) if rate else 20.0
    else:
        cams = [("rgb", add_camera(robot, "rgb", 30.0, y)) for y in range(0, 360, 60)]
        hz = 30.0

    if name == "six_lockstep":
        client.set_fixed_timestep(1.0 / 30.0)
        client.set_lockstep(True)
        fresh = []
        try:
            def read_all(conns):
                out = []
                ths = [threading.Thread(target=lambda r, s: out.append(read_fn(r, "rgb")(s)), args=(r, s)) for r, (_, s) in zip(conns, cams)]
                for t in ths:
                    t.start()
                for t in ths:
                    t.join()
                return out

            conns = [client.robot(robot.id, own_connection=True) for _ in cams]
            for _ in range(10):
                client.step(1)
                read_all(conns)
            t0 = time.time()
            sim0 = client.sim_time()[0]
            steps = 0
            prev = None
            while time.time() - t0 < args.duration:
                client.step(1)
                got = read_all(conns)
                fresh.append(prev is None or min(got) > prev)
                prev = max(got)
                steps += 1
            rec["t0"], rec["t1"] = t0, time.time()
            rec["rtf"] = (client.sim_time()[0] - sim0) / (rec["t1"] - t0)
            rec["steps"] = steps
            rec["fresh_share"] = sum(fresh) / max(len(fresh), 1)
            for r in conns:
                r.close()
        finally:
            client.set_lockstep(False)
            client.set_fixed_timestep(0.0)
        return rec

    until = time.time() + args.warmup + args.duration
    threads, stamps, errors = readers(client, robot.id, cams, hz, until)
    t0 = time.time() + args.warmup
    rec["gpu_passes"] = profile_window(args, t0, until)
    for t in threads:
        t.join()
    rec["t0"], rec["t1"] = t0, until
    rec["hz"] = [round(window(stamps[sid], t0, until), 2) for _, sid in cams]
    rec["errors"] = errors[:5]
    return rec


# //}

# stats //{


def parse_stats(log):
    data = open(log, "rb").read()
    return [tuple(float(x) for x in m.groups()) for m in STATS.finditer(data)]


def summarize(rec, stats):
    rows = [s for s in stats if rec["t0"] + 0.5 <= s[0] <= rec["t1"] + 0.5]
    if not rows:
        return
    arr = np.array(rows)
    w = arr[:, 1]
    for i, key in ((2, "fps"), (3, "game"), (4, "render"), (5, "rhi"), (6, "gpu")):
        rec[key] = round(float(np.average(arr[:, i], weights=w)), 3)


def report(results, path):
    lines = ["| world | quality | phase | fps | GPU ms/frame | GPU ms over base | render ms | game ms | Hz | RTF |", "|---|---|---|---|---|---|---|---|---|---|"]
    for r in results:
        base = next((b for b in results if b["world"] == r["world"] and b["quality"] == r["quality"] and b["phase"] == "base"), None)
        over = f"{r['gpu'] - base['gpu']:.2f}" if base and "gpu" in r and "gpu" in base and r["phase"] != "base" else ""
        hz = "/".join(f"{h:.1f}" for h in r.get("hz", [])) if r.get("hz") else ""
        rtf = f"{r['rtf']:.3f}" if "rtf" in r else ""
        lines.append(f"| {r['world']} | {r['quality']} | {r['phase']} | {r.get('fps', '')} | {r.get('gpu', '')} | {over} | {r.get('render', '')} | {r.get('game', '')} | {hz} | {rtf} |")
    open(path, "w").write("\n".join(lines) + "\n")
    print("\n".join(lines), flush=True)


def report_passes(results, path, top=12):
    """GPU ms per frame by pass, each phase over its world's and setting's base phase."""
    lines = []
    for r in results:
        if not r.get("gpu_passes") or r["phase"] == "base":
            continue
        base = next((b for b in results if b["world"] == r["world"] and b["quality"] == r["quality"] and b["cvars"] == r["cvars"] and b["phase"] == "base"
                     and b.get("gpu_passes")), {"gpu_passes": {}})
        over = {k: v - base["gpu_passes"].get(k, 0.0) for k, v in r["gpu_passes"].items()}
        ranked = sorted(over.items(), key=lambda kv: -kv[1])[:top]
        lines.append(f"{r['world']} {r['quality']} {r['phase']} [{r['cvars'] or 'default'}]: " + ", ".join(f"{k} {v:.2f}" for k, v in ranked))
    open(path, "w").write("\n".join(lines) + "\n")
    print("\n".join(lines), flush=True)


# //}

# snapshot //{


def snapshot(client, robot, sim, args, tag):
    """One raw frame per camera type in lockstep, for each cvar setting in --cvars (';'-separated settings, ',' within one)."""
    for s in robot.sensors():
        robot.remove_sensor(s.id)
    kinds = ["rgb", "rgb_b", "stereo", "fisheye", "depth", "instance"]
    sids = {k: add_camera(robot, k.split("_")[0], 0.0, 60.0 if k == "rgb_b" else 0.0, raw=True) for k in kinds}
    robot2 = client.robot(robot.id)
    client.set_fixed_timestep(1.0 / 30.0)
    client.set_lockstep(True)
    out = {}
    try:
        for i, setting in enumerate(args.cvars or [""]):
            for line in filter(None, setting.split(",")):
                sim.exec(line.strip())
            for _ in range(args.settle):
                client.step(1)
                for k, sid in sids.items():
                    read_fn(robot2, k.split("_")[0])(sid)
            for k, sid in sids.items():
                if k == "stereo":
                    l, r, st = robot.stereo(sid)
                    img = np.concatenate([l, r], 1)
                elif k == "fisheye":
                    img, st = robot.fisheye(sid)
                elif k.startswith("rgb"):
                    img, st = robot.rgb(sid)
                elif k == "depth":
                    img, st = robot.depth(sid)
                else:
                    ids, cls, st = robot.panoptic(sid)
                    img = np.stack([ids.astype(np.int64), cls.astype(np.int64)], -1)
                np.save(os.path.join(args.out, f"snap_{tag}_{i}_{k}.npy"), np.asarray(img))
                out.setdefault(k, []).append(st)
            print(f"snapshot {tag} setting {i} ({setting or 'default'}) stamps", {k: v[-1] for k, v in out.items()}, flush=True)
    finally:
        client.set_lockstep(False)
        client.set_fixed_timestep(0.0)
    diffs = compare(args, tag, kinds, len(args.cvars or [""]))
    json.dump(diffs, open(os.path.join(args.out, f"diff_{tag}.json"), "w"), indent=1)
    return out


def compare(args, tag, kinds, settings):
    """Mean absolute and max difference per channel of every setting against setting 0; instance ids as the share of differing pixels."""
    diffs = {}
    for k in kinds:
        ref = np.load(os.path.join(args.out, f"snap_{tag}_0_{k}.npy")).astype(np.float64)
        for i in range(1, settings):
            img = np.load(os.path.join(args.out, f"snap_{tag}_{i}_{k}.npy")).astype(np.float64)
            if img.shape != ref.shape:
                diffs[f"{k}/{i}"] = {"shape": [list(ref.shape), list(img.shape)]}
                continue
            d = np.abs(img - ref).reshape(-1, ref.shape[-1] if ref.ndim == 3 else 1)
            rec = {"mad": [round(float(x), 4) for x in d.mean(0)], "max": [round(float(x), 3) for x in d.max(0)]}
            if k == "instance":
                rec["ids_differ"] = round(float((d[:, 0] > 0).mean()), 5)
            diffs[f"{k}/{i}"] = rec
            print(f"diff {tag} {k} setting {i} vs 0: {rec}", flush=True)
    return diffs


# //}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-e", "--engine", choices=HOSTS, default="stock")
    ap.add_argument("-w", "--worlds", default="Warehouse,Valley,DeadSpruceForest")
    ap.add_argument("-q", "--quality", default="low,high")
    ap.add_argument("-o", "--out", default="/tmp/ff_bench")
    ap.add_argument("--phases", default=",".join(ALL_PHASES))
    ap.add_argument("--cvars", default="", help="';'-separated console settings; each runs the phases (or the snapshot) once")
    ap.add_argument("--exec", dest="exec_cmds", action="append", default=[], help="extra -ExecCmds entry at launch")
    ap.add_argument("--extra", action="append", default=[], help="extra launch argument")
    ap.add_argument("--port", type=int, default=18600)
    ap.add_argument("--attach", action="store_true", help="use a simulator already listening on --port")
    ap.add_argument("--repeat", type=int, default=1, help="cycle through the --cvars settings this many times, so outside GPU load hits each alike")
    ap.add_argument("--warmup", type=float, default=4.0)
    ap.add_argument("--duration", type=float, default=8.0)
    ap.add_argument("--settle", type=int, default=30, help="lockstep steps before a snapshot read")
    ap.add_argument("--snapshot", action="store_true")
    ap.add_argument("--profile", action="store_true", help="per-pass GPU ms of each free-running phase (CSV profiler)")
    ap.add_argument("--pose", default="0,0,2", help="robot position, ground frame m")
    ap.add_argument("--robot", default="x500", help="robot model; empty has no spinning propellers in view, for snapshots")
    ap.add_argument("--no-taa", action="store_true", help="rgb and stereo cameras without temporal AA")
    args = ap.parse_args()
    global NO_TAA
    NO_TAA = args.no_taa
    args.cvars = [c for c in args.cvars.split(";")] if args.cvars else []
    os.makedirs(args.out, exist_ok=True)

    smi = subprocess.run(["nvidia-smi", "--query-compute-apps=pid,process_name,used_memory", "--format=csv,noheader"], capture_output=True, text=True).stdout.strip()
    print(f"other GPU compute jobs: {smi or 'none'}", flush=True)

    results = []
    for world in args.worlds.split(","):
        log = os.path.join(args.out, f"{args.engine}_{world}.log")
        sim = Sim(args, world, log)
        args.sim = sim
        try:
            client = connect(sim, args)
            time.sleep(10)
            for r in client.robots():
                client.remove_robot(r.id)
            robot = client.spawn_robot(tuple(float(x) for x in args.pose.split(",")), args.robot, label="bench")
            time.sleep(3)
            try:
                client.set_daytime(12, 0, animate=False)
            except Exception as e:
                print("daytime:", e, flush=True)
            for quality in args.quality.split(","):
                client.set_graphics_settings(quality.upper())
                time.sleep(5)
                if args.snapshot:
                    snapshot(client, robot, sim, args, f"{args.engine}_{world}_{quality}")
                    continue
                for ci, setting in enumerate((args.cvars or [""]) * args.repeat):
                    for line in filter(None, setting.split(",")):
                        sim.exec(line.strip())
                    for phase in args.phases.split(","):
                        rec = run_phase(client, robot, phase, args)
                        rec.update(world=world, quality=quality, cvars=setting, engine=args.engine)
                        results.append(rec)
                        print(json.dumps({k: v for k, v in rec.items() if k not in ("t0", "t1")}), flush=True)
            for s in robot.sensors():
                robot.remove_sensor(s.id)
            time.sleep(2)
        finally:
            code = sim.stop()
            print(f"{world}: simulator exit {code}" + (" (GPU watchdog)" if code == 3 else ""), flush=True)
        stats = parse_stats(log)
        for rec in results:
            if rec["world"] == world:
                summarize(rec, stats)
        json.dump(results, open(os.path.join(args.out, f"bench_{args.engine}.json"), "w"), indent=1)

    if results:
        json.dump(results, open(os.path.join(args.out, f"bench_{args.engine}.json"), "w"), indent=1)
        if args.profile:
            report_passes(results, os.path.join(args.out, f"passes_{args.engine}.md"))
        report(results, os.path.join(args.out, f"bench_{args.engine}.md"))


if __name__ == "__main__":
    main()
