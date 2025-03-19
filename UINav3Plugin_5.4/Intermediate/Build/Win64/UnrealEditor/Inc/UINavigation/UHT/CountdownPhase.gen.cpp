// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/CountdownPhase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCountdownPhase() {}

// Begin Cross Module References
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_ECountdownPhase();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum ECountdownPhase
static FEnumRegistrationInfo Z_Registration_Info_UEnum_ECountdownPhase;
static UEnum* ECountdownPhase_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_ECountdownPhase.OuterSingleton)
	{
		Z_Registration_Info_UEnum_ECountdownPhase.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_ECountdownPhase, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("ECountdownPhase"));
	}
	return Z_Registration_Info_UEnum_ECountdownPhase.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<ECountdownPhase>()
{
	return ECountdownPhase_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_ECountdownPhase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "First.DisplayName", "First" },
		{ "First.Name", "ECountdownPhase::First" },
		{ "Looping.DisplayName", "Looping" },
		{ "Looping.Name", "ECountdownPhase::Looping" },
		{ "ModuleRelativePath", "Public/Data/CountdownPhase.h" },
		{ "None.DisplayName", "None" },
		{ "None.Name", "ECountdownPhase::None" },
		{ "ScriptName", "UINavCountdownPhase" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "ECountdownPhase::None", (int64)ECountdownPhase::None },
		{ "ECountdownPhase::First", (int64)ECountdownPhase::First },
		{ "ECountdownPhase::Looping", (int64)ECountdownPhase::Looping },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_ECountdownPhase_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"ECountdownPhase",
	"ECountdownPhase",
	Z_Construct_UEnum_UINavigation_ECountdownPhase_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_ECountdownPhase_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_ECountdownPhase_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_ECountdownPhase_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_ECountdownPhase()
{
	if (!Z_Registration_Info_UEnum_ECountdownPhase.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ECountdownPhase.InnerSingleton, Z_Construct_UEnum_UINavigation_ECountdownPhase_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_ECountdownPhase.InnerSingleton;
}
// End Enum ECountdownPhase

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_CountdownPhase_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ ECountdownPhase_StaticEnum, TEXT("ECountdownPhase"), &Z_Registration_Info_UEnum_ECountdownPhase, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4067549126U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_CountdownPhase_h_3539317863(TEXT("/Script/UINavigation"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_CountdownPhase_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_CountdownPhase_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
