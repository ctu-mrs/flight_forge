// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CaveGenerator/Private/CaveManager.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCaveManager() {}
// Cross Module References
	CAVEGENERATOR_API UClass* Z_Construct_UClass_ACaveManager();
	CAVEGENERATOR_API UClass* Z_Construct_UClass_ACaveManager_NoRegister();
	CAVEGENERATOR_API UClass* Z_Construct_UClass_AMarchingRender_NoRegister();
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FIntVector();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_CaveGenerator();
// End Cross Module References
	void ACaveManager::StaticRegisterNativesACaveManager()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ACaveManager);
	UClass* Z_Construct_UClass_ACaveManager_NoRegister()
	{
		return ACaveManager::StaticClass();
	}
	struct Z_Construct_UClass_ACaveManager_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Seed_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_Seed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ChunksCountInDirectionXYZ_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_ChunksCountInDirectionXYZ;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ChunkSize_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_ChunkSize;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MinHoleRadius_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_MinHoleRadius;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxHoleRadius_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_MaxHoleRadius;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bSmoothMesh_MetaData[];
#endif
		static void NewProp_bSmoothMesh_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bSmoothMesh;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bSpawnLights_MetaData[];
#endif
		static void NewProp_bSpawnLights_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bSpawnLights;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MINStepsBetweenLights_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_MINStepsBetweenLights;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MAXStepsBetweenLights_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_MAXStepsBetweenLights;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MINStepsBetweenStalagmites_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_MINStepsBetweenStalagmites;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MAXStepsBetweenStalagmites_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_MAXStepsBetweenStalagmites;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_StalagmitesDensity_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_StalagmitesDensity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BottomStalagmitesDensity_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BottomStalagmitesDensity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MINStalagmitesScale_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MINStalagmitesScale;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MAXStalagmitesScale_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MAXStalagmitesScale;
		static const UECodeGen_Private::FClassPropertyParams NewProp_StalagmiteBlueprintArray_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_StalagmiteBlueprintArray_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_StalagmiteBlueprintArray;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LightBlueprint_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_LightBlueprint;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ChunkBlueprint_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_ChunkBlueprint;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bDebug_MetaData[];
#endif
		static void NewProp_bDebug_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDebug;
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Chunks_ValueProp;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Chunks_Key_KeyProp;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Chunks_MetaData[];
#endif
		static const UECodeGen_Private::FMapPropertyParams NewProp_Chunks;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ACaveManager_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_CaveGenerator,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "CaveManager.h" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_Seed_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_Seed = { "Seed", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, Seed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_Seed_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_Seed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunksCountInDirectionXYZ_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunksCountInDirectionXYZ = { "ChunksCountInDirectionXYZ", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, ChunksCountInDirectionXYZ), Z_Construct_UScriptStruct_FIntVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunksCountInDirectionXYZ_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunksCountInDirectionXYZ_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunkSize_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunkSize = { "ChunkSize", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, ChunkSize), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunkSize_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunkSize_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_MinHoleRadius_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_MinHoleRadius = { "MinHoleRadius", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, MinHoleRadius), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_MinHoleRadius_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_MinHoleRadius_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_MaxHoleRadius_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_MaxHoleRadius = { "MaxHoleRadius", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, MaxHoleRadius), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_MaxHoleRadius_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_MaxHoleRadius_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_bSmoothMesh_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	void Z_Construct_UClass_ACaveManager_Statics::NewProp_bSmoothMesh_SetBit(void* Obj)
	{
		((ACaveManager*)Obj)->bSmoothMesh = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_bSmoothMesh = { "bSmoothMesh", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ACaveManager), &Z_Construct_UClass_ACaveManager_Statics::NewProp_bSmoothMesh_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_bSmoothMesh_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_bSmoothMesh_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_bSpawnLights_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	void Z_Construct_UClass_ACaveManager_Statics::NewProp_bSpawnLights_SetBit(void* Obj)
	{
		((ACaveManager*)Obj)->bSpawnLights = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_bSpawnLights = { "bSpawnLights", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ACaveManager), &Z_Construct_UClass_ACaveManager_Statics::NewProp_bSpawnLights_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_bSpawnLights_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_bSpawnLights_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStepsBetweenLights_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStepsBetweenLights = { "MINStepsBetweenLights", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, MINStepsBetweenLights), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStepsBetweenLights_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStepsBetweenLights_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStepsBetweenLights_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStepsBetweenLights = { "MAXStepsBetweenLights", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, MAXStepsBetweenLights), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStepsBetweenLights_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStepsBetweenLights_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStepsBetweenStalagmites_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStepsBetweenStalagmites = { "MINStepsBetweenStalagmites", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, MINStepsBetweenStalagmites), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStepsBetweenStalagmites_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStepsBetweenStalagmites_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStepsBetweenStalagmites_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStepsBetweenStalagmites = { "MAXStepsBetweenStalagmites", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, MAXStepsBetweenStalagmites), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStepsBetweenStalagmites_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStepsBetweenStalagmites_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmitesDensity_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
		{ "UIMax", "1.0" },
		{ "UIMin", "0.0" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmitesDensity = { "StalagmitesDensity", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, StalagmitesDensity), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmitesDensity_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmitesDensity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_BottomStalagmitesDensity_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
		{ "UIMax", "1.0" },
		{ "UIMin", "0.0" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_BottomStalagmitesDensity = { "BottomStalagmitesDensity", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, BottomStalagmitesDensity), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_BottomStalagmitesDensity_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_BottomStalagmitesDensity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStalagmitesScale_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStalagmitesScale = { "MINStalagmitesScale", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, MINStalagmitesScale), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStalagmitesScale_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStalagmitesScale_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStalagmitesScale_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStalagmitesScale = { "MAXStalagmitesScale", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, MAXStalagmitesScale), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStalagmitesScale_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStalagmitesScale_MetaData) };
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmiteBlueprintArray_Inner = { "StalagmiteBlueprintArray", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmiteBlueprintArray_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmiteBlueprintArray = { "StalagmiteBlueprintArray", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, StalagmiteBlueprintArray), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmiteBlueprintArray_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmiteBlueprintArray_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_LightBlueprint_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_LightBlueprint = { "LightBlueprint", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, LightBlueprint), Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_LightBlueprint_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_LightBlueprint_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunkBlueprint_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunkBlueprint = { "ChunkBlueprint", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, ChunkBlueprint), Z_Construct_UClass_UClass, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunkBlueprint_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunkBlueprint_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_bDebug_MetaData[] = {
		{ "Category", "CaveManager" },
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	void Z_Construct_UClass_ACaveManager_Statics::NewProp_bDebug_SetBit(void* Obj)
	{
		((ACaveManager*)Obj)->bDebug = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_bDebug = { "bDebug", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(ACaveManager), &Z_Construct_UClass_ACaveManager_Statics::NewProp_bDebug_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_bDebug_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_bDebug_MetaData) };
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_Chunks_ValueProp = { "Chunks", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_AMarchingRender_NoRegister, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_Chunks_Key_KeyProp = { "Chunks_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FIntVector, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACaveManager_Statics::NewProp_Chunks_MetaData[] = {
		{ "ModuleRelativePath", "Private/CaveManager.h" },
	};
#endif
	const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_ACaveManager_Statics::NewProp_Chunks = { "Chunks", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaveManager, Chunks), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::NewProp_Chunks_MetaData), Z_Construct_UClass_ACaveManager_Statics::NewProp_Chunks_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ACaveManager_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_Seed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunksCountInDirectionXYZ,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunkSize,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_MinHoleRadius,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_MaxHoleRadius,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_bSmoothMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_bSpawnLights,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStepsBetweenLights,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStepsBetweenLights,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStepsBetweenStalagmites,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStepsBetweenStalagmites,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmitesDensity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_BottomStalagmitesDensity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_MINStalagmitesScale,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_MAXStalagmitesScale,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmiteBlueprintArray_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_StalagmiteBlueprintArray,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_LightBlueprint,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_ChunkBlueprint,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_bDebug,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_Chunks_ValueProp,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_Chunks_Key_KeyProp,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaveManager_Statics::NewProp_Chunks,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ACaveManager_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACaveManager>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ACaveManager_Statics::ClassParams = {
		&ACaveManager::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_ACaveManager_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::Class_MetaDataParams), Z_Construct_UClass_ACaveManager_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACaveManager_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_ACaveManager()
	{
		if (!Z_Registration_Info_UClass_ACaveManager.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ACaveManager.OuterSingleton, Z_Construct_UClass_ACaveManager_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ACaveManager.OuterSingleton;
	}
	template<> CAVEGENERATOR_API UClass* StaticClass<ACaveManager>()
	{
		return ACaveManager::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ACaveManager);
	ACaveManager::~ACaveManager() {}
	struct Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_Private_CaveManager_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_Private_CaveManager_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ACaveManager, ACaveManager::StaticClass, TEXT("ACaveManager"), &Z_Registration_Info_UClass_ACaveManager, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ACaveManager), 3289319459U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_Private_CaveManager_h_3178051763(TEXT("/Script/CaveGenerator"),
		Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_Private_CaveManager_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_CaveGenerator_Private_CaveManager_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
