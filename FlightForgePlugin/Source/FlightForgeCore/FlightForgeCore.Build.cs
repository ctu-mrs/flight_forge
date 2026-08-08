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
		PrivateDependencyModuleNames.AddRange(new string[] { "MessageSerialization", "ProceduralMeshComponent", "UnrealYAML", "AssetRegistry" });

		string thirdPartyIncludePath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/include"));
		PublicIncludePaths.AddRange(new string[] {thirdPartyIncludePath});
	}
}
