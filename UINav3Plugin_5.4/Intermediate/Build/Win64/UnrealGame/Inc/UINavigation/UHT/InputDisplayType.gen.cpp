// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/InputDisplayType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeInputDisplayType() {}

// Begin Cross Module References
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputDisplayType();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum EInputDisplayType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EInputDisplayType;
static UEnum* EInputDisplayType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EInputDisplayType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EInputDisplayType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_EInputDisplayType, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("EInputDisplayType"));
	}
	return Z_Registration_Info_UEnum_EInputDisplayType.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<EInputDisplayType>()
{
	return EInputDisplayType_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_EInputDisplayType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Both.Name", "EInputDisplayType::Both" },
		{ "Icon.Name", "EInputDisplayType::Icon" },
		{ "ModuleRelativePath", "Public/Data/InputDisplayType.h" },
		{ "Text.Name", "EInputDisplayType::Text" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EInputDisplayType::Icon", (int64)EInputDisplayType::Icon },
		{ "EInputDisplayType::Text", (int64)EInputDisplayType::Text },
		{ "EInputDisplayType::Both", (int64)EInputDisplayType::Both },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_EInputDisplayType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"EInputDisplayType",
	"EInputDisplayType",
	Z_Construct_UEnum_UINavigation_EInputDisplayType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EInputDisplayType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EInputDisplayType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_EInputDisplayType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_EInputDisplayType()
{
	if (!Z_Registration_Info_UEnum_EInputDisplayType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EInputDisplayType.InnerSingleton, Z_Construct_UEnum_UINavigation_EInputDisplayType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EInputDisplayType.InnerSingleton;
}
// End Enum EInputDisplayType

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputDisplayType_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EInputDisplayType_StaticEnum, TEXT("EInputDisplayType"), &Z_Registration_Info_UEnum_EInputDisplayType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 61414587U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputDisplayType_h_2321579416(TEXT("/Script/UINavigation"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputDisplayType_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputDisplayType_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
