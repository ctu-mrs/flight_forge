// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/PlatformConfigData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePlatformConfigData() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UDataTable_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FPlatformConfigData();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin ScriptStruct FPlatformConfigData
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_PlatformConfigData;
class UScriptStruct* FPlatformConfigData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_PlatformConfigData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_PlatformConfigData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FPlatformConfigData, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("PlatformConfigData"));
	}
	return Z_Registration_Info_UScriptStruct_PlatformConfigData.OuterSingleton;
}
template<> UINAVIGATION_API UScriptStruct* StaticStruct<FPlatformConfigData>()
{
	return FPlatformConfigData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FPlatformConfigData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/PlatformConfigData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GamepadKeyIconData_MetaData[] = {
		{ "Category", "PlatformConfigData" },
		{ "ModuleRelativePath", "Public/Data/PlatformConfigData.h" },
		{ "RequiredAssetDataTags", "RowStructure=/Script/UINavigation.InputIconMapping" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GamepadKeyNameData_MetaData[] = {
		{ "Category", "PlatformConfigData" },
		{ "ModuleRelativePath", "Public/Data/PlatformConfigData.h" },
		{ "RequiredAssetDataTags", "RowStructure=/Script/UINavigation.InputNameMapping" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UINavInputContextOverride_MetaData[] = {
		{ "Category", "PlatformConfigData" },
		{ "ModuleRelativePath", "Public/Data/PlatformConfigData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanUseKeyboardMouse_MetaData[] = {
		{ "Category", "PlatformConfigData" },
		{ "ModuleRelativePath", "Public/Data/PlatformConfigData.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_GamepadKeyIconData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_GamepadKeyNameData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UINavInputContextOverride;
	static void NewProp_bCanUseKeyboardMouse_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanUseKeyboardMouse;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPlatformConfigData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewProp_GamepadKeyIconData = { "GamepadKeyIconData", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FPlatformConfigData, GamepadKeyIconData), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GamepadKeyIconData_MetaData), NewProp_GamepadKeyIconData_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewProp_GamepadKeyNameData = { "GamepadKeyNameData", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FPlatformConfigData, GamepadKeyNameData), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GamepadKeyNameData_MetaData), NewProp_GamepadKeyNameData_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewProp_UINavInputContextOverride = { "UINavInputContextOverride", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FPlatformConfigData, UINavInputContextOverride), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UINavInputContextOverride_MetaData), NewProp_UINavInputContextOverride_MetaData) };
void Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewProp_bCanUseKeyboardMouse_SetBit(void* Obj)
{
	((FPlatformConfigData*)Obj)->bCanUseKeyboardMouse = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewProp_bCanUseKeyboardMouse = { "bCanUseKeyboardMouse", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FPlatformConfigData), &Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewProp_bCanUseKeyboardMouse_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanUseKeyboardMouse_MetaData), NewProp_bCanUseKeyboardMouse_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FPlatformConfigData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewProp_GamepadKeyIconData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewProp_GamepadKeyNameData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewProp_UINavInputContextOverride,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewProp_bCanUseKeyboardMouse,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPlatformConfigData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FPlatformConfigData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	&NewStructOps,
	"PlatformConfigData",
	Z_Construct_UScriptStruct_FPlatformConfigData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPlatformConfigData_Statics::PropPointers),
	sizeof(FPlatformConfigData),
	alignof(FPlatformConfigData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPlatformConfigData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FPlatformConfigData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FPlatformConfigData()
{
	if (!Z_Registration_Info_UScriptStruct_PlatformConfigData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_PlatformConfigData.InnerSingleton, Z_Construct_UScriptStruct_FPlatformConfigData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_PlatformConfigData.InnerSingleton;
}
// End ScriptStruct FPlatformConfigData

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PlatformConfigData_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FPlatformConfigData::StaticStruct, Z_Construct_UScriptStruct_FPlatformConfigData_Statics::NewStructOps, TEXT("PlatformConfigData"), &Z_Registration_Info_UScriptStruct_PlatformConfigData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FPlatformConfigData), 232835755U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PlatformConfigData_h_2942276405(TEXT("/Script/UINavigation"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PlatformConfigData_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PlatformConfigData_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
