// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "../../ueds/uedsGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeuedsGameModeBase() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UEDS_API UClass* Z_Construct_UClass_ADronePawn_NoRegister();
	UEDS_API UClass* Z_Construct_UClass_AuedsGameModeBase();
	UEDS_API UClass* Z_Construct_UClass_AuedsGameModeBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_ueds();
// End Cross Module References
	DEFINE_FUNCTION(AuedsGameModeBase::execSpawnDroneAtLocation)
	{
		P_GET_STRUCT(FVector,Z_Param_Location);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*)Z_Param__Result=P_THIS->SpawnDroneAtLocation(Z_Param_Location);
		P_NATIVE_END;
	}
	void AuedsGameModeBase::StaticRegisterNativesAuedsGameModeBase()
	{
		UClass* Class = AuedsGameModeBase::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "SpawnDroneAtLocation", &AuedsGameModeBase::execSpawnDroneAtLocation },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics
	{
		struct uedsGameModeBase_eventSpawnDroneAtLocation_Parms
		{
			FVector Location;
			int32 ReturnValue;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_Location;
		static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::NewProp_Location = { "Location", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(uedsGameModeBase_eventSpawnDroneAtLocation_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(uedsGameModeBase_eventSpawnDroneAtLocation_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::NewProp_Location,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "uedsGameModeBase.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AuedsGameModeBase, nullptr, "SpawnDroneAtLocation", nullptr, nullptr, Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::PropPointers), sizeof(Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::uedsGameModeBase_eventSpawnDroneAtLocation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::Function_MetaDataParams), Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::uedsGameModeBase_eventSpawnDroneAtLocation_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AuedsGameModeBase);
	UClass* Z_Construct_UClass_AuedsGameModeBase_NoRegister()
	{
		return AuedsGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AuedsGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ForestDensityLevel_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_ForestDensityLevel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ForestHillyLevel_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_ForestHillyLevel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DronePawnClass_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_DronePawnClass;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AuedsGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_ueds,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AuedsGameModeBase_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_AuedsGameModeBase_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_AuedsGameModeBase_SpawnDroneAtLocation, "SpawnDroneAtLocation" }, // 1163862671
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AuedsGameModeBase_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AuedsGameModeBase_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "uedsGameModeBase.h" },
		{ "ModuleRelativePath", "uedsGameModeBase.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_ForestDensityLevel_MetaData[] = {
		{ "BlueprintType", "true" },
		{ "Category", "uedsGameModeBase" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "uedsGameModeBase.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_ForestDensityLevel = { "ForestDensityLevel", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AuedsGameModeBase, ForestDensityLevel), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_ForestDensityLevel_MetaData), Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_ForestDensityLevel_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_ForestHillyLevel_MetaData[] = {
		{ "BlueprintType", "true" },
		{ "Category", "uedsGameModeBase" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "uedsGameModeBase.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_ForestHillyLevel = { "ForestHillyLevel", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AuedsGameModeBase, ForestHillyLevel), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_ForestHillyLevel_MetaData), Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_ForestHillyLevel_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_DronePawnClass_MetaData[] = {
		{ "Category", "Classes" },
		{ "ModuleRelativePath", "uedsGameModeBase.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_DronePawnClass = { "DronePawnClass", nullptr, (EPropertyFlags)0x0014000002000015, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AuedsGameModeBase, DronePawnClass), Z_Construct_UClass_UClass, Z_Construct_UClass_ADronePawn_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_DronePawnClass_MetaData), Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_DronePawnClass_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AuedsGameModeBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_ForestDensityLevel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_ForestHillyLevel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AuedsGameModeBase_Statics::NewProp_DronePawnClass,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AuedsGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AuedsGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AuedsGameModeBase_Statics::ClassParams = {
		&AuedsGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_AuedsGameModeBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_AuedsGameModeBase_Statics::PropPointers),
		0,
		0x009002ACu,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AuedsGameModeBase_Statics::Class_MetaDataParams), Z_Construct_UClass_AuedsGameModeBase_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AuedsGameModeBase_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_AuedsGameModeBase()
	{
		if (!Z_Registration_Info_UClass_AuedsGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AuedsGameModeBase.OuterSingleton, Z_Construct_UClass_AuedsGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AuedsGameModeBase.OuterSingleton;
	}
	template<> UEDS_API UClass* StaticClass<AuedsGameModeBase>()
	{
		return AuedsGameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AuedsGameModeBase);
	AuedsGameModeBase::~AuedsGameModeBase() {}
	struct Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_uedsGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_uedsGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AuedsGameModeBase, AuedsGameModeBase::StaticClass, TEXT("AuedsGameModeBase"), &Z_Registration_Info_UClass_AuedsGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AuedsGameModeBase), 1345120106U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_uedsGameModeBase_h_2249184840(TEXT("/Script/ueds"),
		Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_uedsGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_uedsGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
