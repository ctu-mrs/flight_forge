#!/usr/bin/env python3
"""Colour cameras against the main viewport: each camera view (rgb, both stereo eyes, the five fisheye faces) and the viewport
put on that view's exact pose, fov and image size, compared per scalability level and world.

  tools/look_compare.py -e stock|src [-w Warehouse,Valley,DeadSpruceForest] [-q low,medium,high,epic,cinematic]
                        [--cvars "label=A 0,B 1;label=A 1"] [-o ~/Data/fflook/run]

Lockstep (dt 1/30 s), noon, an empty robot. Per view: MAD per channel in 8-bit levels between camera and viewport, the mean
signed difference (camera minus viewport), a 4x4 grid of MADs, and two noise floors taken one step apart (camera vs camera,
viewport vs viewport). Saves viewport | camera | 4x difference PNGs, results.json and results.md under -o.
--seams also saves each stitched fisheye frame and its step across every face border.
"""

import argparse
import json
import math
import os
import sys
import time

import numpy as np
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from bench_cameras import Sim, connect  # noqa: E402
from flightforge import Pose, SensorType, quat_from_rpy  # noqa: E402

LEVELS = ["low", "medium", "high", "epic", "cinematic"]
VIEWS = [("rgb", 0), ("stereo", 0), ("stereo", 1)] + [("fisheye", f) for f in range(5)]
MOUNT = (0.25, 0.0, 0.0)


# metrics //{


def metrics(a, b):
    a, b = a.astype(np.float32), b.astype(np.float32)
    d = np.abs(a - b)
    h, w = d.shape[:2]
    grid = [[round(float(d[i * h // 4:(i + 1) * h // 4, j * w // 4:(j + 1) * w // 4].mean()), 2) for j in range(4)] for i in range(4)]
    return {"mad": round(float(d.mean()), 2), "signed": round(float((a - b).mean()), 2), "grid": grid,
            "mean_a": round(float(a.mean()), 1), "mean_b": round(float(b.mean()), 1)}


def save_triplet(path, viewport, camera):
    diff = np.clip(np.abs(viewport.astype(np.int16) - camera.astype(np.int16)) * 4, 0, 255).astype(np.uint8)
    gap = np.full((viewport.shape[0], 4, 3), 255, np.uint8)
    Image.fromarray(np.concatenate([viewport, gap, camera, gap, diff], 1)).save(path)


# //}

# cameras //{


def add_cameras(robot, pitch_deg):
    pose = Pose(MOUNT, quat_from_rpy(0.0, math.radians(pitch_deg), 0.0))
    raw = {"encoding": {"compression": 0}}
    sids = {}
    sids["rgb"] = robot.add_sensor(SensorType.RGB_CAMERA, pose=pose)
    robot.set_rgb_camera_config(sensor_id=sids["rgb"], **raw)
    sids["stereo"] = robot.add_sensor(SensorType.STEREO_CAMERA, pose=pose)
    robot.set_stereo_camera_config(sensor_id=sids["stereo"], **raw)
    sids["fisheye"] = robot.add_sensor(SensorType.FISHEYE_CAMERA, pose=pose)
    robot.set_fisheye_camera_config(sensor_id=sids["fisheye"], **raw)
    return sids


def read_view(robot, sim, sids, kind, view):
    if kind == "rgb":
        return robot.rgb(sids["rgb"])[0]
    if kind == "stereo":
        left, right, _ = robot.stereo(sids["stereo"])
        return left if view == 0 else right
    sim.exec(f"FlightForge.Camera.Fisheye.ServeFace {view}")
    try:
        return robot.fisheye(sids["fisheye"])[0]
    finally:
        sim.exec("FlightForge.Camera.Fisheye.ServeFace -1")


def stitched(robot, sids):
    return robot.fisheye(sids["fisheye"])[0]


# //}

# seams //{


def seam_steps(client, robot, sids, out):
    """The fisheye frame's brightness step across face borders: mean |left - right| over a 2 px band, against the same away from borders."""
    K = np.asarray(robot.camera_info(sids["fisheye"])[0]["K"], dtype=np.float64)
    img = stitched(robot, sids).astype(np.float32)
    Image.fromarray(img.astype(np.uint8)).save(out)
    h, w = img.shape[:2]
    lum = img.mean(2)
    ys, xs = np.mgrid[0:h, 0:w]
    u, v = (xs - K[0, 2]) / K[0, 0], (ys - K[1, 2]) / K[1, 1]
    theta = np.hypot(u, v)
    phi = np.arctan2(v, u)
    d = np.stack([np.cos(theta), np.sin(theta) * np.cos(phi), -np.sin(theta) * np.sin(phi)], -1)
    inside = theta < math.radians(90)
    face = np.argmax(np.stack([d[..., 0], d[..., 1], -d[..., 1], d[..., 2], -d[..., 2]], -1), -1)
    border = np.zeros_like(inside)
    border[:, 1:] |= face[:, 1:] != face[:, :-1]
    border[1:, :] |= face[1:, :] != face[:-1, :]
    gx = np.abs(np.diff(lum, axis=1))
    gy = np.abs(np.diff(lum, axis=0))
    g = np.maximum(gx[:-1, :], gy[:, :-1])
    b = border[:-1, :-1] & inside[:-1, :-1]
    rest = ~border[:-1, :-1] & inside[:-1, :-1]
    return {"border_gradient": round(float(g[b].mean()), 2), "elsewhere_gradient": round(float(g[rest].mean()), 2)}


# //}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-e", "--engine", choices=["stock", "src"], default="stock")
    ap.add_argument("-w", "--worlds", default="Warehouse,Valley,DeadSpruceForest")
    ap.add_argument("-q", "--quality", default=",".join(LEVELS))
    ap.add_argument("-o", "--out", default=os.path.expanduser("~/Data/fflook/run"))
    ap.add_argument("--cvars", default="", help="';'-separated settings, each 'label=cmd,cmd'; every view is compared under each")
    ap.add_argument("--views", default="", help="kind:view list, e.g. rgb:0,fisheye:3; default every view")
    ap.add_argument("--settle", type=int, default=150, help="lockstep steps after a level change")
    ap.add_argument("--settle-view", type=int, default=45, help="lockstep steps after the viewport takes a view")
    ap.add_argument("--pitch", type=float, default=0.0, help="camera pitch, deg")
    ap.add_argument("--pose", default="0,0,2")
    ap.add_argument("--seams", action="store_true")
    ap.add_argument("--port", type=int, default=18600)
    ap.add_argument("--attach", action="store_true")
    ap.add_argument("--exec", dest="exec_cmds", action="append", default=[])
    ap.add_argument("--extra", action="append", default=[])
    args = ap.parse_args()
    args.out = os.path.abspath(os.path.expanduser(args.out))
    args.profile = False
    os.makedirs(args.out, exist_ok=True)
    settings = [s.split("=", 1) if "=" in s else ["default", s] for s in args.cvars.split(";")] if args.cvars else [["default", ""]]
    views = [(k, int(v)) for k, v in (x.split(":") for x in args.views.split(","))] if args.views else VIEWS
    results_path = os.path.join(args.out, "results.json")
    results = json.load(open(results_path)) if os.path.exists(results_path) else []

    for world in args.worlds.split(","):
        sim = Sim(args, world, os.path.join(args.out, f"{args.engine}_{world}.log"))
        try:
            client = connect(sim, args)
            time.sleep(10)
            for r in client.robots():
                client.remove_robot(r.id)
            robot = client.spawn_robot(tuple(float(x) for x in args.pose.split(",")), "empty", label="look")
            time.sleep(3)
            try:
                client.set_daytime(12, 0, animate=False)
            except Exception as e:
                print("daytime:", e, flush=True)
            sids = add_cameras(robot, args.pitch)
            client.set_fixed_timestep(1.0 / 30.0)
            client.set_lockstep(True)
            for level in args.quality.split(","):
                client.set_graphics_settings(level.upper())
                client.step(args.settle)
                for label, cmds in settings:
                    for line in filter(None, cmds.split(",")):
                        sim.exec(line.strip())
                    client.step(30)
                    for kind, view in views:
                        client.viewport_image(robot, sids[kind], view)
                        client.step(args.settle_view)
                        cam1 = read_view(robot, sim, sids, kind, view)
                        vp1 = client.viewport_image()[0]
                        client.step(1)
                        cam2 = read_view(robot, sim, sids, kind, view)
                        vp2 = client.viewport_image()[0]
                        if cam1.shape != vp1.shape:
                            print(f"{world} {level} {label} {kind}{view}: camera {cam1.shape} viewport {vp1.shape}", flush=True)
                            continue
                        rec = {"engine": args.engine, "world": world, "level": level, "setting": label, "kind": kind, "view": view,
                               "match": metrics(cam1, vp1), "match2": metrics(cam2, vp2),
                               "camera_noise": metrics(cam1, cam2)["mad"], "viewport_noise": metrics(vp1, vp2)["mad"]}
                        name = f"{args.engine}_{world}_{level}_{label}_{kind}{view}.png"
                        save_triplet(os.path.join(args.out, name), vp1, cam1)
                        rec["png"] = name
                        results.append(rec)
                        print(f"{world:16s} {level:9s} {label:8s} {kind}{view}: MAD {rec['match']['mad']:6.2f} / {rec['match2']['mad']:6.2f} "
                              f"signed {rec['match']['signed']:+6.2f}  noise cam {rec['camera_noise']:5.2f} vp {rec['viewport_noise']:5.2f}", flush=True)
                        json.dump(results, open(results_path, "w"), indent=1)
                    if args.seams:
                        rec = {"engine": args.engine, "world": world, "level": level, "setting": label, "kind": "fisheye_seams"}
                        rec.update(seam_steps(client, robot, sids, os.path.join(args.out, f"{args.engine}_{world}_{level}_{label}_fisheye.png")))
                        results.append(rec)
                        print(f"{world:16s} {level:9s} {label:8s} seams: {rec}", flush=True)
                        json.dump(results, open(results_path, "w"), indent=1)
            client.release_viewport()
            client.set_lockstep(False)
        except SystemExit as e:
            print(f"{world}: {e}", flush=True)
        finally:
            code = sim.stop()
            print(f"{world}: simulator exit {code}", flush=True)
    write_md(results, os.path.join(args.out, "results.md"))


def write_md(results, path):
    lines = ["| engine | world | level | setting | view | MAD | MAD 2 | signed | cam noise | vp noise | worst cell |", "|---|---|---|---|---|---|---|---|---|---|---|"]
    for r in results:
        if "match" not in r:
            continue
        worst = max(max(row) for row in r["match"]["grid"])
        lines.append(f"| {r['engine']} | {r['world']} | {r['level']} | {r['setting']} | {r['kind']}{r['view']} | {r['match']['mad']} | {r['match2']['mad']} | "
                     f"{r['match']['signed']:+} | {r['camera_noise']} | {r['viewport_noise']} | {worst} |")
    open(path, "w").write("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
