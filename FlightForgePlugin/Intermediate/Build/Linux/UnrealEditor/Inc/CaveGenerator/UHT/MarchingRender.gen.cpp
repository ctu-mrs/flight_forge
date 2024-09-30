// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CaveGenerator/Private/MarchingRender.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMarchingRender() {}
// Cross Module References
	CAVEGENERATOR_API UClass* Z_Construct_UClass_AMarchingRender();
	CAVEGENERATOR_API UClass* Z_Construct_UClass_AMarchingRender_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialInterface_NoRegister();
	PROCEDURALMESHCOMPONENT_API UClass* Z_Construct_UClass_UProceduralMeshComponent_NoRegister();
	UPackage* Z_Construct_UPackage__Script_CaveGenerator();
// End Cross Module References
	DEFINE_FUNCTION(AMarchingRender::execUpdateProceduralMesh)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdateProceduralMesh();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(AMarchingRender::execCreateMeshData)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->CreateMeshData();
		P_NATIVE_END;
	}
	void AMarchingRender::StaticRegisterNativesAMarchingRender()
	{
		UClass* Class = AMarchingRender::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "CreateMeshData", &AMarchingRender::execCreateMeshData },
			{ "UpdateProceduralMesh", &AMarchingRender::execUpdateProceduralMesh },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_AMarchingRender_CreateMeshData_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AMarchingRender_CreateMeshData_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Private/MarchingRender.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AMarchingRender_CreateMeshData_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AMarchingRender, nullptr, "CreateMeshData", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AMarchingRender_CreateMeshData_Statics::Function_MetaDataParams), Z_Construct_UFunction_AMarchingRender_CreateMeshData_Statics::Function_MetaDataParams) };
	UFunction* Z_Construct_UFunction_AMarchingRender_CreateMeshData()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AMarchingRender_CreateMeshData_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_AMarchingRender_UpdateProceduralMesh_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AMarchingRender_UpdateProceduralMesh_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Private/MarchingRender.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AMarchingRender_UpdateProceduralMesh_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AMarchingRender, nullptr, "UpdateProceduralMesh", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AMarchingRender_UpdateProceduralMesh_Statics::Function_MetaDataParams), Z_Construct_UFunction_AMarchingRender_UpdateProceduralMesh_Statics::Function_MetaDataParams) };
	UFunction* Z_Construct_UFunction_AMarchingRender_UpdateProceduralMesh()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AMarchingRender_UpdateProceduralMesh_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMarchingRender);
	UClass* Z_Construct_UClass_AMarchingRender_NoRegister()
	{
		return AMarchingRender::StaticClass();
	}
	struct Z_Construct_UClass_AMarchingRender_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Size_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_Size;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_IsoLevel_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_IsoLevel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bSmoothMesh_MetaData[];
#endif
		static void NewProp_bSmoothMesh_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bSmoothMesh;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bInterpolate_MetaData[];
#endif
		static void NewProp_bInterpolate_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bInterpolate;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bDrawDebug_MetaData[];
#endif
		static void NewProp_bDrawDebug_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDrawDebug;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Material_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Material;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ProceduralMeshComponent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ProceduralMeshComponent;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AMarchingRender_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_CaveGenerator,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_AMarchingRender_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_AMarchingRender_CreateMeshData, "CreateMeshData" }, // 1823878011
		{ &Z_Construct_UFunction_AMarchingRender_UpdateProceduralMesh, "UpdateProceduralMesh" }, // 2286854306
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMarchingRender_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MarchingRender.h" },
		{ "ModuleRelativePath", "Private/MarchingRender.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMarchingRender_Statics::NewProp_Size_MetaData[] = {
		{ "Category", "MarchingRender" },
		{ "ModuleRelativePath", "Private/MarchingRender.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AMarchingRender_Statics::NewProp_Size = { "Size", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMarchingRender, Size), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::NewProp_Size_MetaData), Z_Construct_UClass_AMarchingRender_Statics::NewProp_Size_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMarchingRender_Statics::NewProp_IsoLevel_MetaData[] = {
		{ "Category", "MarchingRender" },
		{ "ModuleRelativePath", "Private/MarchingRender.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AMarchingRender_Statics::NewProp_IsoLevel = { "IsoLevel", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMarchingRender, IsoLevel), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::NewProp_IsoLevel_MetaData), Z_Construct_UClass_AMarchingRender_Statics::NewProp_IsoLevel_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMarchingRender_Statics::NewProp_bSmoothMesh_MetaData[] = {
		{ "Category", "MarchingRender" },
		{ "ModuleRelativePath", "Private/MarchingRender.h" },
	};
#endif
	void Z_Construct_UClass_AMarchingRender_Statics::NewProp_bSmoothMesh_SetBit(void* Obj)
	{
		((AMarchingRender*)Obj)->bSmoothMesh = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AMarchingRender_Statics::NewProp_bSmoothMesh = { "bSmoothMesh", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AMarchingRender), &Z_Construct_UClass_AMarchingRender_Statics::NewProp_bSmoothMesh_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::NewProp_bSmoothMesh_MetaData), Z_Construct_UClass_AMarchingRender_Statics::NewProp_bSmoothMesh_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMarchingRender_Statics::NewProp_bInterpolate_MetaData[] = {
		{ "Category", "MarchingRender" },
		{ "ModuleRelativePath", "Private/MarchingRender.h" },
	};
#endif
	void Z_Construct_UClass_AMarchingRender_Statics::NewProp_bInterpolate_SetBit(void* Obj)
	{
		((AMarchingRender*)Obj)->bInterpolate = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AMarchingRender_Statics::NewProp_bInterpolate = { "bInterpolate", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AMarchingRender), &Z_Construct_UClass_AMarchingRender_Statics::NewProp_bInterpolate_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::NewProp_bInterpolate_MetaData), Z_Construct_UClass_AMarchingRender_Statics::NewProp_bInterpolate_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMarchingRender_Statics::NewProp_bDrawDebug_MetaData[] = {
		{ "Category", "MarchingRender" },
		{ "ModuleRelativePath", "Private/MarchingRender.h" },
	};
#endif
	void Z_Construct_UClass_AMarchingRender_Statics::NewProp_bDrawDebug_SetBit(void* Obj)
	{
		((AMarchingRender*)Obj)->bDrawDebug = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AMarchingRender_Statics::NewProp_bDrawDebug = { "bDrawDebug", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AMarchingRender), &Z_Construct_UClass_AMarchingRender_Statics::NewProp_bDrawDebug_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::NewProp_bDrawDebug_MetaData), Z_Construct_UClass_AMarchingRender_Statics::NewProp_bDrawDebug_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMarchingRender_Statics::NewProp_Material_MetaData[] = {
		{ "Category", "MarchingRender" },
		{ "ModuleRelativePath", "Private/MarchingRender.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AMarchingRender_Statics::NewProp_Material = { "Material", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMarchingRender, Material), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::NewProp_Material_MetaData), Z_Construct_UClass_AMarchingRender_Statics::NewProp_Material_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMarchingRender_Statics::NewProp_ProceduralMeshComponent_MetaData[] = {
		{ "Category", "MarchingRender" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Private/MarchingRender.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AMarchingRender_Statics::NewProp_ProceduralMeshComponent = { "ProceduralMeshComponent", nullptr, (EPropertyFlags)0x0040000000080009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AMarchingRender, ProceduralMeshComponent), Z_Construct_UClass_UProceduralMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::NewProp_ProceduralMeshComponent_MetaData), Z_Construct_UClass_AMarchingRender_Statics::NewProp_ProceduralMeshComponent_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AMarchingRender_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMarchingRender_Statics::NewProp_Size,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMarchingRender_Statics::NewProp_IsoLevel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMarchingRender_Statics::NewProp_bSmoothMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMarchingRender_Statics::NewProp_bInterpolate,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMarchingRender_Statics::NewProp_bDrawDebug,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMarchingRender_Statics::NewProp_Material,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AMarchingRender_Statics::NewProp_ProceduralMeshComponent,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AMarchingRender_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMarchingRender>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AMarchingRender_Statics::ClassParams = {
		&AMarchingRender::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_AMarchingRender_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::PropPointers),
		0,
		0x008000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::Class_MetaDataParams), Z_Construct_UClass_AMarchingRender_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMarchingRender_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_AMarchingRender()
	{
		if (!Z_Registration_Info_UClass_AMarchingRender.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMarchingRender.OuterSingleton, Z_Construct_UClass_AMarchingRender_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AMarchingRender.OuterSingleton;
	}
	template<> CAVEGENERATOR_API UClass* StaticClass<AMarchingRender>()
	{
		return AMarchingRender::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AMarchingRender);
	AMarchingRender::~AMarchingRender() {}
	struct Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_Private_MarchingRender_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_Private_MarchingRender_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AMarchingRender, AMarchingRender::StaticClass, TEXT("AMarchingRender"), &Z_Registration_Info_UClass_AMarchingRender, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMarchingRender), 89262771U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_Private_MarchingRender_h_4261637439(TEXT("/Script/CaveGenerator"),
		Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_Private_MarchingRender_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_Private_MarchingRender_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
