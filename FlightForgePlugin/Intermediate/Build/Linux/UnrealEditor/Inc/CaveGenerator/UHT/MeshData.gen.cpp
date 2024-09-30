// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CaveGenerator/MeshData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMeshData() {}
// Cross Module References
	CAVEGENERATOR_API UScriptStruct* Z_Construct_UScriptStruct_FMeshData();
	UPackage* Z_Construct_UPackage__Script_CaveGenerator();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_MeshData;
class UScriptStruct* FMeshData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_MeshData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_MeshData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMeshData, (UObject*)Z_Construct_UPackage__Script_CaveGenerator(), TEXT("MeshData"));
	}
	return Z_Registration_Info_UScriptStruct_MeshData.OuterSingleton;
}
template<> CAVEGENERATOR_API UScriptStruct* StaticStruct<FMeshData>()
{
	return FMeshData::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FMeshData_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FMeshData_Statics::Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "MeshData.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FMeshData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMeshData>();
	}
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMeshData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_CaveGenerator,
		nullptr,
		&NewStructOps,
		"MeshData",
		nullptr,
		0,
		sizeof(FMeshData),
		alignof(FMeshData),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMeshData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMeshData_Statics::Struct_MetaDataParams)
	};
	UScriptStruct* Z_Construct_UScriptStruct_FMeshData()
	{
		if (!Z_Registration_Info_UScriptStruct_MeshData.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_MeshData.InnerSingleton, Z_Construct_UScriptStruct_FMeshData_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_MeshData.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_MeshData_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_MeshData_h_Statics::ScriptStructInfo[] = {
		{ FMeshData::StaticStruct, Z_Construct_UScriptStruct_FMeshData_Statics::NewStructOps, TEXT("MeshData"), &Z_Registration_Info_UScriptStruct_MeshData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMeshData), 1554749649U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_MeshData_h_2518124738(TEXT("/Script/CaveGenerator"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_MeshData_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_MeshData_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
