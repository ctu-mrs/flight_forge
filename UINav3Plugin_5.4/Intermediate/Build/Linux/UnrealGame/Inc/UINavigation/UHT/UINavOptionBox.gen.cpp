// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavOptionBox.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavOptionBox() {}

// Begin Cross Module References
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavComponentBox();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavOptionBox();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavOptionBox_NoRegister();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavOptionBox Function GetCurrentString
struct Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics
{
	struct UINavOptionBox_eventGetCurrentString_Parms
	{
		FText ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavOptionBox" },
		{ "ModuleRelativePath", "Public/UINavOptionBox.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavOptionBox_eventGetCurrentString_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavOptionBox, nullptr, "GetCurrentString", nullptr, nullptr, Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::UINavOptionBox_eventGetCurrentString_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::UINavOptionBox_eventGetCurrentString_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavOptionBox_GetCurrentString()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavOptionBox_GetCurrentString_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavOptionBox::execGetCurrentString)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FText*)Z_Param__Result=P_THIS->GetCurrentString();
	P_NATIVE_END;
}
// End Class UUINavOptionBox Function GetCurrentString

// Begin Class UUINavOptionBox
void UUINavOptionBox::StaticRegisterNativesUUINavOptionBox()
{
	UClass* Class = UUINavOptionBox::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetCurrentString", &UUINavOptionBox::execGetCurrentString },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavOptionBox);
UClass* Z_Construct_UClass_UUINavOptionBox_NoRegister()
{
	return UUINavOptionBox::StaticClass();
}
struct Z_Construct_UClass_UUINavOptionBox_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UINavOptionBox.h" },
		{ "ModuleRelativePath", "Public/UINavOptionBox.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseNumberRange_MetaData[] = {
		{ "Category", "UINavOptionBox" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*If set to false, will use StringOptions, otherwise will use\n\x09""all integers in designated range (from MinRange to MaxRange, inclusive)*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavOptionBox.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "If set to false, will use StringOptions, otherwise will use\n       all integers in designated range (from MinRange to MaxRange, inclusive)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StringOptions_MetaData[] = {
		{ "Category", "UINavOptionBox" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//The list of Names to display as options in this slider\n" },
#endif
		{ "EditCondition", "!bUseNumberRange" },
		{ "ModuleRelativePath", "Public/UINavOptionBox.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The list of Names to display as options in this slider" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bUseNumberRange_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseNumberRange;
	static const UECodeGen_Private::FTextPropertyParams NewProp_StringOptions_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_StringOptions;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUINavOptionBox_GetCurrentString, "GetCurrentString" }, // 2453700879
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavOptionBox>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
void Z_Construct_UClass_UUINavOptionBox_Statics::NewProp_bUseNumberRange_SetBit(void* Obj)
{
	((UUINavOptionBox*)Obj)->bUseNumberRange = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavOptionBox_Statics::NewProp_bUseNumberRange = { "bUseNumberRange", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavOptionBox), &Z_Construct_UClass_UUINavOptionBox_Statics::NewProp_bUseNumberRange_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseNumberRange_MetaData), NewProp_bUseNumberRange_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UClass_UUINavOptionBox_Statics::NewProp_StringOptions_Inner = { "StringOptions", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UUINavOptionBox_Statics::NewProp_StringOptions = { "StringOptions", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavOptionBox, StringOptions), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StringOptions_MetaData), NewProp_StringOptions_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavOptionBox_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavOptionBox_Statics::NewProp_bUseNumberRange,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavOptionBox_Statics::NewProp_StringOptions_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavOptionBox_Statics::NewProp_StringOptions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavOptionBox_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavOptionBox_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUINavComponentBox,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavOptionBox_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavOptionBox_Statics::ClassParams = {
	&UUINavOptionBox::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UUINavOptionBox_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavOptionBox_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavOptionBox_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavOptionBox_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavOptionBox()
{
	if (!Z_Registration_Info_UClass_UUINavOptionBox.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavOptionBox.OuterSingleton, Z_Construct_UClass_UUINavOptionBox_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavOptionBox.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavOptionBox>()
{
	return UUINavOptionBox::StaticClass();
}
UUINavOptionBox::UUINavOptionBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavOptionBox);
UUINavOptionBox::~UUINavOptionBox() {}
// End Class UUINavOptionBox

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavOptionBox_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavOptionBox, UUINavOptionBox::StaticClass, TEXT("UUINavOptionBox"), &Z_Registration_Info_UClass_UUINavOptionBox, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavOptionBox), 2771081282U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavOptionBox_h_1613659671(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavOptionBox_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavOptionBox_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
