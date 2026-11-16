#!/usr/bin/env python3
"""Switches the simulator to every world list_worlds names, or to the given names, spawning a robot after each.

Usage:
  PYTHONPATH=<connector>/python tools/sweep_world_names.py [--port 18600] [name ...]

Each switch_world answers once the new world ticks, so a world passes when that answer names the package list_worlds
gave for it, list_worlds then reports it loaded, and a robot spawned right after reads its state.
Exits non-zero if any world fails.
"""

import argparse
import sys
import time

from flightforge import FlightForgeError, Simulator


def visit(sim, name, package):
    """None if the world loads and takes a robot, else why not."""
    started = time.time()
    try:
        result = sim.switch_world(name)
    except FlightForgeError as error:
        return f"switch_world failed: {error}"
    print(f"{name}: {result.package} in {time.time() - started:.0f} s", flush=True)
    if package and result.package != package:
        return f"loaded {result.package}, list_worlds said {package}"
    if sim.list_worlds().current != result.package:
        return "list_worlds does not report it loaded"
    robot = sim.spawn_robot((0.0, 0.0, 2.0))
    try:
        robot.get_state()
    finally:
        sim.remove_robot(robot)
    return None


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("names", nargs="*", help="names to switch to; default every world list_worlds names")
    parser.add_argument("--port", type=int, default=18600)
    args = parser.parse_args()

    sim = Simulator(port=args.port)
    worlds = {world.name: world.package for world in sim.list_worlds().worlds}
    names = args.names or sorted(worlds)

    failed = []
    for name in names:
        problem = visit(sim, name, worlds.get(name))
        if problem:
            print(f"{name}: {problem}", flush=True)
            failed.append(name)

    print("FAILED: " + ", ".join(failed) if failed else f"all {len(names)} worlds load")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
