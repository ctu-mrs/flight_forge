#!/usr/bin/env python3
"""Set up an existing Unreal Engine 5.7 project for FlightForge API 1.x.

The default is deliberately non-destructive: plugins are symlinked, existing
configuration is updated in place, and the first version of every changed file
is kept as <filename>.flightforge.bak. Use --dry-run before making changes.
"""

import argparse
import json
import os
import re
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

GAME_MODE = "/FlightForgePlugin/Blueprints/BP_Ueds_Game_Mode.BP_Ueds_Game_Mode_C"
GAME_INSTANCE = "/Script/FlightForgeCore.FlightForgeGameInstance"
PLUGIN_NAMES = ("FlightForgePlugin", "UnrealYAML", "glTFRuntime")


class SetupError(RuntimeError):
    pass


def log(message, dry_run=False):
    print(("[dry-run] " if dry_run else "") + message)


def write_text(path, content, dry_run):
    content = content if content.endswith("\n") else content + "\n"
    if path.exists() and path.read_text(encoding="utf-8") == content:
        log(f"unchanged {path}", dry_run)
        return

    backup = path.with_name(path.name + ".flightforge.bak")
    log(f"write {path}" + (f" (original: {backup})" if path.exists() else ""), dry_run)
    if dry_run:
        return

    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists() and not backup.exists():
        shutil.copy2(path, backup)
    temporary = path.with_name(path.name + ".flightforge-tmp")
    temporary.write_text(content, encoding="utf-8")
    os.replace(temporary, path)


def require_source(repository):
    repository = repository.expanduser().resolve()
    required = (
        "FlightForgePlugin/FlightForgePlugin.uplugin",
        "UnrealYAML/UnrealYAML.uplugin",
        "FlightForgePlugin/Config/FlightForgeClasses.yaml",
        ".gitman.yml",
    )
    missing = [name for name in required if not (repository / name).is_file()]
    if missing:
        raise SetupError(f"{repository} is not a FlightForge 1.x checkout; missing: {', '.join(missing)}")
    if not (repository / "schema" / "flightforge.fbs").is_file():
        raise SetupError(f"{repository} looks like the old cereal-based API, not API 1.x (schema/flightforge.fbs is missing)")
    return repository


def install_dependencies(repository, install, dry_run):
    source = (repository / "glTFRuntime").resolve()
    descriptor = source / "glTFRuntime.uplugin"
    if descriptor.is_file():
        return source
    if not install:
        raise SetupError(
            f"glTFRuntime is not materialized at {source}. Run this script with --install-deps, "
            f"or run 'python3 -m gitman install' in {repository} first."
        )
    log(f"install gitman dependencies in {repository}", dry_run)
    if dry_run:
        return source
    result = subprocess.run([sys.executable, "-m", "gitman", "install"], cwd=repository)
    if result.returncode != 0:
        raise SetupError(f"gitman install failed with exit code {result.returncode}")
    if not descriptor.is_file():
        raise SetupError(f"gitman finished but {descriptor} is still missing")
    return source


def stage_plugin(source, destination, copy, dry_run):
    if destination.is_symlink():
        if destination.resolve() == source.resolve():
            log(f"already linked {destination}", dry_run)
            return
        raise SetupError(f"{destination} is a symlink to {destination.resolve()}, not {source}; move it aside first")
    if destination.exists():
        if copy:
            log(f"already present, not replacing {destination}", dry_run)
            return
        raise SetupError(f"{destination} already exists and is not this plugin's symlink; move it aside or inspect it first")
    if not source.is_dir():
        if dry_run:
            log(f"would materialize plugin dependency at {source}", dry_run)
            return
        raise SetupError(f"plugin directory does not exist: {source}")

    log((f"copy {source} -> {destination}" if copy else f"link {destination} -> {source}"), dry_run)
    if dry_run:
        return
    destination.parent.mkdir(parents=True, exist_ok=True)
    if copy:
        shutil.copytree(source, destination, symlinks=True)
    else:
        destination.symlink_to(source, target_is_directory=True)


def update_uproject(project_path, module_name, create_module, dry_run):
    try:
        data = json.loads(project_path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        raise SetupError(f"cannot read {project_path}: {error}") from error

    if not create_module and not list((project_path.parent / "Source").glob("*Editor.Target.cs")):
        raise SetupError(f"no *Editor.Target.cs under {project_path.parent / 'Source'}. Rerun with --create-module")
    modules = data.setdefault("Modules", [])
    if not modules and not create_module:
        raise SetupError(f"{project_path} has no C++ module. Rerun with --create-module for a Blueprint-only/new project")
    if create_module and not modules:
        modules.append({"Name": module_name, "Type": "Runtime", "LoadingPhase": "Default"})

    plugins = data.setdefault("Plugins", [])
    for name in PLUGIN_NAMES:
        entry = next((item for item in plugins if item.get("Name", "").lower() == name.lower()), None)
        if entry is None:
            plugins.append({"Name": name, "Enabled": True})
        else:
            entry["Enabled"] = True

    write_text(project_path, json.dumps(data, indent=2, ensure_ascii=False) + "\n", dry_run)


def set_ini_values(path, section, values, dry_run):
    text = path.read_text(encoding="utf-8") if path.exists() else ""
    lines = text.splitlines()
    output = list(lines)
    try:
        start = next(i for i, line in enumerate(output) if line.strip().lower() == section.lower())
    except StopIteration:
        if output and output[-1].strip():
            output.append("")
        output.append(section)
        start = len(output) - 1

    end = next((i for i in range(start + 1, len(output)) if output[i].lstrip().startswith("[")), len(output))
    requested = {key.lower(): (key, value) for key, value in values.items()}
    body = [line for line in output[start + 1 : end] if line.split("=", 1)[0].strip().lower() not in requested]
    body.extend(f"{key}={value}" for key, value in values.items())
    output[start + 1 : end] = body
    write_text(path, "\n".join(output) + "\n", dry_run)


def ensure_cook_path(path, plugin_path, dry_run):
    section = "[/Script/UnrealEd.ProjectPackagingSettings]"
    entry = f'+DirectoriesToAlwaysCook=(Path="{plugin_path}")'
    text = path.read_text(encoding="utf-8") if path.exists() else ""
    lines = text.splitlines()
    if entry in lines:
        log(f"unchanged {path}: {plugin_path}", dry_run)
        return
    try:
        start = next(i for i, line in enumerate(lines) if line.strip().lower() == section.lower())
    except StopIteration:
        if lines and lines[-1].strip():
            lines.append("")
        lines.extend([section, entry])
    else:
        end = next((i for i in range(start + 1, len(lines)) if lines[i].lstrip().startswith("[")), len(lines))
        lines[end:end] = [entry]
    write_text(path, "\n".join(lines) + "\n", dry_run)


def create_cpp_module(project_dir, module_name, dry_run):
    if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", module_name):
        raise SetupError(f"{module_name!r} is not a valid C++ module name; use a .uproject name beginning with a letter or underscore")
    source = project_dir / "Source"
    module = source / module_name
    target = f"""using UnrealBuildTool;
using System.Collections.Generic;

public class {module_name}Target : TargetRules
{{
    public {module_name}Target(TargetInfo Target) : base(Target)
    {{
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.Add(\"{module_name}\");
    }}
}}
"""
    editor = target.replace(f"{module_name}Target", f"{module_name}EditorTarget").replace("TargetType.Game", "TargetType.Editor")
    build = f"""using UnrealBuildTool;

public class {module_name} : ModuleRules
{{
    public {module_name}(ReadOnlyTargetRules Target) : base(Target)
    {{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {{ \"Core\", \"CoreUObject\", \"Engine\" }});
    }}
}}
"""
    implementation = f"""#include \"Modules/ModuleManager.h\"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, {module_name}, \"{module_name}\");
"""
    for path, content in (
        (source / f"{module_name}.Target.cs", target),
        (source / f"{module_name}Editor.Target.cs", editor),
        (module / f"{module_name}.Build.cs", build),
        (module / "Private" / f"{module_name}Module.cpp", implementation),
    ):
        if path.exists():
            log(f"keep existing {path}", dry_run)
        else:
            write_text(path, content, dry_run)


def engine_build(engine, project, dry_run, create_module=False):
    source = project.parent / "Source"
    targets = sorted(source.glob("*Editor.Target.cs"))
    if not targets and not (dry_run and create_module):
        raise SetupError(f"no *Editor.Target.cs under {source}; use --create-module if this project has no C++ target")
    target = targets[0].name[: -len(".Target.cs")] if targets else project.stem + "Editor"
    if os.name == "nt":
        runner = engine / "Engine" / "Build" / "BatchFiles" / "Build.bat"
        platform = "Win64"
    else:
        runner = engine / "Engine" / "Build" / "BatchFiles" / "RunUBT.sh"
        platform = "Linux"
    if not runner.is_file() and not dry_run:
        raise SetupError(f"Unreal Build Tool was not found at {runner}")
    command = [str(runner), target, platform, "Development", f"-Project={project}"]
    log("build: " + shlex.join(command), dry_run)
    if not dry_run:
        subprocess.run(command, check=True)


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("project", type=Path, help="existing Unreal project's <Project>.uproject")
    parser.add_argument("--flight-forge", type=Path, default=Path(__file__).resolve().parents[1], help="FlightForge 1.x checkout (default: this script's repository)")
    parser.add_argument("--copy", action="store_true", help="copy plugins instead of creating symlinks; existing copies are left untouched")
    parser.add_argument("--install-deps", action="store_true", help="run gitman install when glTFRuntime is missing")
    parser.add_argument("--create-module", action="store_true", help="create a minimal C++ module/targets for a Blueprint-only or new project")
    parser.add_argument("--engine", type=Path, help="UE 5.7 root, used for --build and for printing the UBT command")
    parser.add_argument("--build", action="store_true", help="build <Project>Editor after setup; requires --engine")
    parser.add_argument("--dry-run", action="store_true", help="report changes without writing files or running installs/builds")
    return parser.parse_args()


def main():
    args = parse_args()
    if args.build and not args.engine:
        raise SetupError("--build requires --engine=/path/to/UnrealEngine")
    try:
        repository = require_source(args.flight_forge)
        project = args.project.expanduser().resolve()
        if not project.is_file() or project.suffix != ".uproject":
            raise SetupError(f"expected an existing .uproject file, got {project}")
        project_dir = project.parent
        module_name = project.stem
        gltf = install_dependencies(repository, args.install_deps, args.dry_run)

        for name in ("FlightForgePlugin", "UnrealYAML"):
            stage_plugin(repository / name, project_dir / "Plugins" / name, args.copy, args.dry_run)
        stage_plugin(gltf, project_dir / "Plugins" / "glTFRuntime", args.copy, args.dry_run)

        if args.create_module:
            create_cpp_module(project_dir, module_name, args.dry_run)
        update_uproject(project, module_name, args.create_module, args.dry_run)
        set_ini_values(
            project_dir / "Config" / "DefaultEngine.ini",
            "[/Script/EngineSettings.GameMapsSettings]",
            {"GameInstanceClass": GAME_INSTANCE, "GlobalDefaultGameMode": GAME_MODE},
            args.dry_run,
        )
        set_ini_values(project_dir / "Config" / "DefaultEngine.ini", "[/Script/Engine.RendererSettings]", {"r.CustomDepth": "3"}, args.dry_run)
        ensure_cook_path(project_dir / "Config" / "DefaultGame.ini", "/FlightForgePlugin", args.dry_run)
        ensure_cook_path(project_dir / "Config" / "DefaultGame.ini", "/glTFRuntime", args.dry_run)

        classes = project_dir / "Config" / "FlightForgeClasses.yaml"
        if not classes.exists():
            write_text(classes, (repository / "FlightForgePlugin" / "Config" / "FlightForgeClasses.yaml").read_text(encoding="utf-8"), args.dry_run)
        else:
            log(f"keep existing {classes}", args.dry_run)

        if args.engine:
            engine_build(args.engine.expanduser().resolve(), project, args.dry_run, args.create_module)
        else:
            log("setup complete; next build <Project>Editor, then put a PlayerStart on the floor of your map", args.dry_run)
        log("originals of changed files, where present, were saved as <filename>.flightforge.bak", args.dry_run)
    except (SetupError, subprocess.CalledProcessError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
