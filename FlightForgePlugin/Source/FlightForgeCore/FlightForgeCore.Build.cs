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
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Networking", "Sockets", "Projects", "RHI", "RenderCore", "DeveloperSettings", "glTFRuntime", "FlightForgeShaders"});
		PrivateDependencyModuleNames.AddRange(new string[] { "MessageSerialization", "UnrealYAML", "AssetRegistry", "Renderer" });

		if (Target.bBuildEditor)
		{
			// the hit-proxy segmentation ground truth (SegmentationGroundTruth.cpp) reads editor viewports
			PrivateDependencyModuleNames.Add("UnrealEd");
		}

		string thirdPartyIncludePath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/include"));
		PublicIncludePaths.AddRange(new string[] {thirdPartyIncludePath});
	}
}
