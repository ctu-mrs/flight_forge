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
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Networking", "Sockets", "Projects", "RHI", "RenderCore", "DeveloperSettings", "glTFRuntime"});
		// no ProceduralMeshComponent: unused, and as a Default-phase plugin it cannot be linked
		// by a PostConfigInit module (SegIdPass registers shader types, which must happen early)
		PrivateDependencyModuleNames.AddRange(new string[] { "MessageSerialization", "UnrealYAML", "AssetRegistry", "Renderer" });

		// FPostProcessingInputs (SegIdPass) sits in Renderer/Internal, which UBT propagates only to
		// engine-scoped modules; without this the plugin builds via BuildPlugin but not from a project.
		PrivateIncludePaths.Add(Path.Combine(EngineDirectory, "Source", "Runtime", "Renderer", "Internal"));

		if (Target.bBuildEditor)
		{
			// the hit-proxy segmentation ground truth (SegmentationGroundTruth.cpp) reads editor viewports
			PrivateDependencyModuleNames.Add("UnrealEd");
		}

		string thirdPartyIncludePath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/include"));
		PublicIncludePaths.AddRange(new string[] {thirdPartyIncludePath});
	}
}
