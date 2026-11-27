#!/usr/bin/env python3
"""Lumen memory and camera warm-up: colour cameras in a world at one level, per-camera Lumen scenes or the shared one.

  tools/lumen_share.py -e stock|src -w DeadSpruceForest -q high -n 3 [--rig rgb|mixed|none] [--size 640x480] [--far 300,600]
                       [--defaults] [--exec "FlightForge.Camera.SharedLumenScene 0"] [-o ~/Data/fflook/shared_lumen/mem]

Adds the cameras (rgb rig: n rgb cameras at spread yaws, each its own family; mixed: rgb + stereo + fisheye on one mount; --far:
one more robot with an rgb camera at each distance along x; none: the viewport alone, no robot), puts the viewport on camera 0's view, sets the level and lets the
viewport settle with the cameras idle. Then reads every camera every lockstep step; warm-up = first step from which camera 0
stays within its settled match to the viewport in 16 x 16 block means (wind and readback lag average out). Dumps RHI resource memory
(rhi.DumpResourceMemory) before and after, grouped by resource name, the process's GPU memory, and r.LumenScene.DumpStats.
"""

import argparse
import json
import math
import os
import re
import subprocess
import sys
import time
from collections import defaultdict

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from bench_cameras import Sim, connect  # noqa: E402
from look_compare import step  # noqa: E402
from flightforge import Pose, SensorType, quat_from_rpy  # noqa: E402

GROUPS = [
    ("radiance_cache", re.compile(r"RadianceCache")),
    ("screen_probes", re.compile(r"ScreenProbe|ScreenSpace|ShortRangeAO|DiffuseIndirect")),
    ("reflections", re.compile(r"Reflection")),
    ("translucency_volume", re.compile(r"TranslucencyVolume|Lumen\.Translucency")),
    ("lumen_scene", re.compile(r"^Lumen\.")),
    ("global_sdf", re.compile(r"GlobalDistanceField")),
    ("mesh_sdf", re.compile(r"^DistanceFields\.")),
    ("tsr_taa", re.compile(r"^TSR|TAA|TemporalAA")),
    ("shadows", re.compile(r"Shadow")),
    ("texture_streaming", re.compile(r"AsyncReallocateTexture2D|^TEX_|^T_")),
    ("nanite", re.compile(r"^Nanite")),
    ("capture_targets", re.compile(r"FlightForge|TextureRenderTarget")),
]
ROW = re.compile(rb"^(?:\[[^\]]*\])*\s*(?:LogRHI:\s*)?([^,\n]+),(Texture[^,]*|Buffer|[A-Za-z]+),([\d.]+),(Yes|No),(Yes|No),(Yes|No),(Yes|No),(Yes|No),(Yes|No),(Yes|No),(.*)$")


def group(name):
    for key, rx in GROUPS:
        if rx.search(name):
            return key
    return "other"


def gpu_used_mib(pid):
    out = subprocess.run(["nvidia-smi", "--query-compute-apps=pid,used_memory", "--format=csv,noheader,nounits"], capture_output=True, text=True).stdout
    total = 0
    for line in out.splitlines():
        p, m = [x.strip() for x in line.split(",")]
        if pid is None or int(p) == pid:
            total += int(m)
    smi = subprocess.run(["nvidia-smi", "--query-gpu=memory.used,clocks.gr,clocks.mem", "--format=csv,noheader,nounits"], capture_output=True, text=True).stdout.strip()
    return total, smi


def dump_memory(sim, log, tag):
    start = os.path.getsize(log)
    sim.exec("rhi.DumpResourceMemory all -csv")
    time.sleep(4)
    with open(log, "rb") as f:
        f.seek(start)
        data = f.read()
    sizes, rows = defaultdict(float), []
    for line in data.splitlines():
        m = ROW.match(line.strip())
        if not m:
            continue
        name, mb, transient = m.group(1).decode(errors="replace").strip().removeprefix("RHIResources: "), float(m.group(3)), m.group(6) == b"Yes"
        if transient:
            continue
        rows.append((name, mb))
        sizes[group(name)] += mb
    return dict(sizes), rows


def add_rig(client, robot, rig, n, far, pose, size):
    raw = {"encoding": {"compression": 0}}
    mount = Pose((0.25, 0.0, 0.0), quat_from_rpy(0.0, 0.0, 0.0))
    cams = []
    if rig == "mixed":
        sid = robot.add_sensor(SensorType.RGB_CAMERA, pose=mount)
        robot.set_rgb_camera_config(sensor_id=sid, **raw, **size)
        cams.append((robot, "rgb", sid))
        sid = robot.add_sensor(SensorType.STEREO_CAMERA, pose=mount)
        robot.set_stereo_camera_config(sensor_id=sid, **raw, **size)
        cams.append((robot, "stereo", sid))
        sid = robot.add_sensor(SensorType.FISHEYE_CAMERA, pose=mount)
        robot.set_fisheye_camera_config(sensor_id=sid, **raw)
        cams.append((robot, "fisheye", sid))
    else:
        for i in range(n):
            yawed = Pose((0.25, 0.0, 0.0), quat_from_rpy(0.0, 0.0, math.radians(360.0 * i / n)))
            sid = robot.add_sensor(SensorType.RGB_CAMERA, pose=yawed)
            robot.set_rgb_camera_config(sensor_id=sid, **raw, **size)
            cams.append((robot, "rgb", sid))
    for i, distance in enumerate(far):
        x, y, z = pose
        # above the canopy: the terrain away from the start is not at the start height
        other = client.spawn_robot((x + distance, y, z + 25.0), "empty", label=f"far{i}")
        sid = other.add_sensor(SensorType.RGB_CAMERA, pose=mount)
        other.set_rgb_camera_config(sensor_id=sid, **raw, **size)
        cams.append((other, "rgb", sid))
    return cams


def read(robot, kind, sid):
    if kind == "rgb":
        return robot.rgb(sid)[0]
    if kind == "stereo":
        return robot.stereo(sid)[0]
    return robot.fisheye(sid)[0]


def mad(a, b):
    return float(np.abs(a.astype(np.float32) - b.astype(np.float32)).mean())


def low_mad(a, b, block=16):
    """MAD of block means: Lumen's bounce light is low frequency, wind and readback lag are not."""
    def means(x):
        h, w = x.shape[0] // block * block, x.shape[1] // block * block
        return x[:h, :w].astype(np.float32).reshape(h // block, block, w // block, block, -1).mean((1, 3))
    return float(np.abs(means(a) - means(b)).mean())


def warm_step(errs, steps):
    """First sampled step from which the camera stays within the settled match (median of the last ten samples) plus a margin."""
    tail = np.array(errs[-10:])
    settled = float(np.median(tail))
    limit = settled + max(1.0, 3.0 * float(tail.std()))
    warm = steps[-1]
    for i in range(len(errs) - 1, -1, -1):
        if errs[i] > limit:
            break
        warm = steps[i]
    return warm, settled, limit


LUMEN_STATS = {k: re.compile(rb"  " + k.encode() + rb": ([\d.]+)") for k in ("Mesh cards", "Cards", "Mesh cards to add", "Hi-res pages to add")}
PAGES = re.compile(rb"Allocated (\d+) physical pages out of (\d+)")


def lumen_stats(sim, log):
    start = os.path.getsize(log)
    sim.exec("r.LumenScene.DumpStats 1")
    time.sleep(1.5)
    with open(log, "rb") as f:
        f.seek(start)
        data = f.read()
    block = data[data.find(b"*** LumenScene Stats ***"):]
    out = {}
    for key, rx in LUMEN_STATS.items():
        m = rx.search(block)
        if m:
            out[key] = float(m.group(1))
    m = PAGES.search(block)
    if m:
        out["pages"] = int(m.group(1))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-e", "--engine", choices=["stock", "src"], default="stock")
    ap.add_argument("-w", "--world", default="DeadSpruceForest")
    ap.add_argument("-q", "--quality", default="high")
    ap.add_argument("-n", "--cameras", type=int, default=1)
    ap.add_argument("--rig", choices=["rgb", "mixed", "none"], default="rgb")
    ap.add_argument("--size", default="512x512", help="rgb and stereo image size")
    ap.add_argument("--defaults", action="store_true", help="keep the plugin's camera batching (no RobotBatch 0)")
    ap.add_argument("--far", type=lambda x: [float(v) for v in x.split(",")], default=[], help="metres: one more robot with an rgb camera at each distance")
    ap.add_argument("--stats-every", type=int, default=100)
    ap.add_argument("--far-steps", type=int, default=60, help="steps the viewport stays on each far camera at the end")
    ap.add_argument("--probe", action="store_true", help="warm-up mode: the measured cameras are added after the settle, beside a probe camera the viewport holds")
    ap.add_argument("--steps", type=int, default=400)
    ap.add_argument("--every", type=int, default=5)
    ap.add_argument("--settle", type=int, default=300)
    ap.add_argument("--pose", default="0,0,2")
    ap.add_argument("--tag", default="")
    ap.add_argument("-o", "--out", default=os.path.expanduser("~/Data/fflook/shared_lumen/mem"))
    ap.add_argument("--port", type=int, default=18600)
    ap.add_argument("--attach", action="store_true")
    ap.add_argument("--exec", dest="exec_cmds", action="append", default=[])
    ap.add_argument("--extra", action="append", default=[])
    args = ap.parse_args()
    args.out = os.path.abspath(os.path.expanduser(args.out))
    args.profile = False
    os.makedirs(args.out, exist_ok=True)
    tag = args.tag or f"{args.engine}_{args.world}_{args.quality}_{args.rig}{args.cameras}" + "".join(f"_far{int(d)}" for d in args.far) + ("_probe" if args.probe else "")
    log = os.path.join(args.out, f"{tag}.log")
    if not args.attach and not args.defaults:
        args.exec_cmds = ["FlightForge.Camera.RobotBatch 0", *args.exec_cmds]
    sim = Sim(args, args.world, log)
    width, height = (int(v) for v in args.size.split("x"))
    size = {"width": width, "height": height}
    rec = {"tag": tag, "engine": args.engine, "world": args.world, "level": args.quality, "rig": args.rig, "cameras": args.cameras,
           "size": args.size, "far": args.far, "exec": args.exec_cmds}
    try:
        client = connect(sim, args)
        time.sleep(10)
        for r in client.robots():
            client.remove_robot(r.id)
        pose = tuple(float(x) for x in args.pose.split(","))
        robot = client.spawn_robot(pose, "empty", label="lumen") if args.rig != "none" else None
        time.sleep(3)
        client.set_daytime(12, 0, animate=False)
        client.set_fixed_timestep(1.0 / 30.0)
        client.set_lockstep(True)
        client.set_graphics_settings(args.quality.upper())
        if args.rig == "none":
            step(client, args.settle)
            cams = []
        elif args.probe:
            # the viewport holds a probe camera's view (same mount as camera 0) while the world settles; the measured cameras come after
            probe = robot.add_sensor(SensorType.RGB_CAMERA, pose=Pose((0.25, 0.0, 0.0), quat_from_rpy(0.0, 0.0, 0.0)))
            robot.set_rgb_camera_config(sensor_id=probe, encoding={"compression": 0}, **size)
            client.viewport_image(robot, probe)
            step(client, args.settle)
            cams = add_rig(client, robot, args.rig, args.cameras, args.far, pose, size)
        else:
            # added in lockstep: a TSR camera added while free running rendered zoomed and smeared until the viewport resized
            cams = add_rig(client, robot, args.rig, args.cameras, args.far, pose, size)
            client.viewport_image(cams[0][0], cams[0][2])
            step(client, args.settle)
        steps, errs, full, stats, far_means, first_last = [], [], [], [], [], []
        pid = sim.proc.pid if sim.proc else None
        rec["gpu_mib_before"], rec["smi_before"] = gpu_used_mib(pid)
        rec["mem_before"], rows_before = dump_memory(sim, log, "before")
        json.dump(rows_before, open(os.path.join(args.out, f"{tag}_resources_before.json"), "w"))
        k = 0
        t0 = time.time()
        while k <= args.steps:
            imgs = [read(r, kind, sid) for r, kind, sid in cams]
            if cams and k % args.every == 0:
                vp = client.viewport_image()[0]
                steps.append(k)
                errs.append(low_mad(imgs[0], vp))
                full.append(mad(imgs[0], vp))
                if args.far:
                    far_means.append([round(float(img.mean()), 2) for img in imgs[-len(args.far):]])
            if k % args.stats_every == 0:
                stats.append((k, lumen_stats(sim, log)))
            if k == 0 or k == args.steps:
                first_last.append(imgs + [client.viewport_image()[0]])
            if k < args.steps:
                client.step(1)
            k += 1
            if k % 100 == 0 and errs:
                print(f"{tag}: step {k} {time.time() - t0:.0f} s MAD {errs[-1]:.2f}", flush=True)
        rec["seconds"] = round(time.time() - t0, 1)
        rec["lumen_stats"] = stats
        rec["far_means"] = far_means
        if errs:
            warm, settled, limit = warm_step(errs, steps)
            rec["warm"] = {"warm_step": warm, "settled": round(settled, 2), "limit": round(limit, 2), "first": round(errs[0], 2),
                           "errs": [round(e, 2) for e in errs], "full": [round(e, 2) for e in full], "steps": steps}
            print(f"{tag}: cam0 vs viewport warm at step {warm} (settled {settled:.2f}, first {errs[0]:.2f}); lumen {stats[-1][1] if stats else ''}", flush=True)
        rec["far_check"] = []
        for i in range(len(args.far)):
            far_robot, _, far_sid = cams[len(cams) - len(args.far) + i]
            # if the shared scene already covers the far camera, its image does not change once the viewport arrives
            alone = far_robot.rgb(far_sid)[0]
            client.viewport_image(far_robot, far_sid)
            series, drift = [], []
            for k in range(args.far_steps):
                img = far_robot.rgb(far_sid)[0]
                if k % 5 == 4:
                    series.append(round(low_mad(img, client.viewport_image()[0]), 2))
                    drift.append(round(low_mad(img, alone), 2))
                client.step(1)
            rec["far_check"].append({"distance": args.far[i], "vs_viewport": series, "vs_before_viewport": drift})
            print(f"{tag}: far camera at {args.far[i]:.0f} m: vs viewport {series}, vs itself before the viewport came {drift}", flush=True)
        rec["gpu_mib_after"], rec["smi_after"] = gpu_used_mib(pid)
        rec["mem_after"], rows = dump_memory(sim, log, "after")
        json.dump(rows, open(os.path.join(args.out, f"{tag}_resources.json"), "w"))
        for key in sorted(set(rec["mem_after"]) | set(rec["mem_before"])):
            print(f"{tag}: {key:20s} {rec['mem_before'].get(key, 0):9.1f} -> {rec['mem_after'].get(key, 0):9.1f} MB", flush=True)
        print(f"{tag}: process GPU {rec['gpu_mib_before']} -> {rec['gpu_mib_after']} MiB ({rec['smi_after']})", flush=True)
        from PIL import Image
        for label, imgs in zip(("first", "last"), first_last):
            same = [img for img in [imgs[-1]] + imgs[:-1] if img.shape == imgs[-1].shape]
            Image.fromarray(np.concatenate(same, 1)).save(os.path.join(args.out, f"{tag}_{label}.png"))
        client.set_lockstep(False)
    except SystemExit as e:
        rec["error"] = str(e)
        print(f"{tag}: {e}", flush=True)
    except Exception as e:
        rec["error"] = repr(e)[:300]
        print(f"{tag}: {e!r}", flush=True)
    finally:
        rec["exit"] = sim.stop()
        print(f"{tag}: simulator exit {rec['exit']}", flush=True)
        path = os.path.join(args.out, "results.json")
        results = json.load(open(path)) if os.path.exists(path) else []
        results.append(rec)
        json.dump(results, open(path, "w"), indent=1)


if __name__ == "__main__":
    main()
