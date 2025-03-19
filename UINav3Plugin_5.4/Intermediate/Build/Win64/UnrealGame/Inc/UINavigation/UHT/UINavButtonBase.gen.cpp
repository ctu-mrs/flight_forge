// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavButtonBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavButtonBase() {}

// Begin Cross Module References
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavButtonBase();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavButtonBase_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavButtonBase
void UUINavButtonBase::StaticRegisterNativesUUINavButtonBase()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavButtonBase);
UClass* Z_Construct_UClass_UUINavButtonBase_NoRegister()
{
	return UUINavButtonBase::StaticClass();
}
struct Z_Construct_UClass_UUINavButtonBase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UINavButtonBase.h" },
		{ "ModuleRelativePath", "Public/UINavButtonBase.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavButtonBase>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UUINavButtonBase_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UButton,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavButtonBase_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavButtonBase_Statics::ClassParams = {
	&UUINavButtonBase::StaticClass,
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
	0x00B000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavButtonBase_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavButtonBase_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavButtonBase()
{
	if (!Z_Registration_Info_UClass_UUINavButtonBase.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavButtonBase.OuterSingleton, Z_Construct_UClass_UUINavButtonBase_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavButtonBase.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavButtonBase>()
{
	return UUINavButtonBase::StaticClass();
}
UUINavButtonBase::UUINavButtonBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavButtonBase);
UUINavButtonBase::~UUINavButtonBase() {}
// End Class UUINavButtonBase

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavButtonBase_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavButtonBase, UUINavButtonBase::StaticClass, TEXT("UUINavButtonBase"), &Z_Registration_Info_UClass_UUINavButtonBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavButtonBase), 466189490U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavButtonBase_h_4172541642(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavButtonBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavButtonBase_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
