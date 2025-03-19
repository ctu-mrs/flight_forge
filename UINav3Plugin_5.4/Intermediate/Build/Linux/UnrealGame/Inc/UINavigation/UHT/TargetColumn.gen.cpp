// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/TargetColumn.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTargetColumn() {}

// Begin Cross Module References
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_ETargetColumn();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Enum ETargetColumn
static FEnumRegistrationInfo Z_Registration_Info_UEnum_ETargetColumn;
static UEnum* ETargetColumn_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_ETargetColumn.OuterSingleton)
	{
		Z_Registration_Info_UEnum_ETargetColumn.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_UINavigation_ETargetColumn, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("ETargetColumn"));
	}
	return Z_Registration_Info_UEnum_ETargetColumn.OuterSingleton;
}
template<> UINAVIGATION_API UEnum* StaticEnum<ETargetColumn>()
{
	return ETargetColumn_StaticEnum();
}
struct Z_Construct_UEnum_UINavigation_ETargetColumn_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Left.DisplayName", "Left" },
		{ "Left.Name", "ETargetColumn::Left" },
		{ "Middle.DisplayName", "Middle" },
		{ "Middle.Name", "ETargetColumn::Middle" },
		{ "ModuleRelativePath", "Public/Data/TargetColumn.h" },
		{ "Right.DisplayName", "Right" },
		{ "Right.Name", "ETargetColumn::Right" },
		{ "ScriptName", "UINavTargetColumn" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "ETargetColumn::Left", (int64)ETargetColumn::Left },
		{ "ETargetColumn::Middle", (int64)ETargetColumn::Middle },
		{ "ETargetColumn::Right", (int64)ETargetColumn::Right },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_UINavigation_ETargetColumn_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	"ETargetColumn",
	"ETargetColumn",
	Z_Construct_UEnum_UINavigation_ETargetColumn_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_ETargetColumn_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_UINavigation_ETargetColumn_Statics::Enum_MetaDataParams), Z_Construct_UEnum_UINavigation_ETargetColumn_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_UINavigation_ETargetColumn()
{
	if (!Z_Registration_Info_UEnum_ETargetColumn.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ETargetColumn.InnerSingleton, Z_Construct_UEnum_UINavigation_ETargetColumn_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_ETargetColumn.InnerSingleton;
}
// End Enum ETargetColumn

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_TargetColumn_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ ETargetColumn_StaticEnum, TEXT("ETargetColumn"), &Z_Registration_Info_UEnum_ETargetColumn, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4286471576U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_TargetColumn_h_3259457307(TEXT("/Script/UINavigation"),
	nullptr, 0,
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_TargetColumn_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_TargetColumn_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
