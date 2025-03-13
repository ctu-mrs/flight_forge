// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/NavigationEvent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNavigationEvent() {}

// Begin Cross Module References
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_ENavigationEvent();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum ENavigationEvent
static FEnumRegistrationInfo Z_Registration_Info_UEnum_ENavigationEvent;
static UEnum* ENavigationEvent_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_ENavigationEvent.OuterSingleton)
	{
		Z_Registration_Info_UEnum_ENavigationEvent.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_ENavigationEvent, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("ENavigationEvent"));
	}
	return Z_Registration_Info_UEnum_ENavigationEvent.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<ENavigationEvent>()
{
	return ENavigationEvent_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_ENavigationEvent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Data/NavigationEvent.h" },
		{ "OnNavigate.DisplayName", "OnNavigate" },
		{ "OnNavigate.Name", "ENavigationEvent::OnNavigate" },
		{ "OnSelect.DisplayName", "OnSelect" },
		{ "OnSelect.Name", "ENavigationEvent::OnSelect" },
		{ "ScriptName", "UINavNavigationEvent" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "ENavigationEvent::OnSelect", (int64)ENavigationEvent::OnSelect },
		{ "ENavigationEvent::OnNavigate", (int64)ENavigationEvent::OnNavigate },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_ENavigationEvent_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"ENavigationEvent",
	"ENavigationEvent",
	Z_Construct_UEnum_UINavigation_ENavigationEvent_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_ENavigationEvent_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_ENavigationEvent_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_ENavigationEvent_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_ENavigationEvent()
{
	if (!Z_Registration_Info_UEnum_ENavigationEvent.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ENavigationEvent.InnerSingleton, Z_Construct_UEnum_UINavigation_ENavigationEvent_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_ENavigationEvent.InnerSingleton;
}
// End Enum ENavigationEvent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_NavigationEvent_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ ENavigationEvent_StaticEnum, TEXT("ENavigationEvent"), &Z_Registration_Info_UEnum_ENavigationEvent, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3114461984U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_NavigationEvent_h_133693104(TEXT("/Script/UINavigation"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_NavigationEvent_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_NavigationEvent_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
