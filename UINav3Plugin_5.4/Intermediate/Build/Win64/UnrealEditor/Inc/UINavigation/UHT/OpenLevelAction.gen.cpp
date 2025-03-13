// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/ComponentActions/OpenLevelAction.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOpenLevelAction() {}

// Begin Cross Module References
UINAVIGATION_API UClass* Z_Construct_UClass_UOpenLevelAction();
UINAVIGATION_API UClass* Z_Construct_UClass_UOpenLevelAction_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavComponentAction();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UOpenLevelAction
void UOpenLevelAction::StaticRegisterNativesUOpenLevelAction()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UOpenLevelAction);
UClass* Z_Construct_UClass_UOpenLevelAction_NoRegister()
{
	return UOpenLevelAction::StaticClass();
}
struct Z_Construct_UClass_UOpenLevelAction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "ComponentActions/OpenLevelAction.h" },
		{ "ModuleRelativePath", "Public/ComponentActions/OpenLevelAction.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LevelName_MetaData[] = {
		{ "Category", "OpenLevelAction" },
		{ "ModuleRelativePath", "Public/ComponentActions/OpenLevelAction.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_LevelName;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOpenLevelAction>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UOpenLevelAction_Statics::NewProp_LevelName = { "LevelName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOpenLevelAction, LevelName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LevelName_MetaData), NewProp_LevelName_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOpenLevelAction_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOpenLevelAction_Statics::NewProp_LevelName,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOpenLevelAction_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UOpenLevelAction_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUINavComponentAction,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOpenLevelAction_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOpenLevelAction_Statics::ClassParams = {
	&UOpenLevelAction::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOpenLevelAction_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOpenLevelAction_Statics::PropPointers),
	0,
	0x003010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOpenLevelAction_Statics::Class_MetaDataParams), Z_Construct_UClass_UOpenLevelAction_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UOpenLevelAction()
{
	if (!Z_Registration_Info_UClass_UOpenLevelAction.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOpenLevelAction.OuterSingleton, Z_Construct_UClass_UOpenLevelAction_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOpenLevelAction.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UOpenLevelAction>()
{
	return UOpenLevelAction::StaticClass();
}
UOpenLevelAction::UOpenLevelAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UOpenLevelAction);
UOpenLevelAction::~UOpenLevelAction() {}
// End Class UOpenLevelAction

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_OpenLevelAction_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOpenLevelAction, UOpenLevelAction::StaticClass, TEXT("UOpenLevelAction"), &Z_Registration_Info_UClass_UOpenLevelAction, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOpenLevelAction), 502548143U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_OpenLevelAction_h_742019643(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_OpenLevelAction_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_OpenLevelAction_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
