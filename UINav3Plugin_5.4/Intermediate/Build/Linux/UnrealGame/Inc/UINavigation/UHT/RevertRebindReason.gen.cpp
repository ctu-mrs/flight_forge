// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/RevertRebindReason.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRevertRebindReason() {}

// Begin Cross Module References
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_ERevertRebindReason();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum ERevertRebindReason
static FEnumRegistrationInfo Z_Registration_Info_UEnum_ERevertRebindReason;
static UEnum* ERevertRebindReason_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_ERevertRebindReason.OuterSingleton)
	{
		Z_Registration_Info_UEnum_ERevertRebindReason.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_ERevertRebindReason, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("ERevertRebindReason"));
	}
	return Z_Registration_Info_UEnum_ERevertRebindReason.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<ERevertRebindReason>()
{
	return ERevertRebindReason_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_ERevertRebindReason_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlacklistedKey.DisplayName", "Blacklisted Key" },
		{ "BlacklistedKey.Name", "ERevertRebindReason::BlacklistedKey" },
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/RevertRebindReason.h" },
		{ "None.DisplayName", "None" },
		{ "None.Name", "ERevertRebindReason::None" },
		{ "NonWhitelistedKey.DisplayName", "Non-Whitelisted Key" },
		{ "NonWhitelistedKey.Name", "ERevertRebindReason::NonWhitelistedKey" },
		{ "RestrictionMismatch.DisplayName", "Restriction Mismatch" },
		{ "RestrictionMismatch.Name", "ERevertRebindReason::RestrictionMismatch" },
		{ "ScriptName", "UINavRevertRebindReason" },
		{ "SwapRejected.DisplayName", "Swap Rejected" },
		{ "SwapRejected.Name", "ERevertRebindReason::SwapRejected" },
		{ "UsedBySameInput.DisplayName", "Used By Same Input" },
		{ "UsedBySameInput.Name", "ERevertRebindReason::UsedBySameInput" },
		{ "UsedBySameInputGroup.DisplayName", "Used By Same Input Group" },
		{ "UsedBySameInputGroup.Name", "ERevertRebindReason::UsedBySameInputGroup" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "ERevertRebindReason::None", (int64)ERevertRebindReason::None },
		{ "ERevertRebindReason::RestrictionMismatch", (int64)ERevertRebindReason::RestrictionMismatch },
		{ "ERevertRebindReason::NonWhitelistedKey", (int64)ERevertRebindReason::NonWhitelistedKey },
		{ "ERevertRebindReason::BlacklistedKey", (int64)ERevertRebindReason::BlacklistedKey },
		{ "ERevertRebindReason::UsedBySameInputGroup", (int64)ERevertRebindReason::UsedBySameInputGroup },
		{ "ERevertRebindReason::UsedBySameInput", (int64)ERevertRebindReason::UsedBySameInput },
		{ "ERevertRebindReason::SwapRejected", (int64)ERevertRebindReason::SwapRejected },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_ERevertRebindReason_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"ERevertRebindReason",
	"ERevertRebindReason",
	Z_Construct_UEnum_UINavigation_ERevertRebindReason_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_ERevertRebindReason_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_ERevertRebindReason_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_ERevertRebindReason_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_ERevertRebindReason()
{
	if (!Z_Registration_Info_UEnum_ERevertRebindReason.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ERevertRebindReason.InnerSingleton, Z_Construct_UEnum_UINavigation_ERevertRebindReason_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_ERevertRebindReason.InnerSingleton;
}
// End Enum ERevertRebindReason

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_RevertRebindReason_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ ERevertRebindReason_StaticEnum, TEXT("ERevertRebindReason"), &Z_Registration_Info_UEnum_ERevertRebindReason, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3554923296U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_RevertRebindReason_h_1437864815(TEXT("/Script/UINavigation"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_RevertRebindReason_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_RevertRebindReason_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
