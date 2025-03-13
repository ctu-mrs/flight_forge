// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/ReceiveInputType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeReceiveInputType() {}

// Begin Cross Module References
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EReceiveInputType();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum EReceiveInputType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EReceiveInputType;
static UEnum* EReceiveInputType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EReceiveInputType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EReceiveInputType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_EReceiveInputType, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("EReceiveInputType"));
	}
	return Z_Registration_Info_UEnum_EReceiveInputType.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<EReceiveInputType>()
{
	return EReceiveInputType_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_EReceiveInputType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Action.DisplayName", "Action" },
		{ "Action.Name", "EReceiveInputType::Action" },
		{ "Axis.DisplayName", "Axis" },
		{ "Axis.Name", "EReceiveInputType::Axis" },
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/ReceiveInputType.h" },
		{ "None.DisplayName", "None" },
		{ "None.Name", "EReceiveInputType::None" },
		{ "ScriptName", "UINavReceiveInputType" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EReceiveInputType::None", (int64)EReceiveInputType::None },
		{ "EReceiveInputType::Action", (int64)EReceiveInputType::Action },
		{ "EReceiveInputType::Axis", (int64)EReceiveInputType::Axis },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_EReceiveInputType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"EReceiveInputType",
	"EReceiveInputType",
	Z_Construct_UEnum_UINavigation_EReceiveInputType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EReceiveInputType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EReceiveInputType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_EReceiveInputType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_EReceiveInputType()
{
	if (!Z_Registration_Info_UEnum_EReceiveInputType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EReceiveInputType.InnerSingleton, Z_Construct_UEnum_UINavigation_EReceiveInputType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EReceiveInputType.InnerSingleton;
}
// End Enum EReceiveInputType

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_ReceiveInputType_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EReceiveInputType_StaticEnum, TEXT("EReceiveInputType"), &Z_Registration_Info_UEnum_EReceiveInputType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 798500070U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_ReceiveInputType_h_1041090147(TEXT("/Script/UINavigation"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_ReceiveInputType_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_ReceiveInputType_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
