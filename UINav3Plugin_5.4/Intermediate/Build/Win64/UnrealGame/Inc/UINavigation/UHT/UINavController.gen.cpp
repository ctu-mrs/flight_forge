// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavController.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavController() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_APlayerController();
UINAVIGATION_API UClass* Z_Construct_UClass_AUINavController();
UINAVIGATION_API UClass* Z_Construct_UClass_AUINavController_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPCComponent_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPCReceiver_NoRegister();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class AUINavController
void AUINavController::StaticRegisterNativesAUINavController()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AUINavController);
UClass* Z_Construct_UClass_AUINavController_NoRegister()
{
	return AUINavController::StaticClass();
}
struct Z_Construct_UClass_AUINavController_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * This class contains the logic for input-related actions with UINavWidgets\n */" },
#endif
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "UINavController.h" },
		{ "ModuleRelativePath", "Public/UINavController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This class contains the logic for input-related actions with UINavWidgets" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UINavPCComp_MetaData[] = {
		{ "Category", "UINavController" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavController.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UINavPCComp;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AUINavController>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AUINavController_Statics::NewProp_UINavPCComp = { "UINavPCComp", nullptr, (EPropertyFlags)0x00200800000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AUINavController, UINavPCComp), Z_Construct_UClass_UUINavPCComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UINavPCComp_MetaData), NewProp_UINavPCComp_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AUINavController_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AUINavController_Statics::NewProp_UINavPCComp,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AUINavController_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_AUINavController_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerController,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AUINavController_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_AUINavController_Statics::InterfaceParams[] = {
	{ Z_Construct_UClass_UUINavPCReceiver_NoRegister, (int32)VTABLE_OFFSET(AUINavController, IUINavPCReceiver), false },  // 1279494643
};
const UECodeGen_Private::FClassParams Z_Construct_UClass_AUINavController_Statics::ClassParams = {
	&AUINavController::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_AUINavController_Statics::PropPointers,
	InterfaceParams,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_AUINavController_Statics::PropPointers),
	UE_ARRAY_COUNT(InterfaceParams),
	0x009002A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AUINavController_Statics::Class_MetaDataParams), Z_Construct_UClass_AUINavController_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AUINavController()
{
	if (!Z_Registration_Info_UClass_AUINavController.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AUINavController.OuterSingleton, Z_Construct_UClass_AUINavController_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AUINavController.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<AUINavController>()
{
	return AUINavController::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AUINavController);
AUINavController::~AUINavController() {}
// End Class AUINavController

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavController_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AUINavController, AUINavController::StaticClass, TEXT("AUINavController"), &Z_Registration_Info_UClass_AUINavController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AUINavController), 3949267574U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavController_h_3202946589(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavController_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavController_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
