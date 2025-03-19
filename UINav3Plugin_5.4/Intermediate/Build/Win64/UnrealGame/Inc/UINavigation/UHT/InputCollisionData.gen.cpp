// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/InputCollisionData.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeInputCollisionData() {}

// Begin Cross Module References
INPUTCORE_API UScriptStruct* Z_Construct_UScriptStruct_FKey();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputCollisionData();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin ScriptStruct FInputCollisionData
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_InputCollisionData;
class UScriptStruct* FInputCollisionData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_InputCollisionData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_InputCollisionData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FInputCollisionData, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("InputCollisionData"));
	}
	return Z_Registration_Info_UScriptStruct_InputCollisionData.OuterSingleton;
}
template<> UINAVIGATION_API UScriptStruct* StaticStruct<FInputCollisionData>()
{
	return FInputCollisionData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FInputCollisionData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/InputCollisionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentInputText_MetaData[] = {
		{ "Category", "InputCollisionData" },
		{ "ModuleRelativePath", "Public/Data/InputCollisionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CollidingInputText_MetaData[] = {
		{ "Category", "InputCollisionData" },
		{ "ModuleRelativePath", "Public/Data/InputCollisionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CollidingKeyIndex_MetaData[] = {
		{ "Category", "InputCollisionData" },
		{ "ModuleRelativePath", "Public/Data/InputCollisionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentInputKey_MetaData[] = {
		{ "Category", "InputCollisionData" },
		{ "ModuleRelativePath", "Public/Data/InputCollisionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PressedKey_MetaData[] = {
		{ "Category", "InputCollisionData" },
		{ "ModuleRelativePath", "Public/Data/InputCollisionData.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_CurrentInputText;
	static const UECodeGen_Private::FTextPropertyParams NewProp_CollidingInputText;
	static const UECodeGen_Private::FIntPropertyParams NewProp_CollidingKeyIndex;
	static const UECodeGen_Private::FStructPropertyParams NewProp_CurrentInputKey;
	static const UECodeGen_Private::FStructPropertyParams NewProp_PressedKey;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FInputCollisionData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewProp_CurrentInputText = { "CurrentInputText", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputCollisionData, CurrentInputText), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentInputText_MetaData), NewProp_CurrentInputText_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewProp_CollidingInputText = { "CollidingInputText", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputCollisionData, CollidingInputText), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CollidingInputText_MetaData), NewProp_CollidingInputText_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewProp_CollidingKeyIndex = { "CollidingKeyIndex", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputCollisionData, CollidingKeyIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CollidingKeyIndex_MetaData), NewProp_CollidingKeyIndex_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewProp_CurrentInputKey = { "CurrentInputKey", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputCollisionData, CurrentInputKey), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentInputKey_MetaData), NewProp_CurrentInputKey_MetaData) }; // 658672854
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewProp_PressedKey = { "PressedKey", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputCollisionData, PressedKey), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PressedKey_MetaData), NewProp_PressedKey_MetaData) }; // 658672854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FInputCollisionData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewProp_CurrentInputText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewProp_CollidingInputText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewProp_CollidingKeyIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewProp_CurrentInputKey,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewProp_PressedKey,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputCollisionData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FInputCollisionData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	&NewStructOps,
	"InputCollisionData",
	Z_Construct_UScriptStruct_FInputCollisionData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputCollisionData_Statics::PropPointers),
	sizeof(FInputCollisionData),
	alignof(FInputCollisionData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputCollisionData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FInputCollisionData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FInputCollisionData()
{
	if (!Z_Registration_Info_UScriptStruct_InputCollisionData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_InputCollisionData.InnerSingleton, Z_Construct_UScriptStruct_FInputCollisionData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_InputCollisionData.InnerSingleton;
}
// End ScriptStruct FInputCollisionData

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputCollisionData_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FInputCollisionData::StaticStruct, Z_Construct_UScriptStruct_FInputCollisionData_Statics::NewStructOps, TEXT("InputCollisionData"), &Z_Registration_Info_UScriptStruct_InputCollisionData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FInputCollisionData), 3537832298U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputCollisionData_h_1887013560(TEXT("/Script/UINavigation"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputCollisionData_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputCollisionData_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
