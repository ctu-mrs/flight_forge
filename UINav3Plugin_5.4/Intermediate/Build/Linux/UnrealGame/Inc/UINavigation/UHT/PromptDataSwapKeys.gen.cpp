// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/PromptDataSwapKeys.h"
#include "UINavigation/Public/Data/InputCollisionData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePromptDataSwapKeys() {}

// Begin Cross Module References
UINAVIGATION_API UClass* Z_Construct_UClass_UPromptDataBase();
UINAVIGATION_API UClass* Z_Construct_UClass_UPromptDataSwapKeys();
UINAVIGATION_API UClass* Z_Construct_UClass_UPromptDataSwapKeys_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavInputBox_NoRegister();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputCollisionData();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UPromptDataSwapKeys
void UPromptDataSwapKeys::StaticRegisterNativesUPromptDataSwapKeys()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPromptDataSwapKeys);
UClass* Z_Construct_UClass_UPromptDataSwapKeys_NoRegister()
{
	return UPromptDataSwapKeys::StaticClass();
}
struct Z_Construct_UClass_UPromptDataSwapKeys_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Data/PromptDataSwapKeys.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Data/PromptDataSwapKeys.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bShouldSwap_MetaData[] = {
		{ "Category", "Swap Keys Prompt Data" },
		{ "ModuleRelativePath", "Public/Data/PromptDataSwapKeys.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputCollisionData_MetaData[] = {
		{ "Category", "Swap Keys Prompt Data" },
		{ "ModuleRelativePath", "Public/Data/PromptDataSwapKeys.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentInputBox_MetaData[] = {
		{ "Category", "Swap Keys Prompt Data" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Data/PromptDataSwapKeys.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CollidingInputBox_MetaData[] = {
		{ "Category", "Swap Keys Prompt Data" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Data/PromptDataSwapKeys.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bShouldSwap_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bShouldSwap;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InputCollisionData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CurrentInputBox;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CollidingInputBox;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPromptDataSwapKeys>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
void Z_Construct_UClass_UPromptDataSwapKeys_Statics::NewProp_bShouldSwap_SetBit(void* Obj)
{
	((UPromptDataSwapKeys*)Obj)->bShouldSwap = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UPromptDataSwapKeys_Statics::NewProp_bShouldSwap = { "bShouldSwap", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UPromptDataSwapKeys), &Z_Construct_UClass_UPromptDataSwapKeys_Statics::NewProp_bShouldSwap_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bShouldSwap_MetaData), NewProp_bShouldSwap_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UPromptDataSwapKeys_Statics::NewProp_InputCollisionData = { "InputCollisionData", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UPromptDataSwapKeys, InputCollisionData), Z_Construct_UScriptStruct_FInputCollisionData, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputCollisionData_MetaData), NewProp_InputCollisionData_MetaData) }; // 3537832298
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UPromptDataSwapKeys_Statics::NewProp_CurrentInputBox = { "CurrentInputBox", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UPromptDataSwapKeys, CurrentInputBox), Z_Construct_UClass_UUINavInputBox_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentInputBox_MetaData), NewProp_CurrentInputBox_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UPromptDataSwapKeys_Statics::NewProp_CollidingInputBox = { "CollidingInputBox", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UPromptDataSwapKeys, CollidingInputBox), Z_Construct_UClass_UUINavInputBox_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CollidingInputBox_MetaData), NewProp_CollidingInputBox_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UPromptDataSwapKeys_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPromptDataSwapKeys_Statics::NewProp_bShouldSwap,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPromptDataSwapKeys_Statics::NewProp_InputCollisionData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPromptDataSwapKeys_Statics::NewProp_CurrentInputBox,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPromptDataSwapKeys_Statics::NewProp_CollidingInputBox,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UPromptDataSwapKeys_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UPromptDataSwapKeys_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UPromptDataBase,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UPromptDataSwapKeys_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UPromptDataSwapKeys_Statics::ClassParams = {
	&UPromptDataSwapKeys::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UPromptDataSwapKeys_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UPromptDataSwapKeys_Statics::PropPointers),
	0,
	0x008000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UPromptDataSwapKeys_Statics::Class_MetaDataParams), Z_Construct_UClass_UPromptDataSwapKeys_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UPromptDataSwapKeys()
{
	if (!Z_Registration_Info_UClass_UPromptDataSwapKeys.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPromptDataSwapKeys.OuterSingleton, Z_Construct_UClass_UPromptDataSwapKeys_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UPromptDataSwapKeys.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UPromptDataSwapKeys>()
{
	return UPromptDataSwapKeys::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UPromptDataSwapKeys);
UPromptDataSwapKeys::~UPromptDataSwapKeys() {}
// End Class UPromptDataSwapKeys

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PromptDataSwapKeys_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UPromptDataSwapKeys, UPromptDataSwapKeys::StaticClass, TEXT("UPromptDataSwapKeys"), &Z_Registration_Info_UClass_UPromptDataSwapKeys, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPromptDataSwapKeys), 11005191U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PromptDataSwapKeys_h_322214738(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PromptDataSwapKeys_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PromptDataSwapKeys_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
