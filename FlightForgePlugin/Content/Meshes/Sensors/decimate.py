#!/usr/bin/env python3
"""Quadric-decimate a glb to a triangle budget: parts are merged per colour so flat CAD colours survive."""
import os
import sys
import numpy as np
import trimesh
import fast_simplification

src, dst, budget = sys.argv[1], sys.argv[2], int(sys.argv[3])
scene = trimesh.load(src, force="scene")
groups = {}
for name, geom in scene.geometry.items():
    if not isinstance(geom, trimesh.Trimesh) or len(geom.faces) == 0:
        continue
    try:
        c = geom.visual.material.main_color if geom.visual.kind == "texture" else geom.visual.face_colors[0]
    except Exception:
        c = (200, 200, 200, 255)
    key = tuple(int(v) for v in np.asarray(c)[:4]) if len(c) >= 4 else tuple(int(v) for v in c) + (255,)
    transformed = geom.copy()
    for node in scene.graph.nodes_geometry:
        if scene.graph[node][1] == name:
            transformed.apply_transform(scene.graph[node][0])
            break
    # screws, pins and internal fittings: a closed body cannot collapse below a few faces each
    if np.linalg.norm(transformed.extents) < float(sys.argv[4]) if len(sys.argv) > 4 else 0.006:
        continue
    groups.setdefault(key, []).append(transformed)

total = sum(len(g.faces) for parts in groups.values() for g in parts)
out = trimesh.Scene()
kept = 0
for colour, parts in groups.items():
    merged = trimesh.util.concatenate(parts)
    target = max(int(budget * len(merged.faces) / total), 500)
    if len(merged.faces) > target:
        v, f = fast_simplification.simplify(merged.vertices.astype(np.float64), merged.faces.astype(np.int64), target_reduction=1.0 - target / len(merged.faces), agg=3)
        merged = trimesh.Trimesh(vertices=v, faces=f, process=False)
        merged.update_faces(merged.nondegenerate_faces())
    merged.fix_normals(multibody=True)
    merged.visual.face_colors = np.tile(np.array(colour, dtype=np.uint8), (len(merged.faces), 1))
    kept += len(merged.faces)
    out.add_geometry(merged, node_name=f"colour_{'_'.join(map(str, colour[:3]))}")
out.export(dst)
print(f"{os.path.basename(src)}: {total} -> {kept} faces, {len(groups)} colours, {os.path.getsize(dst)/1e6:.2f} MB, extents {out.extents.round(4)}")
