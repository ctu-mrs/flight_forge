// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavWidgetComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavWidgetComponent() {}

// Begin Cross Module References
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavWidgetComponent();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavWidgetComponent_NoRegister();
UMG_API UClass* Z_Construct_UClass_UWidgetComponent();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavWidgetComponent
void UUINavWidgetComponent::StaticRegisterNativesUUINavWidgetComponent()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavWidgetComponent);
UClass* Z_Construct_UClass_UUINavWidgetComponent_NoRegister()
{
	return UUINavWidgetComponent::StaticClass();
}
struct Z_Construct_UClass_UUINavWidgetComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "UINav" },
		{ "HideCategories", "Object Activation Components|Activation Sockets Base Lighting LOD Mesh Mobility Trigger" },
		{ "IncludePath", "UINavWidgetComponent.h" },
		{ "ModuleRelativePath", "Public/UINavWidgetComponent.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavWidgetComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UUINavWidgetComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWidgetComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavWidgetComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavWidgetComponent_Statics::ClassParams = {
	&UUINavWidgetComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x00B010A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavWidgetComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavWidgetComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavWidgetComponent()
{
	if (!Z_Registration_Info_UClass_UUINavWidgetComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavWidgetComponent.OuterSingleton, Z_Construct_UClass_UUINavWidgetComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavWidgetComponent.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavWidgetComponent>()
{
	return UUINavWidgetComponent::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavWidgetComponent);
UUINavWidgetComponent::~UUINavWidgetComponent() {}
// End Class UUINavWidgetComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidgetComponent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavWidgetComponent, UUINavWidgetComponent::StaticClass, TEXT("UUINavWidgetComponent"), &Z_Registration_Info_UClass_UUINavWidgetComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavWidgetComponent), 366932222U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidgetComponent_h_683503045(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidgetComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidgetComponent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
