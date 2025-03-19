// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/InputIconMapping.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeInputIconMapping() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UTexture2D_NoRegister();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTableRowBase();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputIconMapping();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin ScriptStruct FInputIconMapping
static_assert(std::is_polymorphic<FInputIconMapping>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FInputIconMapping cannot be polymorphic unless super FTableRowBase is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_InputIconMapping;
class UScriptStruct* FInputIconMapping::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_InputIconMapping.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_InputIconMapping.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FInputIconMapping, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("InputIconMapping"));
	}
	return Z_Registration_Info_UScriptStruct_InputIconMapping.OuterSingleton;
}
template<> UINAVIGATION_API UScriptStruct* StaticStruct<FInputIconMapping>()
{
	return FInputIconMapping::StaticStruct();
}
struct Z_Construct_UScriptStruct_FInputIconMapping_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Data/InputIconMapping.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputIcon_MetaData[] = {
		{ "Category", "UINav Input" },
		{ "ModuleRelativePath", "Public/Data/InputIconMapping.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_InputIcon;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FInputIconMapping>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FInputIconMapping_Statics::NewProp_InputIcon = { "InputIcon", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputIconMapping, InputIcon), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputIcon_MetaData), NewProp_InputIcon_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FInputIconMapping_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputIconMapping_Statics::NewProp_InputIcon,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputIconMapping_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FInputIconMapping_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
	Z_Construct_UScriptStruct_FTableRowBase,
	&NewStructOps,
	"InputIconMapping",
	Z_Construct_UScriptStruct_FInputIconMapping_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputIconMapping_Statics::PropPointers),
	sizeof(FInputIconMapping),
	alignof(FInputIconMapping),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputIconMapping_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FInputIconMapping_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FInputIconMapping()
{
	if (!Z_Registration_Info_UScriptStruct_InputIconMapping.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_InputIconMapping.InnerSingleton, Z_Construct_UScriptStruct_FInputIconMapping_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_InputIconMapping.InnerSingleton;
}
// End ScriptStruct FInputIconMapping

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputIconMapping_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FInputIconMapping::StaticStruct, Z_Construct_UScriptStruct_FInputIconMapping_Statics::NewStructOps, TEXT("InputIconMapping"), &Z_Registration_Info_UScriptStruct_InputIconMapping, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FInputIconMapping), 1614494227U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputIconMapping_h_3876919931(TEXT("/Script/UINavigation"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputIconMapping_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputIconMapping_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
