
using System.IO;
using UnrealBuildTool;

public class FlightForgeShaders : ModuleRules
{
	public FlightForgeShaders(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "RHI", "RenderCore", "Projects" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Renderer" });

		PrivateIncludePaths.Add(Path.Combine(EngineDirectory, "Source", "Runtime", "Renderer", "Internal"));
		// FViewInfo::DynamicMeshElements: the per-view batches of instanced and skeletal meshes, which register no static ones
		PrivateIncludePaths.Add(Path.Combine(EngineDirectory, "Source", "Runtime", "Renderer", "Private"));

		string Marker = Path.Combine(EngineDirectory, "Build", "FlightForgeEnginePatch.json");
		bool bEnginePatched = File.Exists(Marker);
		if (bEnginePatched)
		{
			ExternalDependencies.Add(Marker);
		}
		PrivateDefinitions.Add("FLIGHTFORGE_NANITE_IDS=" + (bEnginePatched ? "1" : "0"));
		PrivateDefinitions.Add("FLIGHTFORGE_SHARED_RADIANCE_CACHE=" + (bEnginePatched && File.ReadAllText(Marker).Contains("\"shared_radiance_cache\"") ? "1" : "0"));
	}
}
