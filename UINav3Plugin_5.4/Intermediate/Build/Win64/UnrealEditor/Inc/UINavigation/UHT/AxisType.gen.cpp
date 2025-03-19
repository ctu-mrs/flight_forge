// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/AxisType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAxisType() {}

// Begin Cross Module References
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EAxisType();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum EAxisType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EAxisType;
static UEnum* EAxisType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EAxisType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EAxisType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_EAxisType, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("EAxisType"));
	}
	return Z_Registration_Info_UEnum_EAxisType.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<EAxisType>()
{
	return EAxisType_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_EAxisType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/AxisType.h" },
		{ "Negative.DisplayName", "Negative" },
		{ "Negative.Name", "EAxisType::Negative" },
		{ "None.DisplayName", "None" },
		{ "None.Name", "EAxisType::None" },
		{ "Positive.DisplayName", "Positive" },
		{ "Positive.Name", "EAxisType::Positive" },
		{ "ScriptName", "UINavAxisType" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EAxisType::None", (int64)EAxisType::None },
		{ "EAxisType::Positive", (int64)EAxisType::Positive },
		{ "EAxisType::Negative", (int64)EAxisType::Negative },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_EAxisType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"EAxisType",
	"EAxisType",
	Z_Construct_UEnum_UINavigation_EAxisType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EAxisType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EAxisType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_EAxisType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_EAxisType()
{
	if (!Z_Registration_Info_UEnum_EAxisType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EAxisType.InnerSingleton, Z_Construct_UEnum_UINavigation_EAxisType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EAxisType.InnerSingleton;
}
// End Enum EAxisType

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_AxisType_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EAxisType_StaticEnum, TEXT("EAxisType"), &Z_Registration_Info_UEnum_EAxisType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2012741077U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_AxisType_h_254857295(TEXT("/Script/UINavigation"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_AxisType_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_AxisType_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
