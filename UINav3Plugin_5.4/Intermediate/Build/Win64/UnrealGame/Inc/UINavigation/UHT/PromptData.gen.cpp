// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/Data/PromptData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePromptData() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
UINAVIGATION_API UClass* Z_Construct_UClass_UPromptDataBase();
UINAVIGATION_API UClass* Z_Construct_UClass_UPromptDataBase_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UPromptDataBinary();
UINAVIGATION_API UClass* Z_Construct_UClass_UPromptDataBinary_NoRegister();
UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Delegate FPromptWidgetDecided
struct Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics
{
	struct _Script_UINavigation_eventPromptWidgetDecided_Parms
	{
		const UPromptDataBase* PromptData;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Data/PromptData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PromptData_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PromptData;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::NewProp_PromptData = { "PromptData", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_UINavigation_eventPromptWidgetDecided_Parms, PromptData), Z_Construct_UClass_UPromptDataBase_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PromptData_MetaData), NewProp_PromptData_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::NewProp_PromptData,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_UINavigation, nullptr, "PromptWidgetDecided__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::_Script_UINavigation_eventPromptWidgetDecided_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00120000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::_Script_UINavigation_eventPromptWidgetDecided_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FPromptWidgetDecided_DelegateWrapper(const FScriptDelegate& PromptWidgetDecided, const UPromptDataBase* PromptData)
{
	struct _Script_UINavigation_eventPromptWidgetDecided_Parms
	{
		const UPromptDataBase* PromptData;
	};
	_Script_UINavigation_eventPromptWidgetDecided_Parms Parms;
	Parms.PromptData=PromptData;
	PromptWidgetDecided.ProcessDelegate<UObject>(&Parms);
}
// End Delegate FPromptWidgetDecided

// Begin Class UPromptDataBase
void UPromptDataBase::StaticRegisterNativesUPromptDataBase()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPromptDataBase);
UClass* Z_Construct_UClass_UPromptDataBase_NoRegister()
{
	return UPromptDataBase::StaticClass();
}
struct Z_Construct_UClass_UPromptDataBase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Data/PromptData.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Data/PromptData.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPromptDataBase>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UPromptDataBase_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UPromptDataBase_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UPromptDataBase_Statics::ClassParams = {
	&UPromptDataBase::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UPromptDataBase_Statics::Class_MetaDataParams), Z_Construct_UClass_UPromptDataBase_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UPromptDataBase()
{
	if (!Z_Registration_Info_UClass_UPromptDataBase.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPromptDataBase.OuterSingleton, Z_Construct_UClass_UPromptDataBase_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UPromptDataBase.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UPromptDataBase>()
{
	return UPromptDataBase::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UPromptDataBase);
UPromptDataBase::~UPromptDataBase() {}
// End Class UPromptDataBase

// Begin Class UPromptDataBinary
void UPromptDataBinary::StaticRegisterNativesUPromptDataBinary()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPromptDataBinary);
UClass* Z_Construct_UClass_UPromptDataBinary_NoRegister()
{
	return UPromptDataBinary::StaticClass();
}
struct Z_Construct_UClass_UPromptDataBinary_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Data/PromptData.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Data/PromptData.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAccept_MetaData[] = {
		{ "Category", "Prompt Data" },
		{ "ModuleRelativePath", "Public/Data/PromptData.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bAccept_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAccept;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPromptDataBinary>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
void Z_Construct_UClass_UPromptDataBinary_Statics::NewProp_bAccept_SetBit(void* Obj)
{
	((UPromptDataBinary*)Obj)->bAccept = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UPromptDataBinary_Statics::NewProp_bAccept = { "bAccept", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UPromptDataBinary), &Z_Construct_UClass_UPromptDataBinary_Statics::NewProp_bAccept_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAccept_MetaData), NewProp_bAccept_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UPromptDataBinary_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPromptDataBinary_Statics::NewProp_bAccept,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UPromptDataBinary_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UPromptDataBinary_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UPromptDataBase,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UPromptDataBinary_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UPromptDataBinary_Statics::ClassParams = {
	&UPromptDataBinary::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UPromptDataBinary_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UPromptDataBinary_Statics::PropPointers),
	0,
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UPromptDataBinary_Statics::Class_MetaDataParams), Z_Construct_UClass_UPromptDataBinary_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UPromptDataBinary()
{
	if (!Z_Registration_Info_UClass_UPromptDataBinary.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPromptDataBinary.OuterSingleton, Z_Construct_UClass_UPromptDataBinary_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UPromptDataBinary.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UPromptDataBinary>()
{
	return UPromptDataBinary::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UPromptDataBinary);
UPromptDataBinary::~UPromptDataBinary() {}
// End Class UPromptDataBinary

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PromptData_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UPromptDataBase, UPromptDataBase::StaticClass, TEXT("UPromptDataBase"), &Z_Registration_Info_UClass_UPromptDataBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPromptDataBase), 2186300622U) },
		{ Z_Construct_UClass_UPromptDataBinary, UPromptDataBinary::StaticClass, TEXT("UPromptDataBinary"), &Z_Registration_Info_UClass_UPromptDataBinary, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPromptDataBinary), 164386950U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PromptData_h_1404817715(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PromptData_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_PromptData_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
