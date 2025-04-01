// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavSectionsWidget.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavSectionsWidget() {}

// Begin Cross Module References
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavSectionsWidget();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavSectionsWidget_NoRegister();
UMG_API UClass* Z_Construct_UClass_UPanelWidget_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavSectionsWidget
void UUINavSectionsWidget::StaticRegisterNativesUUINavSectionsWidget()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavSectionsWidget);
UClass* Z_Construct_UClass_UUINavSectionsWidget_NoRegister()
{
	return UUINavSectionsWidget::StaticClass();
}
struct Z_Construct_UClass_UUINavSectionsWidget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UINavSectionsWidget.h" },
		{ "ModuleRelativePath", "Public/UINavSectionsWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SectionButtonsPanel_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavSectionsWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavSectionsWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SectionButtonsPanel;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavSectionsWidget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavSectionsWidget_Statics::NewProp_SectionButtonsPanel = { "SectionButtonsPanel", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSectionsWidget, SectionButtonsPanel), Z_Construct_UClass_UPanelWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SectionButtonsPanel_MetaData), NewProp_SectionButtonsPanel_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavSectionsWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSectionsWidget_Statics::NewProp_SectionButtonsPanel,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSectionsWidget_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavSectionsWidget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSectionsWidget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavSectionsWidget_Statics::ClassParams = {
	&UUINavSectionsWidget::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UUINavSectionsWidget_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSectionsWidget_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSectionsWidget_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavSectionsWidget_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavSectionsWidget()
{
	if (!Z_Registration_Info_UClass_UUINavSectionsWidget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavSectionsWidget.OuterSingleton, Z_Construct_UClass_UUINavSectionsWidget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavSectionsWidget.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavSectionsWidget>()
{
	return UUINavSectionsWidget::StaticClass();
}
UUINavSectionsWidget::UUINavSectionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavSectionsWidget);
UUINavSectionsWidget::~UUINavSectionsWidget() {}
// End Class UUINavSectionsWidget

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSectionsWidget_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavSectionsWidget, UUINavSectionsWidget::StaticClass, TEXT("UUINavSectionsWidget"), &Z_Registration_Info_UClass_UUINavSectionsWidget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavSectionsWidget), 614135426U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSectionsWidget_h_2069829180(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSectionsWidget_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSectionsWidget_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
