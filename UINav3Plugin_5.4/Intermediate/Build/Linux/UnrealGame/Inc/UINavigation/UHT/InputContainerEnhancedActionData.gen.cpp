// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/InputContainerEnhancedActionData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeInputContainerEnhancedActionData() {}

// Begin Cross Module References
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EAxisType();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputAxis();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputContainerEnhancedActionData();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum EInputAxis
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EInputAxis;
static UEnum* EInputAxis_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EInputAxis.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EInputAxis.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_EInputAxis, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("EInputAxis"));
	}
	return Z_Registration_Info_UEnum_EInputAxis.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<EInputAxis>()
{
	return EInputAxis_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_EInputAxis_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/InputContainerEnhancedActionData.h" },
		{ "ScriptName", "UINavAxis" },
		{ "X.DisplayName", "X" },
		{ "X.Name", "EInputAxis::X" },
		{ "Y.DisplayName", "Y" },
		{ "Y.Name", "EInputAxis::Y" },
		{ "Z.DisplayName", "Z" },
		{ "Z.Name", "EInputAxis::Z" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EInputAxis::X", (int64)EInputAxis::X },
		{ "EInputAxis::Y", (int64)EInputAxis::Y },
		{ "EInputAxis::Z", (int64)EInputAxis::Z },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_EInputAxis_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"EInputAxis",
	"EInputAxis",
	Z_Construct_UEnum_UINavigation_EInputAxis_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EInputAxis_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EInputAxis_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_EInputAxis_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_EInputAxis()
{
	if (!Z_Registration_Info_UEnum_EInputAxis.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EInputAxis.InnerSingleton, Z_Construct_UEnum_UINavigation_EInputAxis_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EInputAxis.InnerSingleton;
}
// End Enum EInputAxis

// Begin ScriptStruct FInputContainerEnhancedActionData
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionData;
class UScriptStruct* FInputContainerEnhancedActionData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FInputContainerEnhancedActionData, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("InputContainerEnhancedActionData"));
	}
	return Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionData.OuterSingleton;
}
template<> UINAVIGATION_API UScriptStruct* StaticStruct<FInputContainerEnhancedActionData>()
{
	return FInputContainerEnhancedActionData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/InputContainerEnhancedActionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisplayName_MetaData[] = {
		{ "Category", "EnhancedInput" },
		{ "ModuleRelativePath", "Public/Data/InputContainerEnhancedActionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Action_MetaData[] = {
		{ "Category", "EnhancedInput" },
		{ "ModuleRelativePath", "Public/Data/InputContainerEnhancedActionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Axis_MetaData[] = {
		{ "Category", "EnhancedInput" },
		{ "ModuleRelativePath", "Public/Data/InputContainerEnhancedActionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AxisScale_MetaData[] = {
		{ "Category", "EnhancedInput" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Specifies whether you want this input to be treated as a normal axis or as a positive / negative axis.\n\x09(Only applies to Input Actions with a ValueType different than Boolean).\n\x09If you set it to a value different than none, you should always have a 2nd box with the opposite value.\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/Data/InputContainerEnhancedActionData.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Specifies whether you want this input to be treated as a normal axis or as a positive / negative axis.\n(Only applies to Input Actions with a ValueType different than Boolean).\nIf you set it to a value different than none, you should always have a 2nd box with the opposite value." },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_DisplayName;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Action;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Axis_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Axis;
	static const UECodeGen_Private::FBytePropertyParams NewProp_AxisScale_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_AxisScale;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FInputContainerEnhancedActionData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_DisplayName = { "DisplayName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputContainerEnhancedActionData, DisplayName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisplayName_MetaData), NewProp_DisplayName_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_Action = { "Action", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputContainerEnhancedActionData, Action), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Action_MetaData), NewProp_Action_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_Axis_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_Axis = { "Axis", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputContainerEnhancedActionData, Axis), Z_Construct_UEnum_UINavigation_EInputAxis, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Axis_MetaData), NewProp_Axis_MetaData) }; // 2063529479
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_AxisScale_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_AxisScale = { "AxisScale", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputContainerEnhancedActionData, AxisScale), Z_Construct_UEnum_UINavigation_EAxisType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AxisScale_MetaData), NewProp_AxisScale_MetaData) }; // 2012741077
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_DisplayName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_Action,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_Axis_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_Axis,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_AxisScale_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewProp_AxisScale,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	&NewStructOps,
	"InputContainerEnhancedActionData",
	Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::PropPointers),
	sizeof(FInputContainerEnhancedActionData),
	alignof(FInputContainerEnhancedActionData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FInputContainerEnhancedActionData()
{
	if (!Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionData.InnerSingleton, Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionData.InnerSingleton;
}
// End ScriptStruct FInputContainerEnhancedActionData

// Begin ScriptStruct FInputContainerEnhancedActionDataArray
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionDataArray;
class UScriptStruct* FInputContainerEnhancedActionDataArray::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionDataArray.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionDataArray.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("InputContainerEnhancedActionDataArray"));
	}
	return Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionDataArray.OuterSingleton;
}
template<> UINAVIGATION_API UScriptStruct* StaticStruct<FInputContainerEnhancedActionDataArray>()
{
	return FInputContainerEnhancedActionDataArray::StaticStruct();
}
struct Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/InputContainerEnhancedActionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputGroups_MetaData[] = {
		{ "Category", "InputRebindData" },
		{ "ModuleRelativePath", "Public/Data/InputContainerEnhancedActionData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Actions_MetaData[] = {
		{ "Category", "EnhancedInput" },
		{ "ModuleRelativePath", "Public/Data/InputContainerEnhancedActionData.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_InputGroups_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InputGroups;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Actions_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Actions;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FInputContainerEnhancedActionDataArray>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::NewProp_InputGroups_Inner = { "InputGroups", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::NewProp_InputGroups = { "InputGroups", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputContainerEnhancedActionDataArray, InputGroups), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputGroups_MetaData), NewProp_InputGroups_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::NewProp_Actions_Inner = { "Actions", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FInputContainerEnhancedActionData, METADATA_PARAMS(0, nullptr) }; // 2773913468
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::NewProp_Actions = { "Actions", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FInputContainerEnhancedActionDataArray, Actions), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Actions_MetaData), NewProp_Actions_MetaData) }; // 2773913468
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::NewProp_InputGroups_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::NewProp_InputGroups,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::NewProp_Actions_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::NewProp_Actions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	&NewStructOps,
	"InputContainerEnhancedActionDataArray",
	Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::PropPointers),
	sizeof(FInputContainerEnhancedActionDataArray),
	alignof(FInputContainerEnhancedActionDataArray),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray()
{
	if (!Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionDataArray.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionDataArray.InnerSingleton, Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionDataArray.InnerSingleton;
}
// End ScriptStruct FInputContainerEnhancedActionDataArray

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputContainerEnhancedActionData_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EInputAxis_StaticEnum, TEXT("EInputAxis"), &Z_Registration_Info_UEnum_EInputAxis, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2063529479U) },
	};
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FInputContainerEnhancedActionData::StaticStruct, Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics::NewStructOps, TEXT("InputContainerEnhancedActionData"), &Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FInputContainerEnhancedActionData), 2773913468U) },
		{ FInputContainerEnhancedActionDataArray::StaticStruct, Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics::NewStructOps, TEXT("InputContainerEnhancedActionDataArray"), &Z_Registration_Info_UScriptStruct_InputContainerEnhancedActionDataArray, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FInputContainerEnhancedActionDataArray), 3240473501U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputContainerEnhancedActionData_h_1903988093(TEXT("/Script/UINavigation"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputContainerEnhancedActionData_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputContainerEnhancedActionData_h_Statics::ScriptStructInfo),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputContainerEnhancedActionData_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputContainerEnhancedActionData_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
