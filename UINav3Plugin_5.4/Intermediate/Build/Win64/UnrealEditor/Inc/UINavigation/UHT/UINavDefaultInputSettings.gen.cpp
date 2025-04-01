// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavDefaultInputSettings.h"
#include "UINavigation/Public/Data/InputMappingArray.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavDefaultInputSettings() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavDefaultInputSettings();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavDefaultInputSettings_NoRegister();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputMappingArray();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavDefaultInputSettings
void UUINavDefaultInputSettings::StaticRegisterNativesUUINavDefaultInputSettings()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavDefaultInputSettings);
UClass* Z_Construct_UClass_UUINavDefaultInputSettings_NoRegister()
{
	return UUINavDefaultInputSettings::StaticClass();
}
struct Z_Construct_UClass_UUINavDefaultInputSettings_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *\n */" },
#endif
		{ "IncludePath", "UINavDefaultInputSettings.h" },
		{ "ModuleRelativePath", "Public/UINavDefaultInputSettings.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultEnhancedInputMappings_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// A map for each Input Context in your game and its respective Default Input Context Mappings\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavDefaultInputSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "A map for each Input Context in your game and its respective Default Input Context Mappings" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputVersion_MetaData[] = {
		{ "ModuleRelativePath", "Public/UINavDefaultInputSettings.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_DefaultEnhancedInputMappings_ValueProp;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_DefaultEnhancedInputMappings_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_DefaultEnhancedInputMappings;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InputVersion;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavDefaultInputSettings>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UUINavDefaultInputSettings_Statics::NewProp_DefaultEnhancedInputMappings_ValueProp = { "DefaultEnhancedInputMappings", nullptr, (EPropertyFlags)0x0000000000004000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FInputMappingArray, METADATA_PARAMS(0, nullptr) }; // 1348253946
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UUINavDefaultInputSettings_Statics::NewProp_DefaultEnhancedInputMappings_Key_KeyProp = { "DefaultEnhancedInputMappings_Key", nullptr, (EPropertyFlags)0x0004000000004000, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UUINavDefaultInputSettings_Statics::NewProp_DefaultEnhancedInputMappings = { "DefaultEnhancedInputMappings", nullptr, (EPropertyFlags)0x0010000000004000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavDefaultInputSettings, DefaultEnhancedInputMappings), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultEnhancedInputMappings_MetaData), NewProp_DefaultEnhancedInputMappings_MetaData) }; // 1348253946
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavDefaultInputSettings_Statics::NewProp_InputVersion = { "InputVersion", nullptr, (EPropertyFlags)0x0010000000004000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavDefaultInputSettings, InputVersion), nullptr, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputVersion_MetaData), NewProp_InputVersion_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavDefaultInputSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavDefaultInputSettings_Statics::NewProp_DefaultEnhancedInputMappings_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavDefaultInputSettings_Statics::NewProp_DefaultEnhancedInputMappings_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavDefaultInputSettings_Statics::NewProp_DefaultEnhancedInputMappings,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavDefaultInputSettings_Statics::NewProp_InputVersion,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavDefaultInputSettings_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavDefaultInputSettings_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavDefaultInputSettings_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavDefaultInputSettings_Statics::ClassParams = {
	&UUINavDefaultInputSettings::StaticClass,
	"UserUINavDefaultInputSettings",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UUINavDefaultInputSettings_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavDefaultInputSettings_Statics::PropPointers),
	0,
	0x001000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavDefaultInputSettings_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavDefaultInputSettings_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavDefaultInputSettings()
{
	if (!Z_Registration_Info_UClass_UUINavDefaultInputSettings.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavDefaultInputSettings.OuterSingleton, Z_Construct_UClass_UUINavDefaultInputSettings_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavDefaultInputSettings.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavDefaultInputSettings>()
{
	return UUINavDefaultInputSettings::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavDefaultInputSettings);
UUINavDefaultInputSettings::~UUINavDefaultInputSettings() {}
// End Class UUINavDefaultInputSettings

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavDefaultInputSettings_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavDefaultInputSettings, UUINavDefaultInputSettings::StaticClass, TEXT("UUINavDefaultInputSettings"), &Z_Registration_Info_UClass_UUINavDefaultInputSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavDefaultInputSettings), 2433612658U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavDefaultInputSettings_h_1100393272(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavDefaultInputSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavDefaultInputSettings_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
