// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/InputMappingArray.h"
#include "UINavigation/Public/Data/UINavEnhancedActionKeyMapping.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeInputMappingArray() {}

// Begin Cross Module References
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputMappingArray();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FUINavEnhancedActionKeyMapping();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin ScriptStruct FInputMappingArray
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_InputMappingArray;
class UScriptStruct* FInputMappingArray::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_InputMappingArray.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_InputMappingArray.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FInputMappingArray, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("InputMappingArray"));
	}
	return Z_Registration_Info_UScriptStruct_InputMappingArray.OuterSingleton;
}
template<> UINAVIGATION_API UScriptStruct* StaticStruct<FInputMappingArray>()
{
	return FInputMappingArray::StaticStruct();
}
struct Z_Construct_UScriptStruct_FInputMappingArray_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/InputMappingArray.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputMappings_MetaData[] = {
		{ "ModuleRelativePath", "Public/Data/InputMappingArray.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_InputMappings_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InputMappings;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FInputMappingArray>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FInputMappingArray_Statics::NewProp_InputMappings_Inner = { "InputMappings", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FUINavEnhancedActionKeyMapping, METADATA_PARAMS(0, nullptr) }; // 3682184329
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FInputMappingArray_Statics::NewProp_InputMappings = { "InputMappings", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputMappingArray, InputMappings), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputMappings_MetaData), NewProp_InputMappings_MetaData) }; // 3682184329
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FInputMappingArray_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputMappingArray_Statics::NewProp_InputMappings_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputMappingArray_Statics::NewProp_InputMappings,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputMappingArray_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FInputMappingArray_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	&NewStructOps,
	"InputMappingArray",
	Z_Construct_UScriptStruct_FInputMappingArray_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputMappingArray_Statics::PropPointers),
	sizeof(FInputMappingArray),
	alignof(FInputMappingArray),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputMappingArray_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FInputMappingArray_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FInputMappingArray()
{
	if (!Z_Registration_Info_UScriptStruct_InputMappingArray.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_InputMappingArray.InnerSingleton, Z_Construct_UScriptStruct_FInputMappingArray_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_InputMappingArray.InnerSingleton;
}
// End ScriptStruct FInputMappingArray

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputMappingArray_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FInputMappingArray::StaticStruct, Z_Construct_UScriptStruct_FInputMappingArray_Statics::NewStructOps, TEXT("InputMappingArray"), &Z_Registration_Info_UScriptStruct_InputMappingArray, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FInputMappingArray), 1348253946U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputMappingArray_h_2395454173(TEXT("/Script/UINavigation"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputMappingArray_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputMappingArray_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
