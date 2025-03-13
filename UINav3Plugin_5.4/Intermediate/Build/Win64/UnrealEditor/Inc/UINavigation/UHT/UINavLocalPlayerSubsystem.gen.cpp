// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavLocalPlayerSubsystem.h"
#include "Runtime/Engine/Classes/Engine/LocalPlayer.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavLocalPlayerSubsystem() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_ULocalPlayerSubsystem();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavLocalPlayerSubsystem();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavLocalPlayerSubsystem_NoRegister();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavLocalPlayerSubsystem
void UUINavLocalPlayerSubsystem::StaticRegisterNativesUUINavLocalPlayerSubsystem()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavLocalPlayerSubsystem);
UClass* Z_Construct_UClass_UUINavLocalPlayerSubsystem_NoRegister()
{
	return UUINavLocalPlayerSubsystem::StaticClass();
}
struct Z_Construct_UClass_UUINavLocalPlayerSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UINavLocalPlayerSubsystem.h" },
		{ "ModuleRelativePath", "Public/UINavLocalPlayerSubsystem.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavLocalPlayerSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UUINavLocalPlayerSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ULocalPlayerSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavLocalPlayerSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavLocalPlayerSubsystem_Statics::ClassParams = {
	&UUINavLocalPlayerSubsystem::StaticClass,
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
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavLocalPlayerSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavLocalPlayerSubsystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavLocalPlayerSubsystem()
{
	if (!Z_Registration_Info_UClass_UUINavLocalPlayerSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavLocalPlayerSubsystem.OuterSingleton, Z_Construct_UClass_UUINavLocalPlayerSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavLocalPlayerSubsystem.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavLocalPlayerSubsystem>()
{
	return UUINavLocalPlayerSubsystem::StaticClass();
}
UUINavLocalPlayerSubsystem::UUINavLocalPlayerSubsystem() {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavLocalPlayerSubsystem);
UUINavLocalPlayerSubsystem::~UUINavLocalPlayerSubsystem() {}
// End Class UUINavLocalPlayerSubsystem

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavLocalPlayerSubsystem_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavLocalPlayerSubsystem, UUINavLocalPlayerSubsystem::StaticClass, TEXT("UUINavLocalPlayerSubsystem"), &Z_Registration_Info_UClass_UUINavLocalPlayerSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavLocalPlayerSubsystem), 2047817127U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavLocalPlayerSubsystem_h_4126616491(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavLocalPlayerSubsystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavLocalPlayerSubsystem_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
