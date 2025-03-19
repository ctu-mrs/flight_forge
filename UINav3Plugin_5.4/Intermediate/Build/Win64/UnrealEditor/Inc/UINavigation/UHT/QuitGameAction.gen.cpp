// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/ComponentActions/QuitGameAction.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeQuitGameAction() {}

// Begin Cross Module References
UINAVIGATION_API UClass* Z_Construct_UClass_UQuitGameAction();
UINAVIGATION_API UClass* Z_Construct_UClass_UQuitGameAction_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavComponentAction();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UQuitGameAction
void UQuitGameAction::StaticRegisterNativesUQuitGameAction()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UQuitGameAction);
UClass* Z_Construct_UClass_UQuitGameAction_NoRegister()
{
	return UQuitGameAction::StaticClass();
}
struct Z_Construct_UClass_UQuitGameAction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "ComponentActions/QuitGameAction.h" },
		{ "ModuleRelativePath", "Public/ComponentActions/QuitGameAction.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UQuitGameAction>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UQuitGameAction_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUINavComponentAction,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UQuitGameAction_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UQuitGameAction_Statics::ClassParams = {
	&UQuitGameAction::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x003010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UQuitGameAction_Statics::Class_MetaDataParams), Z_Construct_UClass_UQuitGameAction_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UQuitGameAction()
{
	if (!Z_Registration_Info_UClass_UQuitGameAction.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UQuitGameAction.OuterSingleton, Z_Construct_UClass_UQuitGameAction_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UQuitGameAction.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UQuitGameAction>()
{
	return UQuitGameAction::StaticClass();
}
UQuitGameAction::UQuitGameAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UQuitGameAction);
UQuitGameAction::~UQuitGameAction() {}
// End Class UQuitGameAction

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_QuitGameAction_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UQuitGameAction, UQuitGameAction::StaticClass, TEXT("UQuitGameAction"), &Z_Registration_Info_UClass_UQuitGameAction, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UQuitGameAction), 2382754617U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_QuitGameAction_h_1158158444(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_QuitGameAction_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_QuitGameAction_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
