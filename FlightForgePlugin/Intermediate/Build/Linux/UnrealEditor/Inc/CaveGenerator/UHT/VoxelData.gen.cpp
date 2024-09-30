// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CaveGenerator/VoxelData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVoxelData() {}
// Cross Module References
	CAVEGENERATOR_API UScriptStruct* Z_Construct_UScriptStruct_FVoxelData();
	UPackage* Z_Construct_UPackage__Script_CaveGenerator();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VoxelData;
class UScriptStruct* FVoxelData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VoxelData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VoxelData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVoxelData, (UObject*)Z_Construct_UPackage__Script_CaveGenerator(), TEXT("VoxelData"));
	}
	return Z_Registration_Info_UScriptStruct_VoxelData.OuterSingleton;
}
template<> CAVEGENERATOR_API UScriptStruct* StaticStruct<FVoxelData>()
{
	return FVoxelData::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVoxelData_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVoxelData_Statics::Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "VoxelData.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVoxelData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVoxelData>();
	}
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVoxelData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_CaveGenerator,
		nullptr,
		&NewStructOps,
		"VoxelData",
		nullptr,
		0,
		sizeof(FVoxelData),
		alignof(FVoxelData),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVoxelData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVoxelData_Statics::Struct_MetaDataParams)
	};
	UScriptStruct* Z_Construct_UScriptStruct_FVoxelData()
	{
		if (!Z_Registration_Info_UScriptStruct_VoxelData.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VoxelData.InnerSingleton, Z_Construct_UScriptStruct_FVoxelData_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VoxelData.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_VoxelData_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_VoxelData_h_Statics::ScriptStructInfo[] = {
		{ FVoxelData::StaticStruct, Z_Construct_UScriptStruct_FVoxelData_Statics::NewStructOps, TEXT("VoxelData"), &Z_Registration_Info_UScriptStruct_VoxelData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVoxelData), 2890876299U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_VoxelData_h_1441885918(TEXT("/Script/CaveGenerator"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_VoxelData_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_VoxelData_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
