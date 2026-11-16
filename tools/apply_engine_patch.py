#!/usr/bin/env python3
# Exports the Renderer declarations the Nanite id pass links against and lets one Lumen scene keep cards around every view. Usage: tools/apply_engine_patch.py <engine root> [--check | --revert]
import json, os, sys

SUPPORTED = (5, 7)
MACRO = "RENDERER_API "
MARKER = os.path.join("Engine", "Build", "FlightForgeEnginePatch.json")
RENDERER = os.path.join("Engine", "Source", "Runtime", "Renderer", "Private")

EXPORTS = {
    "Nanite/NaniteCullRaster.h": [
        "FRasterContext InitRasterContext(",
        "\tstatic TUniquePtr< IRenderer > Create(",
        "\tvoid SetViewFlags(const FViewInfo& View);",
    ],
    "Nanite/NaniteShared.h": [
        "FPackedView CreatePackedViewFromViewInfo(",
        "\tstatic FPackedViewArray* Create(FRDGBuilder& GraphBuilder, uint32 NumViews, ArrayType&& Views);",
    ],
    "SceneRendering.h": [
        "\tFViewShaderParameters GetShaderParameters() const;",
    ],
    "ScenePrivate.h": [
        "\tvirtual FPrimitiveSceneInfo* GetPrimitiveSceneInfo(FPrimitiveComponentId PrimitiveId) const final;",
    ],
}


# whole-line replacements: (plain, patched)
REPLACEMENTS = {
    # the shared Lumen scene's card culling takes every origin the plugin hands a family, not just LUMEN_MAX_VIEWS (2) including its own views
    "Lumen/LumenSceneRendering.cpp": [
        ("\t\tif (bAddOrigin && OutOrigins.Num() < LUMEN_MAX_VIEWS)", "\t\tif (bAddOrigin)"),
    ],
}


def exported(line):
    body = line.lstrip("\t")
    return line[: len(line) - len(body)] + MACRO + body


def fail(message):
    sys.exit(f"apply_engine_patch: {message}")


args = [a for a in sys.argv[1:] if not a.startswith("--")]
flags = {a for a in sys.argv[1:] if a.startswith("--")}
if len(args) != 1 or flags - {"--check", "--revert"} or len(flags) > 1:
    fail("usage: apply_engine_patch.py <engine root> [--check | --revert]")

root = args[0]
try:
    version = json.load(open(os.path.join(root, "Engine", "Build", "Build.version")))
except OSError:
    fail(f"{root} is not an engine root")
if os.path.exists(os.path.join(root, "Engine", "Build", "InstalledBuild.txt")):
    fail("an installed engine cannot be rebuilt, use a source checkout")
if (version["MajorVersion"], version["MinorVersion"]) != SUPPORTED:
    fail(f"written for {SUPPORTED[0]}.{SUPPORTED[1]}, this engine is {version['MajorVersion']}.{version['MinorVersion']}")

revert = "--revert" in flags
edits = {}
for header, lines in EXPORTS.items():
    edits.setdefault(header, []).extend((line, exported(line)) for line in lines)
for source_file, pairs in REPLACEMENTS.items():
    edits.setdefault(source_file, []).extend(pairs)
total = sum(len(pairs) for pairs in edits.values())

plan, missing = {}, 0
for name, pairs in edits.items():
    path = os.path.join(root, RENDERER, name)
    have = open(path, newline="").read().split("\n")
    for plain_line, patched_line in pairs:
        plain, done = have.count(plain_line), have.count(patched_line)
        if plain + done != 1:
            fail(f"{name}: expected one line '{plain_line.strip()}', found {plain + done}")
        source, target = (patched_line, plain_line) if revert else (plain_line, patched_line)
        if have.count(source):
            missing += 1
            have[have.index(source)] = target
    plan[path] = "\n".join(have)

if "--check" in flags:
    print(f"{total - missing}/{total} edits applied")
    sys.exit(1 if missing else 0)

for path, text in plan.items():
    if text != open(path, newline="").read():
        open(path, "w", newline="").write(text)

marker = os.path.join(root, MARKER)
if revert:
    if os.path.exists(marker):
        os.remove(marker)
else:
    json.dump({"exports": sum(len(lines) for lines in EXPORTS.values()), "replacements": sum(len(p) for p in REPLACEMENTS.values())}, open(marker, "w"))
print(f"{'reverted' if revert else 'applied'} {missing} edit(s), {'removed' if revert else 'wrote'} {MARKER}")
