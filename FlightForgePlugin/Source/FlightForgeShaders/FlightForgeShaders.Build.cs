// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

// Holds only what has to exist before the engine starts compiling shaders: the SegId
// mesh pass and its shader types, plus the readback slot the pass writes into. Kept
// apart from FlightForgeCore so the gameplay module can load at the normal Default
// phase instead of being dragged to PostConfigInit by IMPLEMENT_MATERIAL_SHADER_TYPE.
public class FlightForgeShaders : ModuleRules
{
	public FlightForgeShaders(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "RHI", "RenderCore", "Projects" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Renderer" });

		// FPostProcessingInputs (SegIdPass) sits in Renderer/Internal, which UBT propagates only to
		// engine-scoped modules; without this the plugin builds via BuildPlugin but not from a project.
		PrivateIncludePaths.Add(Path.Combine(EngineDirectory, "Source", "Runtime", "Renderer", "Internal"));
	}
}
