// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;
using System.Collections.Generic;
using System.Text.RegularExpressions;

public class FlightForgeCore : ModuleRules
{
	public FlightForgeCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		//bUseRTTI = true;

    // Opt into UE 5.7 build settings
    DefaultBuildSettings = BuildSettingsVersion.V6;
    IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		
		bEnableUndefinedIdentifierWarnings = false;
    bEnableExceptions = true;
    //bUseRTTI = true;
		// FlightForgeShaders carries the SegId mesh pass and its shader types; it loads at
		// PostConfigInit so this module can stay on the normal Default phase.
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Landscape", "Networking", "Sockets", "Projects", "RHI", "RenderCore", "DeveloperSettings", "glTFRuntime", "FlightForgeShaders"});
		PrivateDependencyModuleNames.AddRange(new string[] { "MessageSerialization", "UnrealYAML", "AssetRegistry", "Renderer", "Chaos", "Slate", "SlateCore", "UMG", "ApplicationCore", "ImageWrapper", "ImageCore" });

		if (Target.bBuildEditor)
		{
			// the hit-proxy segmentation ground truth (SegmentationGroundTruth.cpp) reads editor viewports
			PrivateDependencyModuleNames.Add("UnrealEd");
		}

		// read from disk at runtime, so the cooker would leave them out of a package
		RuntimeDependencies.Add("$(PluginDir)/Content/Lidar/*.ffpat", StagedFileType.NonUFS);
		foreach (string Housing in new string[] { "Mid360", "OUSTER_Rev7_OS0", "Garmin", "Odin1" })
		{
			RuntimeDependencies.Add("$(PluginDir)/Content/Meshes/Sensors/" + Housing + ".glb", StagedFileType.NonUFS);
		}
		RuntimeDependencies.Add("$(PluginDir)/Content/ExternalModels/...", StagedFileType.NonUFS);
		// get_info serves it
		RuntimeDependencies.Add("$(PluginDir)/Source/MessageSerialization/Public/Generated/flightforge.bfbs", StagedFileType.NonUFS);
		// the segmentation class tree is read from Config/ at runtime, and only .ini files stage there by default
		string ProjectConfig = Target.ProjectFile == null ? "" : Path.Combine(Target.ProjectFile.Directory.FullName, "Config");
		if (Directory.Exists(ProjectConfig))
		{
			foreach (string ConfigFile in Directory.GetFiles(ProjectConfig, "*.yaml"))
			{
				RuntimeDependencies.Add(ConfigFile, StagedFileType.NonUFS);
			}
		}
		string ProjectFiles = Target.ProjectFile == null ? "" : Path.Combine(Target.ProjectFile.Directory.FullName, "FlightForge");
		if (Directory.Exists(ProjectFiles))
		{
			// listed one by one: the ... wildcard drops symlinked models
			foreach (string ProjectFile in Directory.GetFiles(ProjectFiles, "*", SearchOption.AllDirectories))
			{
				RuntimeDependencies.Add(ProjectFile, StagedFileType.NonUFS);
			}
		}

		string thirdPartyIncludePath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/include"));
		PublicIncludePaths.AddRange(new string[] {thirdPartyIncludePath});
	}
}
