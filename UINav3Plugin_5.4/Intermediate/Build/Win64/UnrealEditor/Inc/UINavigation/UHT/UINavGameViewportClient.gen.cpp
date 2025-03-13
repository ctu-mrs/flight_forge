// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavGameViewportClient.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavGameViewportClient() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UGameViewportClient();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavGameViewportClient();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavGameViewportClient_NoRegister();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavGameViewportClient
void UUINavGameViewportClient::StaticRegisterNativesUUINavGameViewportClient()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavGameViewportClient);
UClass* Z_Construct_UClass_UUINavGameViewportClient_NoRegister()
{
	return UUINavGameViewportClient::StaticClass();
}
struct Z_Construct_UClass_UUINavGameViewportClient_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UINavGameViewportClient.h" },
		{ "ModuleRelativePath", "Public/UINavGameViewportClient.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavGameViewportClient>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UUINavGameViewportClient_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGameViewportClient,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavGameViewportClient_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavGameViewportClient_Statics::ClassParams = {
	&UUINavGameViewportClient::StaticClass,
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
	0x001000ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavGameViewportClient_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavGameViewportClient_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavGameViewportClient()
{
	if (!Z_Registration_Info_UClass_UUINavGameViewportClient.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavGameViewportClient.OuterSingleton, Z_Construct_UClass_UUINavGameViewportClient_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavGameViewportClient.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavGameViewportClient>()
{
	return UUINavGameViewportClient::StaticClass();
}
UUINavGameViewportClient::UUINavGameViewportClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavGameViewportClient);
UUINavGameViewportClient::~UUINavGameViewportClient() {}
// End Class UUINavGameViewportClient

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavGameViewportClient_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavGameViewportClient, UUINavGameViewportClient::StaticClass, TEXT("UUINavGameViewportClient"), &Z_Registration_Info_UClass_UUINavGameViewportClient, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavGameViewportClient), 1587590609U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavGameViewportClient_h_354521385(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavGameViewportClient_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavGameViewportClient_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
