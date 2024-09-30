// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "../../ueds/DronePawn.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDronePawn() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_APawn();
	ENGINE_API UClass* Z_Construct_UClass_UCameraComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UMaterial_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USceneCaptureComponent2D_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USpringArmComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMeshComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UTextureRenderTarget2D_NoRegister();
	UEDS_API UClass* Z_Construct_UClass_ADronePawn();
	UEDS_API UClass* Z_Construct_UClass_ADronePawn_NoRegister();
	UPackage* Z_Construct_UPackage__Script_ueds();
// End Cross Module References
	DEFINE_FUNCTION(ADronePawn::execGetRangefinderData)
	{
		P_GET_PROPERTY_REF(FDoubleProperty,Z_Param_Out_range);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetRangefinderData(Z_Param_Out_range);
		P_NATIVE_END;
	}
	void ADronePawn::StaticRegisterNativesADronePawn()
	{
		UClass* Class = ADronePawn::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetRangefinderData", &ADronePawn::execGetRangefinderData },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics
	{
		struct DronePawn_eventGetRangefinderData_Parms
		{
			double range;
		};
		static const UECodeGen_Private::FDoublePropertyParams NewProp_range;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FDoublePropertyParams Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::NewProp_range = { "range", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Double, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(DronePawn_eventGetRangefinderData_Parms, range), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::NewProp_range,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ADronePawn, nullptr, "GetRangefinderData", nullptr, nullptr, Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::PropPointers), sizeof(Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::DronePawn_eventGetRangefinderData_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::Function_MetaDataParams), Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::DronePawn_eventGetRangefinderData_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_ADronePawn_GetRangefinderData()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ADronePawn_GetRangefinderData_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ADronePawn);
	UClass* Z_Construct_UClass_ADronePawn_NoRegister()
	{
		return ADronePawn::StaticClass();
	}
	struct Z_Construct_UClass_ADronePawn_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ThirdPersonCameraSpringArm_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ThirdPersonCameraSpringArm;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RenderTarget2DRgb_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_RenderTarget2DRgb;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RenderTarget2DStereoLeft_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_RenderTarget2DStereoLeft;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RenderTarget2DStereoRight_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_RenderTarget2DStereoRight;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RenderTarget2DRgbSeg_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_RenderTarget2DRgbSeg;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ThirdPersonCamera_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ThirdPersonCamera;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DroneCamera_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_DroneCamera;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SceneCaptureMeshHolderRgb_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneCaptureMeshHolderRgb;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SceneCaptureMeshHolderRgbSeg_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneCaptureMeshHolderRgbSeg;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SceneCaptureMeshHolderStereoLeft_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneCaptureMeshHolderStereoLeft;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SceneCaptureMeshHolderStereoRight_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneCaptureMeshHolderStereoRight;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SceneCaptureComponent2DRgb_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneCaptureComponent2DRgb;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SceneCaptureComponent2DRgbSeg_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneCaptureComponent2DRgbSeg;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SceneCaptureComponent2DStereoLeft_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneCaptureComponent2DStereoLeft;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SceneCaptureComponent2DStereoRight_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneCaptureComponent2DStereoRight;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PostProcessMaterial_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_PostProcessMaterial;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RootMeshComponent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_RootMeshComponent;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ADronePawn_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APawn,
		(UObject* (*)())Z_Construct_UPackage__Script_ueds,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_ADronePawn_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_ADronePawn_GetRangefinderData, "GetRangefinderData" }, // 2987933994
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "DronePawn.h" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_ThirdPersonCameraSpringArm_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_ThirdPersonCameraSpringArm = { "ThirdPersonCameraSpringArm", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, ThirdPersonCameraSpringArm), Z_Construct_UClass_USpringArmComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_ThirdPersonCameraSpringArm_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_ThirdPersonCameraSpringArm_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DRgb_MetaData[] = {
		{ "Category", "Components" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DRgb = { "RenderTarget2DRgb", nullptr, (EPropertyFlags)0x0010000000020001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, RenderTarget2DRgb), Z_Construct_UClass_UTextureRenderTarget2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DRgb_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DRgb_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DStereoLeft_MetaData[] = {
		{ "Category", "Components" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DStereoLeft = { "RenderTarget2DStereoLeft", nullptr, (EPropertyFlags)0x0010000000020001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, RenderTarget2DStereoLeft), Z_Construct_UClass_UTextureRenderTarget2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DStereoLeft_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DStereoLeft_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DStereoRight_MetaData[] = {
		{ "Category", "Components" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DStereoRight = { "RenderTarget2DStereoRight", nullptr, (EPropertyFlags)0x0010000000020001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, RenderTarget2DStereoRight), Z_Construct_UClass_UTextureRenderTarget2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DStereoRight_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DStereoRight_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DRgbSeg_MetaData[] = {
		{ "Category", "Components" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DRgbSeg = { "RenderTarget2DRgbSeg", nullptr, (EPropertyFlags)0x0010000000020001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, RenderTarget2DRgbSeg), Z_Construct_UClass_UTextureRenderTarget2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DRgbSeg_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DRgbSeg_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_ThirdPersonCamera_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_ThirdPersonCamera = { "ThirdPersonCamera", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, ThirdPersonCamera), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_ThirdPersonCamera_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_ThirdPersonCamera_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_DroneCamera_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_DroneCamera = { "DroneCamera", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, DroneCamera), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_DroneCamera_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_DroneCamera_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderRgb_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderRgb = { "SceneCaptureMeshHolderRgb", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, SceneCaptureMeshHolderRgb), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderRgb_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderRgb_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderRgbSeg_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderRgbSeg = { "SceneCaptureMeshHolderRgbSeg", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, SceneCaptureMeshHolderRgbSeg), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderRgbSeg_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderRgbSeg_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderStereoLeft_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderStereoLeft = { "SceneCaptureMeshHolderStereoLeft", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, SceneCaptureMeshHolderStereoLeft), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderStereoLeft_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderStereoLeft_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderStereoRight_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderStereoRight = { "SceneCaptureMeshHolderStereoRight", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, SceneCaptureMeshHolderStereoRight), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderStereoRight_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderStereoRight_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DRgb_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DRgb = { "SceneCaptureComponent2DRgb", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, SceneCaptureComponent2DRgb), Z_Construct_UClass_USceneCaptureComponent2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DRgb_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DRgb_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DRgbSeg_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DRgbSeg = { "SceneCaptureComponent2DRgbSeg", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, SceneCaptureComponent2DRgbSeg), Z_Construct_UClass_USceneCaptureComponent2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DRgbSeg_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DRgbSeg_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DStereoLeft_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DStereoLeft = { "SceneCaptureComponent2DStereoLeft", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, SceneCaptureComponent2DStereoLeft), Z_Construct_UClass_USceneCaptureComponent2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DStereoLeft_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DStereoLeft_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DStereoRight_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DStereoRight = { "SceneCaptureComponent2DStereoRight", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, SceneCaptureComponent2DStereoRight), Z_Construct_UClass_USceneCaptureComponent2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DStereoRight_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DStereoRight_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_PostProcessMaterial_MetaData[] = {
		{ "Category", "Segmentation PostProcess Setup" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// PostProcessMaterial used for segmentation\n" },
#endif
		{ "ModuleRelativePath", "DronePawn.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "PostProcessMaterial used for segmentation" },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_PostProcessMaterial = { "PostProcessMaterial", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, PostProcessMaterial), Z_Construct_UClass_UMaterial_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_PostProcessMaterial_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_PostProcessMaterial_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADronePawn_Statics::NewProp_RootMeshComponent_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "DronePawn.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADronePawn_Statics::NewProp_RootMeshComponent = { "RootMeshComponent", nullptr, (EPropertyFlags)0x00100000000a000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ADronePawn, RootMeshComponent), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::NewProp_RootMeshComponent_MetaData), Z_Construct_UClass_ADronePawn_Statics::NewProp_RootMeshComponent_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ADronePawn_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_ThirdPersonCameraSpringArm,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DRgb,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DStereoLeft,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DStereoRight,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_RenderTarget2DRgbSeg,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_ThirdPersonCamera,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_DroneCamera,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderRgb,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderRgbSeg,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderStereoLeft,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureMeshHolderStereoRight,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DRgb,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DRgbSeg,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DStereoLeft,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_SceneCaptureComponent2DStereoRight,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_PostProcessMaterial,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADronePawn_Statics::NewProp_RootMeshComponent,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ADronePawn_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ADronePawn>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ADronePawn_Statics::ClassParams = {
		&ADronePawn::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_ADronePawn_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::Class_MetaDataParams), Z_Construct_UClass_ADronePawn_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ADronePawn_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_ADronePawn()
	{
		if (!Z_Registration_Info_UClass_ADronePawn.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ADronePawn.OuterSingleton, Z_Construct_UClass_ADronePawn_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ADronePawn.OuterSingleton;
	}
	template<> UEDS_API UClass* StaticClass<ADronePawn>()
	{
		return ADronePawn::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ADronePawn);
	ADronePawn::~ADronePawn() {}
	struct Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_DronePawn_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_DronePawn_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ADronePawn, ADronePawn::StaticClass, TEXT("ADronePawn"), &Z_Registration_Info_UClass_ADronePawn, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ADronePawn), 2378143992U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_DronePawn_h_668569741(TEXT("/Script/ueds"),
		Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_DronePawn_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_DronePawn_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
