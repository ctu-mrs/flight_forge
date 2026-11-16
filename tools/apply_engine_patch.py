#!/usr/bin/env python3
# Exports the Nanite id pass's Renderer declarations, keeps Lumen cards around every view, shares radiance caches across share-origin views. Usage: tools/apply_engine_patch.py <engine root> [--check | --revert]
import json, os, sys

SUPPORTED = (5, 7)
MACRO = "RENDERER_API "
MARKER = os.path.join("Engine", "Build", "FlightForgeEnginePatch.json")
RENDERER = os.path.join("Engine", "Source", "Runtime", "Renderer", "Private")
FEATURES = ["nanite_ids", "lumen_view_origins", "shared_radiance_cache"]

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
    # r.Lumen.RadianceCache.ShareOrigin: the screen probe and translucency volume caches of a share-origin view state are its target's
    "Lumen/LumenScreenProbeGather.cpp": [
        ("\t\t\t\tView.ViewState->Lumen.RadianceCacheState,", "\t\t\t\tView.ViewState->RadianceCacheOwner().RadianceCacheState,"),
        ("\t\t\t\t\t\tView.ViewState->Lumen.TranslucencyVolumeRadianceCacheState,", "\t\t\t\t\t\tView.ViewState->RadianceCacheOwner().TranslucencyVolumeRadianceCacheState,"),
    ],
    "Lumen/LumenIrradianceFieldGather.cpp": [
        ("\t\tView.ViewState->Lumen.RadianceCacheState,", "\t\tView.ViewState->RadianceCacheOwner().RadianceCacheState,"),
        ("\t\t\tView.ViewState->Lumen.TranslucencyVolumeRadianceCacheState,", "\t\t\tView.ViewState->RadianceCacheOwner().TranslucencyVolumeRadianceCacheState,"),
    ],
    "Lumen/LumenTranslucencyVolumeLighting.cpp": [
        ("\t\t\t\t\tView.ViewState->Lumen.TranslucencyVolumeRadianceCacheState,", "\t\t\t\t\tView.ViewState->RadianceCacheOwner().TranslucencyVolumeRadianceCacheState,"),
    ],
}

# multi-line edits: (plain, patched); an insertion keeps its anchor inside the patched text
FRAME_INDEX = "\t\t\t\tPassParameters->FrameNumber = View.ViewState ? View.ViewState->GetFrameIndex() : View.Family->FrameNumber;\n"
SHARED_FRAME_INDEX = "\t\t\t\tPassParameters->FrameNumber = RadianceCacheFrameIndex(View);\n"
BLOCKS = {
    "ScenePrivate.h": [
        ("/**\n * The scene manager's private implementation of persistent view state.\n",
         "extern int32 GLumenRadianceCacheShareOrigin;\n\n"
         "/**\n * The scene manager's private implementation of persistent view state.\n"),
        ("\tuint32 ShareOriginUniqueID;\n",
         "\tuint32 ShareOriginUniqueID;\n\n"
         "\tFSceneViewState* ShareOriginState = nullptr;\n"
         "\tbool bShareOriginTarget = false;\n"
         "\tuint32 SharedRadianceCacheFrameIndex = 0;\n"
         "\tuint64 SharedRadianceCacheFrameCounter = 0;\n"),
        ("\tuint32 GetShareOriginViewKey() const\n\t{\n\t\treturn ShareOriginUniqueID;\n\t}\n",
         "\tuint32 GetShareOriginViewKey() const\n\t{\n\t\treturn ShareOriginUniqueID;\n\t}\n\n"
         "\tbool SharesRadianceCache() const\n\t{\n\t\treturn GLumenRadianceCacheShareOrigin != 0 && (ShareOriginState || bShareOriginTarget);\n\t}\n\n"
         "\tFLumenViewState& RadianceCacheOwner()\n\t{\n\t\treturn GLumenRadianceCacheShareOrigin != 0 && ShareOriginState ? ShareOriginState->Lumen : Lumen;\n\t}\n"),
    ],
    "RendererScene.cpp": [
        ("\t\tShareOriginUniqueID = ShareOriginTarget->UniqueID;\n",
         "\t\tShareOriginUniqueID = ShareOriginTarget->UniqueID;\n"
         "\t\tShareOriginState = ShareOriginTarget;\n"
         "\t\tShareOriginTarget->bShareOriginTarget = true;\n"),
    ],
    "Lumen/LumenRadianceCache.cpp": [
        # a group's views render in different families and view states, so they age probes by one clock that ticks in frames the group renders
        ("int32 GRadianceCacheUpdate = 1;\n",
         "int32 GLumenRadianceCacheShareOrigin = 0;\n"
         "FAutoConsoleVariableRef CVarLumenRadianceCacheShareOrigin(\n"
         "\tTEXT(\"r.Lumen.RadianceCache.ShareOrigin\"),\n"
         "\tGLumenRadianceCacheShareOrigin,\n"
         "\tTEXT(\"1: view states sharing an origin (FSceneViewStateReference::ShareOrigin) share their target's radiance caches\"),\n"
         "\tECVF_RenderThreadSafe\n);\n\n"
         "static uint32 RadianceCacheFrameIndex(const FViewInfo& View)\n{\n"
         "\tif (!View.ViewState)\n\t{\n\t\treturn View.Family->FrameNumber;\n\t}\n"
         "\tif (!View.ViewState->SharesRadianceCache())\n\t{\n\t\treturn View.ViewState->GetFrameIndex();\n\t}\n"
         "\tFSceneViewState& Owner = View.ViewState->ShareOriginState ? *View.ViewState->ShareOriginState : *View.ViewState;\n"
         "\tif (Owner.SharedRadianceCacheFrameCounter != GFrameCounterRenderThread)\n\t{\n"
         "\t\tOwner.SharedRadianceCacheFrameCounter = GFrameCounterRenderThread;\n"
         "\t\t++Owner.SharedRadianceCacheFrameIndex;\n\t}\n"
         "\treturn Owner.SharedRadianceCacheFrameIndex;\n}\n\n"
         "int32 GRadianceCacheUpdate = 1;\n"),
        # a group's views stay within a cell of each other, so the clipmaps hold still instead of following whichever view rendered last
        ("\tconst FVector NewViewOrigin = View.ViewMatrices.GetViewOrigin();\n\n"
         "\tCacheState.Clipmaps.SetNum(NumClipmaps);\n\n"
         "\tdouble Level0CellSize = (ClipmapWorldExtent * 2.0f) / ClipmapResolution;\n",
         "\tFVector NewViewOrigin = View.ViewMatrices.GetViewOrigin();\n"
         "\tdouble Level0CellSize = (ClipmapWorldExtent * 2.0f) / ClipmapResolution;\n"
         "\tif (View.ViewState && View.ViewState->SharesRadianceCache() && !bResetState && CacheState.Clipmaps.Num() > 0\n"
         "\t\t&& FVector::Dist(NewViewOrigin, CacheState.Clipmaps[0].Center) < Level0CellSize)\n\t{\n"
         "\t\tNewViewOrigin = CacheState.Clipmaps[0].Center;\n\t}\n\n"
         "\tCacheState.Clipmaps.SetNum(NumClipmaps);\n"),
        # the last update may have been another view's: reproject its probes through world space
        ("\t\t\tSetupOutputs.LastFrameClipmaps = RadianceCacheState.Clipmaps;\n",
         "\t\t\tSetupOutputs.LastFrameClipmaps = RadianceCacheState.Clipmaps;\n"
         "\t\t\tif (View.ViewState && View.ViewState->SharesRadianceCache())\n\t\t\t{\n"
         "\t\t\t\tfor (FRadianceCacheClipmap& Clipmap : SetupOutputs.LastFrameClipmaps)\n\t\t\t\t{\n"
         "\t\t\t\t\tClipmap.CornerTranslatedWorldSpace = (FVector3f)(Clipmap.CornerWorldSpace + View.PrevViewInfo.ViewMatrices.GetPreViewTranslation());\n"
         "\t\t\t\t}\n\t\t\t}\n"),
        ("\t\t\t\t\tPassParameters->FrameNumber = View.ViewState->GetFrameIndex();\n",
         "\t\t\t\t\tPassParameters->FrameNumber = RadianceCacheFrameIndex(View);\n"),
        ("\t\t\t\tPassParameters->DownsampleDistanceFromCameraSq = GLumenRadianceCacheDownsampleDistanceFromCamera * GLumenRadianceCacheDownsampleDistanceFromCamera;\n" + FRAME_INDEX,
         "\t\t\t\tPassParameters->DownsampleDistanceFromCameraSq = GLumenRadianceCacheDownsampleDistanceFromCamera * GLumenRadianceCacheDownsampleDistanceFromCamera;\n" + SHARED_FRAME_INDEX),
        ("\t\t\t\tPassParameters->FirstClipmapWorldExtentRcp = 1.0f / FMath::Max(RadianceCacheInputs.ClipmapWorldExtent, 1.0f);\n" + FRAME_INDEX,
         "\t\t\t\tPassParameters->FirstClipmapWorldExtentRcp = 1.0f / FMath::Max(RadianceCacheInputs.ClipmapWorldExtent, 1.0f);\n" + SHARED_FRAME_INDEX),
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
    edits.setdefault(header, []).extend((f"\n{line}\n", f"\n{exported(line)}\n") for line in lines)
for source_file, pairs in REPLACEMENTS.items():
    edits.setdefault(source_file, []).extend((f"\n{plain}\n", f"\n{patched}\n") for plain, patched in pairs)
for source_file, pairs in BLOCKS.items():
    edits.setdefault(source_file, []).extend(pairs)
total = sum(len(pairs) for pairs in edits.values())

plan, missing = {}, 0
for name, pairs in edits.items():
    path = os.path.join(root, RENDERER, name)
    text = open(path, newline="").read()
    for plain, patched in pairs:
        done = text.count(patched)
        if done > 1 or (done == 0 and text.count(plain) != 1):
            fail(f"{name}: expected one '{plain.strip().splitlines()[0]}', found {text.count(plain)} plain and {done} patched")
        if (done == 0) != revert:
            missing += 1
            text = text.replace(patched, plain) if revert else text.replace(plain, patched)
    plan[path] = text

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
    json.dump({"features": FEATURES, "exports": sum(len(lines) for lines in EXPORTS.values()), "replacements": sum(len(p) for p in REPLACEMENTS.values()),
               "blocks": sum(len(p) for p in BLOCKS.values())}, open(marker, "w"))
print(f"{'reverted' if revert else 'applied'} {missing} edit(s), {'removed' if revert else 'wrote'} {MARKER}")
