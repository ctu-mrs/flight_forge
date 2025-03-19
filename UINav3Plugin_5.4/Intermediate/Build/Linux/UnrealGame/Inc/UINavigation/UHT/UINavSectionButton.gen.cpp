// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavSectionButton.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavSectionButton() {}

// Begin Cross Module References
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavSectionButton();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavSectionButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavSectionButton
void UUINavSectionButton::StaticRegisterNativesUUINavSectionButton()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavSectionButton);
UClass* Z_Construct_UClass_UUINavSectionButton_NoRegister()
{
	return UUINavSectionButton::StaticClass();
}
struct Z_Construct_UClass_UUINavSectionButton_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UINavSectionButton.h" },
		{ "ModuleRelativePath", "Public/UINavSectionButton.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SectionButton_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavSectionButton" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavSectionButton.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SectionButton;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavSectionButton>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavSectionButton_Statics::NewProp_SectionButton = { "SectionButton", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSectionButton, SectionButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SectionButton_MetaData), NewProp_SectionButton_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavSectionButton_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSectionButton_Statics::NewProp_SectionButton,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSectionButton_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavSectionButton_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSectionButton_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavSectionButton_Statics::ClassParams = {
	&UUINavSectionButton::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UUINavSectionButton_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSectionButton_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSectionButton_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavSectionButton_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavSectionButton()
{
	if (!Z_Registration_Info_UClass_UUINavSectionButton.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavSectionButton.OuterSingleton, Z_Construct_UClass_UUINavSectionButton_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavSectionButton.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavSectionButton>()
{
	return UUINavSectionButton::StaticClass();
}
UUINavSectionButton::UUINavSectionButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavSectionButton);
UUINavSectionButton::~UUINavSectionButton() {}
// End Class UUINavSectionButton

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSectionButton_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavSectionButton, UUINavSectionButton::StaticClass, TEXT("UUINavSectionButton"), &Z_Registration_Info_UClass_UUINavSectionButton, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavSectionButton), 3338825519U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSectionButton_h_1277594041(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSectionButton_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSectionButton_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
