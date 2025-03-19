// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/InputType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeInputType() {}

// Begin Cross Module References
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputType();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum EInputType
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EInputType;
static UEnum* EInputType_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EInputType.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EInputType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_EInputType, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("EInputType"));
	}
	return Z_Registration_Info_UEnum_EInputType.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<EInputType>()
{
	return EInputType_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_EInputType_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Gamepad.DisplayName", "Gamepad" },
		{ "Gamepad.Name", "EInputType::Gamepad" },
		{ "Keyboard.DisplayName", "Keyboard" },
		{ "Keyboard.Name", "EInputType::Keyboard" },
		{ "ModuleRelativePath", "Public/Data/InputType.h" },
		{ "Mouse.DisplayName", "Mouse" },
		{ "Mouse.Name", "EInputType::Mouse" },
		{ "None.DisplayName", "None" },
		{ "None.Name", "EInputType::None" },
		{ "ScriptName", "UINavInputType" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EInputType::None", (int64)EInputType::None },
		{ "EInputType::Keyboard", (int64)EInputType::Keyboard },
		{ "EInputType::Mouse", (int64)EInputType::Mouse },
		{ "EInputType::Gamepad", (int64)EInputType::Gamepad },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_EInputType_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"EInputType",
	"EInputType",
	Z_Construct_UEnum_UINavigation_EInputType_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EInputType_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_EInputType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_EInputType_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_EInputType()
{
	if (!Z_Registration_Info_UEnum_EInputType.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EInputType.InnerSingleton, Z_Construct_UEnum_UINavigation_EInputType_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EInputType.InnerSingleton;
}
// End Enum EInputType

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputType_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EInputType_StaticEnum, TEXT("EInputType"), &Z_Registration_Info_UEnum_EInputType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 346009216U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputType_h_484262126(TEXT("/Script/UINavigation"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputType_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputType_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
