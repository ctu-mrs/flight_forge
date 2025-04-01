// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/ComponentActions/GoToWidgetAction.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGoToWidgetAction() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
UINAVIGATION_API UClass* Z_Construct_UClass_UGoToWidgetAction();
UINAVIGATION_API UClass* Z_Construct_UClass_UGoToWidgetAction_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavComponentAction();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavWidget_NoRegister();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UGoToWidgetAction
void UGoToWidgetAction::StaticRegisterNativesUGoToWidgetAction()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGoToWidgetAction);
UClass* Z_Construct_UClass_UGoToWidgetAction_NoRegister()
{
	return UGoToWidgetAction::StaticClass();
}
struct Z_Construct_UClass_UGoToWidgetAction_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "ComponentActions/GoToWidgetAction.h" },
		{ "ModuleRelativePath", "Public/ComponentActions/GoToWidgetAction.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WidgetClass_MetaData[] = {
		{ "Category", "GoToWidgetAction" },
		{ "ModuleRelativePath", "Public/ComponentActions/GoToWidgetAction.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRemoveParent_MetaData[] = {
		{ "Category", "GoToWidgetAction" },
		{ "ModuleRelativePath", "Public/ComponentActions/GoToWidgetAction.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bDestroyParent_MetaData[] = {
		{ "Category", "GoToWidgetAction" },
		{ "ModuleRelativePath", "Public/ComponentActions/GoToWidgetAction.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ZOrder_MetaData[] = {
		{ "Category", "GoToWidgetAction" },
		{ "ModuleRelativePath", "Public/ComponentActions/GoToWidgetAction.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_WidgetClass;
	static void NewProp_bRemoveParent_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRemoveParent;
	static void NewProp_bDestroyParent_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bDestroyParent;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ZOrder;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGoToWidgetAction>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_WidgetClass = { "WidgetClass", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGoToWidgetAction, WidgetClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WidgetClass_MetaData), NewProp_WidgetClass_MetaData) };
void Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_bRemoveParent_SetBit(void* Obj)
{
	((UGoToWidgetAction*)Obj)->bRemoveParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_bRemoveParent = { "bRemoveParent", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UGoToWidgetAction), &Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_bRemoveParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRemoveParent_MetaData), NewProp_bRemoveParent_MetaData) };
void Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_bDestroyParent_SetBit(void* Obj)
{
	((UGoToWidgetAction*)Obj)->bDestroyParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_bDestroyParent = { "bDestroyParent", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UGoToWidgetAction), &Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_bDestroyParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDestroyParent_MetaData), NewProp_bDestroyParent_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_ZOrder = { "ZOrder", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGoToWidgetAction, ZOrder), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ZOrder_MetaData), NewProp_ZOrder_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGoToWidgetAction_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_WidgetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_bRemoveParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_bDestroyParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGoToWidgetAction_Statics::NewProp_ZOrder,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGoToWidgetAction_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGoToWidgetAction_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUINavComponentAction,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGoToWidgetAction_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGoToWidgetAction_Statics::ClassParams = {
	&UGoToWidgetAction::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGoToWidgetAction_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGoToWidgetAction_Statics::PropPointers),
	0,
	0x003010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGoToWidgetAction_Statics::Class_MetaDataParams), Z_Construct_UClass_UGoToWidgetAction_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGoToWidgetAction()
{
	if (!Z_Registration_Info_UClass_UGoToWidgetAction.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGoToWidgetAction.OuterSingleton, Z_Construct_UClass_UGoToWidgetAction_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGoToWidgetAction.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UGoToWidgetAction>()
{
	return UGoToWidgetAction::StaticClass();
}
UGoToWidgetAction::UGoToWidgetAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGoToWidgetAction);
UGoToWidgetAction::~UGoToWidgetAction() {}
// End Class UGoToWidgetAction

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_GoToWidgetAction_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGoToWidgetAction, UGoToWidgetAction::StaticClass, TEXT("UGoToWidgetAction"), &Z_Registration_Info_UClass_UGoToWidgetAction, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGoToWidgetAction), 3312295309U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_GoToWidgetAction_h_1656591658(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_GoToWidgetAction_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_GoToWidgetAction_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
