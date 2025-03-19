// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/InputRebindData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeInputRebindData() {}

// Begin Cross Module References
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTableRowBase();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputRebindData();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin ScriptStruct FInputRebindData
static_assert(std::is_polymorphic<FInputRebindData>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FInputRebindData cannot be polymorphic unless super FTableRowBase is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_InputRebindData;
class UScriptStruct* FInputRebindData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_InputRebindData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_InputRebindData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FInputRebindData, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("InputRebindData"));
	}
	return Z_Registration_Info_UScriptStruct_InputRebindData.OuterSingleton;
}
template<> UINAVIGATION_API UScriptStruct* StaticStruct<FInputRebindData>()
{
	return FInputRebindData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FInputRebindData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/InputRebindData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputText_MetaData[] = {
		{ "Category", "InputRebindData" },
		{ "ModuleRelativePath", "Public/Data/InputRebindData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputGroups_MetaData[] = {
		{ "Category", "InputRebindData" },
		{ "ModuleRelativePath", "Public/Data/InputRebindData.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_InputText;
	static const UECodeGen_Private::FIntPropertyParams NewProp_InputGroups_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InputGroups;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FInputRebindData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FInputRebindData_Statics::NewProp_InputText = { "InputText", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputRebindData, InputText), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputText_MetaData), NewProp_InputText_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FInputRebindData_Statics::NewProp_InputGroups_Inner = { "InputGroups", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FInputRebindData_Statics::NewProp_InputGroups = { "InputGroups", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputRebindData, InputGroups), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputGroups_MetaData), NewProp_InputGroups_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FInputRebindData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputRebindData_Statics::NewProp_InputText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputRebindData_Statics::NewProp_InputGroups_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputRebindData_Statics::NewProp_InputGroups,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputRebindData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FInputRebindData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
	Z_Construct_UScriptStruct_FTableRowBase,
	&NewStructOps,
	"InputRebindData",
	Z_Construct_UScriptStruct_FInputRebindData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputRebindData_Statics::PropPointers),
	sizeof(FInputRebindData),
	alignof(FInputRebindData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputRebindData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FInputRebindData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FInputRebindData()
{
	if (!Z_Registration_Info_UScriptStruct_InputRebindData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_InputRebindData.InnerSingleton, Z_Construct_UScriptStruct_FInputRebindData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_InputRebindData.InnerSingleton;
}
// End ScriptStruct FInputRebindData

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputRebindData_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FInputRebindData::StaticStruct, Z_Construct_UScriptStruct_FInputRebindData_Statics::NewStructOps, TEXT("InputRebindData"), &Z_Registration_Info_UScriptStruct_InputRebindData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FInputRebindData), 2728884328U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputRebindData_h_3602966604(TEXT("/Script/UINavigation"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputRebindData_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputRebindData_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
