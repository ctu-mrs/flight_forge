// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/ThumbstickAsMouse.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeThumbstickAsMouse() {}

// Begin Cross Module References
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EThumbstickAsMouse();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum EThumbstickAsMouse
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EThumbstickAsMouse;
static UEnum* EThumbstickAsMouse_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EThumbstickAsMouse.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EThumbstickAsMouse.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_EThumbstickAsMouse, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("EThumbstickAsMouse"));
	}
	return Z_Registration_Info_UEnum_EThumbstickAsMouse.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<EThumbstickAsMouse>()
{
	return EThumbstickAsMouse_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_EThumbstickAsMouse_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "LeftThumbstick.Name", "EThumbstickAsMouse::LeftThumbstick" },
		{ "ModuleRelativePath", "Public/Data/ThumbstickAsMouse.h" },
		{ "None.Name", "EThumbstickAsMouse::None" },
		{ "RightThumbstick.Name", "EThumbstickAsMouse::RightThumbstick" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EThumbstickAsMouse::None", (int64)EThumbstickAsMouse::None },
		{ "EThumbstickAsMouse::LeftThumbstick", (int64)EThumbstickAsMouse::LeftThumbstick },
		{ "EThumbstickAsMouse::RightThumbstick", (int64)EThumbstickAsMouse::RightThumbstick },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_EThumbstickAsMouse_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"EThumbstickAsMouse",
	"EThumbstickAsMouse",
	Z_Construct_UEnum_UINavigation_EThumbstickAsMouse_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EThumbstickAsMouse_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EThumbstickAsMouse_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_EThumbstickAsMouse_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_EThumbstickAsMouse()
{
	if (!Z_Registration_Info_UEnum_EThumbstickAsMouse.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EThumbstickAsMouse.InnerSingleton, Z_Construct_UEnum_UINavigation_EThumbstickAsMouse_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EThumbstickAsMouse.InnerSingleton;
}
// End Enum EThumbstickAsMouse

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_ThumbstickAsMouse_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EThumbstickAsMouse_StaticEnum, TEXT("EThumbstickAsMouse"), &Z_Registration_Info_UEnum_EThumbstickAsMouse, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 819177189U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_ThumbstickAsMouse_h_2863184810(TEXT("/Script/UINavigation"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_ThumbstickAsMouse_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_ThumbstickAsMouse_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
