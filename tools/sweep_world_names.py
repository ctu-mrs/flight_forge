#!/usr/bin/env python3
"""Checks that every friendly world name the clients know opens the level it names.

Usage:
  PYTHONPATH=<connector>/python tools/sweep_world_names.py <project dir> [--port 18600 --log sim.log] [name ...]

Offline, always: the C++ table (data_types.h) and the Python table (WORLD_LEVELS) must agree, and each level must be
exactly one .umap of the project, compared case-insensitively as the engine resolves a short name. With --log, each
level is then opened through switch_world on the simulator at --port, and passes once the simulator's log says that very
map finished loading and the world ticks again. Give it Saved/Logs/<project>.log: the -stdout copy is block-buffered.
Exits non-zero if any name fails.
"""

import argparse
import re
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from flightforge import WORLD_LEVELS  # noqa: E402
from sweep_sky import alive, connect, wait_ticking  # noqa: E402

CPP_TABLE = Path(__file__).resolve().parents[1] / "mrs_flight_forge_connector" / "include" / "flight_forge_connector" / "data_types.h"
LOADED = re.compile(r"LoadMap Load map complete (\S+)")


def cpp_table():
    text = CPP_TABLE.read_text()
    body = text[text.index("Name2Level()") :]
    body = body[: body.index("};")]
    return dict(re.findall(r'\{"(\w+)", "([^"]+)"\}', body))


def package_of(project, level):
    """The one map a level resolves to, or why it does not."""
    content = project / "Content"
    if level.startswith("/Game/"):
        path = content / (level[len("/Game/") :] + ".umap")
        return (level, None) if path.exists() else (None, "no such map")
    found = sorted(path for path in content.rglob("*.umap") if path.stem.lower() == level.lower() and "__ExternalActors__" not in path.parts)
    if not found:
        return None, "no such map"
    if len(found) > 1:
        return None, "ambiguous: " + ", ".join("/Game/" + str(path.relative_to(content).with_suffix("")) for path in found)
    return "/Game/" + str(found[0].relative_to(content).with_suffix("")), None


def switch_and_watch(port, level, log, budget):
    """The package the simulator finished loading after switch_world(level), or None."""
    offset = log.stat().st_size
    sim = connect(port)
    if sim is None:
        return None
    try:
        sim.switch_world(level)
    finally:
        sim.close()

    deadline = time.time() + budget
    while time.time() < deadline:
        time.sleep(3.0)
        with log.open("rb") as handle:
            handle.seek(offset)
            loaded = [package for package in LOADED.findall(handle.read().decode("utf-8", "replace")) if package != "/Engine/Maps/Entry"]
        if loaded and wait_ticking(port, budget):
            return loaded[-1]
    return None


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("project", type=Path)
    parser.add_argument("names", nargs="*", help="friendly names; default the whole table")
    parser.add_argument("--port", type=int, default=18600)
    parser.add_argument("--log", type=Path, help="the simulator's Saved/Logs log; without it only the offline checks run")
    parser.add_argument("--load-timeout", type=float, default=900.0, help="s per world")
    args = parser.parse_args()

    failed = []
    cpp = cpp_table()
    if cpp != WORLD_LEVELS:
        differ = sorted(name for name in set(cpp) | set(WORLD_LEVELS) if cpp.get(name) != WORLD_LEVELS.get(name))
        print(f"C++ and Python tables differ on: {', '.join(differ)}")
        failed += differ

    names = args.names or sorted(WORLD_LEVELS)
    expected = {}
    for name in names:
        level = WORLD_LEVELS.get(name)
        package, problem = package_of(args.project, level) if level else (None, "not in the table")
        print(f"{name} -> {level}: {package or problem}", flush=True)
        if problem:
            failed.append(name)
        else:
            expected[name] = package

    if args.log:
        tried = {}
        for name, package in expected.items():
            level = WORLD_LEVELS[name]
            if level not in tried:
                if not alive(args.port):
                    print(f"{name}: the simulator is not answering")
                    failed.append(name)
                    continue
                started = time.time()
                tried[level] = switch_and_watch(args.port, level, args.log, args.load_timeout)
                print(f"switch_world({level}) loaded {tried[level]} in {time.time() - started:.0f} s", flush=True)
            if tried[level] is None or tried[level].lower() != package.lower():
                print(f"{name}: expected {package}, the simulator loaded {tried[level]}")
                failed.append(name)

    print("FAILED: " + ", ".join(sorted(set(failed))) if failed else "every name resolves")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
