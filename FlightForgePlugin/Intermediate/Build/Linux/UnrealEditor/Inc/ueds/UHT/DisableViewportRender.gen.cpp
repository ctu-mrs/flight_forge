// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "../../ueds/RenderSettings/DisableViewportRender.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDisableViewportRender() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
	UEDS_API UClass* Z_Construct_UClass_UDisableViewportRender();
	UEDS_API UClass* Z_Construct_UClass_UDisableViewportRender_NoRegister();
	UPackage* Z_Construct_UPackage__Script_ueds();
// End Cross Module References
	DEFINE_FUNCTION(UDisableViewportRender::execEnableViewPortRender)
	{
		P_GET_UBOOL(Z_Param_bEnabled);
		P_FINISH;
		P_NATIVE_BEGIN;
		UDisableViewportRender::EnableViewPortRender(Z_Param_bEnabled);
		P_NATIVE_END;
	}
	void UDisableViewportRender::StaticRegisterNativesUDisableViewportRender()
	{
		UClass* Class = UDisableViewportRender::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "EnableViewPortRender", &UDisableViewportRender::execEnableViewPortRender },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics
	{
		struct DisableViewportRender_eventEnableViewPortRender_Parms
		{
			bool bEnabled;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bEnabled_MetaData[];
#endif
		static void NewProp_bEnabled_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnabled;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::NewProp_bEnabled_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	void Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::NewProp_bEnabled_SetBit(void* Obj)
	{
		((DisableViewportRender_eventEnableViewPortRender_Parms*)Obj)->bEnabled = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::NewProp_bEnabled = { "bEnabled", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(DisableViewportRender_eventEnableViewPortRender_Parms), &Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::NewProp_bEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::NewProp_bEnabled_MetaData), Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::NewProp_bEnabled_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::NewProp_bEnabled,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::Function_MetaDataParams[] = {
		{ "Category", "Viewport" },
		{ "DisplayName", "Enable ViewPort Render" },
		{ "Keywords", "viewport rendering enable disable" },
		{ "ModuleRelativePath", "RenderSettings/DisableViewportRender.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UDisableViewportRender, nullptr, "EnableViewPortRender", nullptr, nullptr, Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::PropPointers), sizeof(Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::DisableViewportRender_eventEnableViewPortRender_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04042401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::DisableViewportRender_eventEnableViewPortRender_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UDisableViewportRender);
	UClass* Z_Construct_UClass_UDisableViewportRender_NoRegister()
	{
		return UDisableViewportRender::StaticClass();
	}
	struct Z_Construct_UClass_UDisableViewportRender_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UDisableViewportRender_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
		(UObject* (*)())Z_Construct_UPackage__Script_ueds,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDisableViewportRender_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UDisableViewportRender_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UDisableViewportRender_EnableViewPortRender, "EnableViewPortRender" }, // 3306478632
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDisableViewportRender_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDisableViewportRender_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "RenderSettings/DisableViewportRender.h" },
		{ "ModuleRelativePath", "RenderSettings/DisableViewportRender.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UDisableViewportRender_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UDisableViewportRender>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UDisableViewportRender_Statics::ClassParams = {
		&UDisableViewportRender::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UDisableViewportRender_Statics::Class_MetaDataParams), Z_Construct_UClass_UDisableViewportRender_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_UDisableViewportRender()
	{
		if (!Z_Registration_Info_UClass_UDisableViewportRender.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UDisableViewportRender.OuterSingleton, Z_Construct_UClass_UDisableViewportRender_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UDisableViewportRender.OuterSingleton;
	}
	template<> UEDS_API UClass* StaticClass<UDisableViewportRender>()
	{
		return UDisableViewportRender::StaticClass();
	}
	UDisableViewportRender::UDisableViewportRender(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UDisableViewportRender);
	UDisableViewportRender::~UDisableViewportRender() {}
	struct Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_RenderSettings_DisableViewportRender_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_RenderSettings_DisableViewportRender_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UDisableViewportRender, UDisableViewportRender::StaticClass, TEXT("UDisableViewportRender"), &Z_Registration_Info_UClass_UDisableViewportRender, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UDisableViewportRender), 3871373387U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_RenderSettings_DisableViewportRender_h_366609294(TEXT("/Script/ueds"),
		Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_RenderSettings_DisableViewportRender_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_git_unreal_engine_simulator_blueprint_plug_test_MyProject_Plugins_FlightForgePlugin_Source_ueds_RenderSettings_DisableViewportRender_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
