// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/InputMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeInputMode() {}

// Begin Cross Module References
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputMode();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum EInputMode
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EInputMode;
static UEnum* EInputMode_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EInputMode.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EInputMode.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_EInputMode, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("EInputMode"));
	}
	return Z_Registration_Info_UEnum_EInputMode.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<EInputMode>()
{
	return EInputMode_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_EInputMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Game.DisplayName", "Game Only" },
		{ "Game.Name", "EInputMode::Game" },
		{ "GameUI.DisplayName", "Game and UI" },
		{ "GameUI.Name", "EInputMode::GameUI" },
		{ "ModuleRelativePath", "Public/Data/InputMode.h" },
		{ "None.DisplayName", "None" },
		{ "None.Name", "EInputMode::None" },
		{ "ScriptName", "UINavInputMode" },
		{ "UI.DisplayName", "UI Only" },
		{ "UI.Name", "EInputMode::UI" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EInputMode::None", (int64)EInputMode::None },
		{ "EInputMode::GameUI", (int64)EInputMode::GameUI },
		{ "EInputMode::Game", (int64)EInputMode::Game },
		{ "EInputMode::UI", (int64)EInputMode::UI },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_EInputMode_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"EInputMode",
	"EInputMode",
	Z_Construct_UEnum_UINavigation_EInputMode_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EInputMode_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EInputMode_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_EInputMode_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_EInputMode()
{
	if (!Z_Registration_Info_UEnum_EInputMode.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EInputMode.InnerSingleton, Z_Construct_UEnum_UINavigation_EInputMode_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EInputMode.InnerSingleton;
}
// End Enum EInputMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputMode_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EInputMode_StaticEnum, TEXT("EInputMode"), &Z_Registration_Info_UEnum_EInputMode, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2006005193U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputMode_h_695319403(TEXT("/Script/UINavigation"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputMode_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputMode_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
