// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavSavedInputSettings.h"
#include "UINavigation/Public/Data/InputMappingArray.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavSavedInputSettings() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavSavedInputSettings();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavSavedInputSettings_NoRegister();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputMappingArray();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavSavedInputSettings
void UUINavSavedInputSettings::StaticRegisterNativesUUINavSavedInputSettings()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavSavedInputSettings);
UClass* Z_Construct_UClass_UUINavSavedInputSettings_NoRegister()
{
	return UUINavSavedInputSettings::StaticClass();
}
struct Z_Construct_UClass_UUINavSavedInputSettings_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *\n */" },
#endif
		{ "IncludePath", "UINavSavedInputSettings.h" },
		{ "ModuleRelativePath", "Public/UINavSavedInputSettings.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SavedEnhancedInputMappings_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// A map for each Input Context that's been overriden in your game and its respective Input Context Mappings\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavSavedInputSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "A map for each Input Context that's been overriden in your game and its respective Input Context Mappings" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputVersion_MetaData[] = {
		{ "ModuleRelativePath", "Public/UINavSavedInputSettings.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_SavedEnhancedInputMappings_ValueProp;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_SavedEnhancedInputMappings_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_SavedEnhancedInputMappings;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InputVersion;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavSavedInputSettings>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UUINavSavedInputSettings_Statics::NewProp_SavedEnhancedInputMappings_ValueProp = { "SavedEnhancedInputMappings", nullptr, (EPropertyFlags)0x0000000000004000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FInputMappingArray, METADATA_PARAMS(0, nullptr) }; // 1348253946
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UUINavSavedInputSettings_Statics::NewProp_SavedEnhancedInputMappings_Key_KeyProp = { "SavedEnhancedInputMappings_Key", nullptr, (EPropertyFlags)0x0004000000004000, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UUINavSavedInputSettings_Statics::NewProp_SavedEnhancedInputMappings = { "SavedEnhancedInputMappings", nullptr, (EPropertyFlags)0x0010000000004000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSavedInputSettings, SavedEnhancedInputMappings), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SavedEnhancedInputMappings_MetaData), NewProp_SavedEnhancedInputMappings_MetaData) }; // 1348253946
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavSavedInputSettings_Statics::NewProp_InputVersion = { "InputVersion", nullptr, (EPropertyFlags)0x0010000000004000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSavedInputSettings, InputVersion), nullptr, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputVersion_MetaData), NewProp_InputVersion_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavSavedInputSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSavedInputSettings_Statics::NewProp_SavedEnhancedInputMappings_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSavedInputSettings_Statics::NewProp_SavedEnhancedInputMappings_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSavedInputSettings_Statics::NewProp_SavedEnhancedInputMappings,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSavedInputSettings_Statics::NewProp_InputVersion,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSavedInputSettings_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavSavedInputSettings_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSavedInputSettings_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavSavedInputSettings_Statics::ClassParams = {
	&UUINavSavedInputSettings::StaticClass,
	"UserUINavSavedInputSettings",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UUINavSavedInputSettings_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSavedInputSettings_Statics::PropPointers),
	0,
	0x001000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSavedInputSettings_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavSavedInputSettings_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavSavedInputSettings()
{
	if (!Z_Registration_Info_UClass_UUINavSavedInputSettings.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavSavedInputSettings.OuterSingleton, Z_Construct_UClass_UUINavSavedInputSettings_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavSavedInputSettings.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavSavedInputSettings>()
{
	return UUINavSavedInputSettings::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavSavedInputSettings);
UUINavSavedInputSettings::~UUINavSavedInputSettings() {}
// End Class UUINavSavedInputSettings

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSavedInputSettings_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavSavedInputSettings, UUINavSavedInputSettings::StaticClass, TEXT("UUINavSavedInputSettings"), &Z_Registration_Info_UClass_UUINavSavedInputSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavSavedInputSettings), 1679244031U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSavedInputSettings_h_3646610058(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSavedInputSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSavedInputSettings_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
