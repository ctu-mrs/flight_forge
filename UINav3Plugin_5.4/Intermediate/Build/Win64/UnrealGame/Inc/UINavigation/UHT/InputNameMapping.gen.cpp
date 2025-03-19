// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/InputNameMapping.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeInputNameMapping() {}

// Begin Cross Module References
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTableRowBase();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputNameMapping();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin ScriptStruct FInputNameMapping
static_assert(std::is_polymorphic<FInputNameMapping>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FInputNameMapping cannot be polymorphic unless super FTableRowBase is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_InputNameMapping;
class UScriptStruct* FInputNameMapping::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_InputNameMapping.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_InputNameMapping.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FInputNameMapping, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("InputNameMapping"));
	}
	return Z_Registration_Info_UScriptStruct_InputNameMapping.OuterSingleton;
}
template<> UINAVIGATION_API UScriptStruct* StaticStruct<FInputNameMapping>()
{
	return FInputNameMapping::StaticStruct();
}
struct Z_Construct_UScriptStruct_FInputNameMapping_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Data/InputNameMapping.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputText_MetaData[] = {
		{ "Category", "UINav Input" },
		{ "ModuleRelativePath", "Public/Data/InputNameMapping.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_InputText;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FInputNameMapping>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FInputNameMapping_Statics::NewProp_InputText = { "InputText", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputNameMapping, InputText), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputText_MetaData), NewProp_InputText_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FInputNameMapping_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputNameMapping_Statics::NewProp_InputText,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputNameMapping_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FInputNameMapping_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
	Z_Construct_UScriptStruct_FTableRowBase,
	&NewStructOps,
	"InputNameMapping",
	Z_Construct_UScriptStruct_FInputNameMapping_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputNameMapping_Statics::PropPointers),
	sizeof(FInputNameMapping),
	alignof(FInputNameMapping),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputNameMapping_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FInputNameMapping_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FInputNameMapping()
{
	if (!Z_Registration_Info_UScriptStruct_InputNameMapping.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_InputNameMapping.InnerSingleton, Z_Construct_UScriptStruct_FInputNameMapping_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_InputNameMapping.InnerSingleton;
}
// End ScriptStruct FInputNameMapping

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputNameMapping_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FInputNameMapping::StaticStruct, Z_Construct_UScriptStruct_FInputNameMapping_Statics::NewStructOps, TEXT("InputNameMapping"), &Z_Registration_Info_UScriptStruct_InputNameMapping, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FInputNameMapping), 101550610U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputNameMapping_h_45918224(TEXT("/Script/UINavigation"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputNameMapping_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputNameMapping_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
