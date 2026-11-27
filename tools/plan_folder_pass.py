#!/usr/bin/env python3
# Proposes the /Game/Assets folder pass from the latest audit CSVs; moves nothing. Usage: tools/plan_folder_pass.py <project dir>
import csv, glob, os, re, sys

project = sys.argv[1]
content = os.path.join(project, "Content")
saved = os.path.join(project, "Saved", "FlightForge")

classes = {}
for path in sorted(glob.glob(os.path.join(saved, "classes_*.csv")), key=os.path.getmtime):
    raw = open(path, "rb").read()
    text = raw.decode("utf-16") if raw[:2] in (b"\xff\xfe", b"\xfe\xff") else raw.decode("utf-8-sig")
    for row in csv.DictReader(text.splitlines()):
        asset = row["asset"].split(".")[0]
        if row["class_id"] != "0" or asset not in classes:
            classes[asset] = (row["class_path"], row["source"], row.get("suggested_rule", ""), os.path.basename(path)[8:-4])

PACK_HINTS = [
    (r"/VisualFramework/", "Quixel demo-room pack"),
    (r"/Assets/MS/|/MS_", "Megascans"),
    (r"/TransmissionTowers/", "TransmissionTowers pack"),
    (r"/SM_DSF_", "MW Dead Spruce Forest pack"),
    (r"/SM_Ind_|/Ind_", "Industrial pack (Ind_ prefix)"),
    (r"/SM_BLDG", "building pack (BLDG prefix)"),
]

rows = []
for root, _, files in os.walk(os.path.join(content, "Worlds")):
    if "/Meshes" not in root.replace(os.sep, "/"):
        continue
    for name in files:
        if not name.endswith(".uasset"):
            continue
        rel = os.path.join(root, name)[len(content):].replace(os.sep, "/")[:-7]
        asset = "/Game" + rel
        cls, source, suggestion, world = classes.get(asset, ("", "", "", ""))
        if re.match(r"^/Game/Worlds/[^/]+/Meshes/", asset):
            origin, why = "project-made?", "directly in the world's Meshes folder"
        else:
            origin, why = "pack-derived?", "inside a pack folder: " + asset.split("/")[4]
        for pattern, label in PACK_HINTS:
            if re.search(pattern, asset, re.I):
                origin, why = "pack-derived?", label
                break
        if re.match(r"^(M|MI|T)_|Material|Steel_-_|_mat$|^vinyl$", os.path.basename(asset), re.I):
            origin, why = "not a mesh?", "material/texture name"
        if not cls and suggestion:
            m = re.search(r"class: ([\w/]+)", suggestion)
            cls, source = (m.group(1), "suggested") if m else ("", "")
        target = ""
        if origin == "project-made?" and cls and not cls.startswith("class_"):
            target = "/Game/Assets/" + "/".join(p.capitalize() for p in cls.split("/")) + "/" + os.path.basename(asset)
        rows.append([asset, world or "(not placed in an audited world)", cls, source, origin, why, target])

rows.sort(key=lambda r: (r[4], r[0]))
out = os.path.join(saved, "folder_pass_plan.csv")
with open(out, "w", newline="") as f:
    w = csv.writer(f)
    w.writerow(["asset", "seen_in", "class_path", "class_source", "origin_guess", "why", "proposed_target"])
    w.writerows(rows)

def count(pred):
    return sum(1 for r in rows if pred(r))
print(out)
print(f"{len(rows)} meshes: {count(lambda r: r[4]=='project-made?')} project-made?, {count(lambda r: r[4]=='pack-derived?')} pack-derived?")
print(f"project-made with a target: {count(lambda r: r[6])}, without a class yet: {count(lambda r: r[4]=='project-made?' and not r[2])}, never placed in an audited world: {count(lambda r: r[1].startswith('('))}")
