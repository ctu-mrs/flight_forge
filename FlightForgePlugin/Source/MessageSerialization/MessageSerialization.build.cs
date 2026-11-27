// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;
using System.Collections.Generic;
using System.Text.RegularExpressions;

public class MessageSerialization : ModuleRules
{
	public MessageSerialization(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableUndefinedIdentifierWarnings = false;
    bEnableExceptions = true;		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
		PrivateDependencyModuleNames.AddRange(new string[] {  });

		string thirdPartyIncludePath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/include"));
		PrivateIncludePaths.Add("MessageSerialization/Private");
		PublicIncludePaths.Add(thirdPartyIncludePath);
		
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
	}
}
