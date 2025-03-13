// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavInputBox.h"
#include "UINavigation/Public/Data/InputContainerEnhancedActionData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavInputBox() {}

// Begin Cross Module References
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavInputBox();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavInputBox_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavInputComponent_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavInputContainer_NoRegister();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FInputContainerEnhancedActionData();
UMG_API UClass* Z_Construct_UClass_URichTextBlock_NoRegister();
UMG_API UClass* Z_Construct_UClass_UTextBlock_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavInputBox Function InputComponent1Clicked
struct Z_Construct_UFunction_UUINavInputBox_InputComponent1Clicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavInputBox_InputComponent1Clicked_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavInputBox, nullptr, "InputComponent1Clicked", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavInputBox_InputComponent1Clicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavInputBox_InputComponent1Clicked_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavInputBox_InputComponent1Clicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavInputBox_InputComponent1Clicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavInputBox::execInputComponent1Clicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->InputComponent1Clicked();
	P_NATIVE_END;
}
// End Class UUINavInputBox Function InputComponent1Clicked

// Begin Class UUINavInputBox Function InputComponent2Clicked
struct Z_Construct_UFunction_UUINavInputBox_InputComponent2Clicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavInputBox_InputComponent2Clicked_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavInputBox, nullptr, "InputComponent2Clicked", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavInputBox_InputComponent2Clicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavInputBox_InputComponent2Clicked_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavInputBox_InputComponent2Clicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavInputBox_InputComponent2Clicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavInputBox::execInputComponent2Clicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->InputComponent2Clicked();
	P_NATIVE_END;
}
// End Class UUINavInputBox Function InputComponent2Clicked

// Begin Class UUINavInputBox Function InputComponent3Clicked
struct Z_Construct_UFunction_UUINavInputBox_InputComponent3Clicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavInputBox_InputComponent3Clicked_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavInputBox, nullptr, "InputComponent3Clicked", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavInputBox_InputComponent3Clicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavInputBox_InputComponent3Clicked_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavInputBox_InputComponent3Clicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavInputBox_InputComponent3Clicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavInputBox::execInputComponent3Clicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->InputComponent3Clicked();
	P_NATIVE_END;
}
// End Class UUINavInputBox Function InputComponent3Clicked

// Begin Class UUINavInputBox
void UUINavInputBox::StaticRegisterNativesUUINavInputBox()
{
	UClass* Class = UUINavInputBox::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "InputComponent1Clicked", &UUINavInputBox::execInputComponent1Clicked },
		{ "InputComponent2Clicked", &UUINavInputBox::execInputComponent2Clicked },
		{ "InputComponent3Clicked", &UUINavInputBox::execInputComponent3Clicked },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavInputBox);
UClass* Z_Construct_UClass_UUINavInputBox_NoRegister()
{
	return UUINavInputBox::StaticClass();
}
struct Z_Construct_UClass_UUINavInputBox_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n* This class contains the logic for rebinding input keys to their respective actions\n*/" },
#endif
		{ "IncludePath", "UINavInputBox.h" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This class contains the logic for rebinding input keys to their respective actions" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputButton1_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINav Input" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputButton2_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINav Input" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputButton3_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINav Input" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputText_MetaData[] = {
		{ "BindWidgetOptional", "" },
		{ "Category", "UINav Input" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputRichText_MetaData[] = {
		{ "BindWidgetOptional", "" },
		{ "Category", "UINav Input" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Container_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputName_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputContext_MetaData[] = {
		{ "Category", "Enhanced Input" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputActionData_MetaData[] = {
		{ "Category", "Enhanced Input" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputTextStyleRowName_MetaData[] = {
		{ "Category", "UINavPromptWidget" },
		{ "ModuleRelativePath", "Public/UINavInputBox.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputButton1;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputButton2;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputButton3;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputRichText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Container;
	static const UECodeGen_Private::FNamePropertyParams NewProp_InputName;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputContext;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InputActionData;
	static const UECodeGen_Private::FStrPropertyParams NewProp_InputTextStyleRowName;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUINavInputBox_InputComponent1Clicked, "InputComponent1Clicked" }, // 1452248101
		{ &Z_Construct_UFunction_UUINavInputBox_InputComponent2Clicked, "InputComponent2Clicked" }, // 924899376
		{ &Z_Construct_UFunction_UUINavInputBox_InputComponent3Clicked, "InputComponent3Clicked" }, // 1184393343
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavInputBox>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputButton1 = { "InputButton1", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputBox, InputButton1), Z_Construct_UClass_UUINavInputComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputButton1_MetaData), NewProp_InputButton1_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputButton2 = { "InputButton2", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputBox, InputButton2), Z_Construct_UClass_UUINavInputComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputButton2_MetaData), NewProp_InputButton2_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputButton3 = { "InputButton3", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputBox, InputButton3), Z_Construct_UClass_UUINavInputComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputButton3_MetaData), NewProp_InputButton3_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputText = { "InputText", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputBox, InputText), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputText_MetaData), NewProp_InputText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputRichText = { "InputRichText", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputBox, InputRichText), Z_Construct_UClass_URichTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputRichText_MetaData), NewProp_InputRichText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputBox_Statics::NewProp_Container = { "Container", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputBox, Container), Z_Construct_UClass_UUINavInputContainer_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Container_MetaData), NewProp_Container_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputName = { "InputName", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputBox, InputName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputName_MetaData), NewProp_InputName_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputContext = { "InputContext", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputBox, InputContext), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputContext_MetaData), NewProp_InputContext_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputActionData = { "InputActionData", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputBox, InputActionData), Z_Construct_UScriptStruct_FInputContainerEnhancedActionData, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputActionData_MetaData), NewProp_InputActionData_MetaData) }; // 2773913468
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputTextStyleRowName = { "InputTextStyleRowName", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputBox, InputTextStyleRowName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputTextStyleRowName_MetaData), NewProp_InputTextStyleRowName_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavInputBox_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputButton1,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputButton2,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputButton3,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputRichText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputBox_Statics::NewProp_Container,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputActionData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputBox_Statics::NewProp_InputTextStyleRowName,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavInputBox_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavInputBox_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavInputBox_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavInputBox_Statics::ClassParams = {
	&UUINavInputBox::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UUINavInputBox_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavInputBox_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavInputBox_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavInputBox_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavInputBox()
{
	if (!Z_Registration_Info_UClass_UUINavInputBox.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavInputBox.OuterSingleton, Z_Construct_UClass_UUINavInputBox_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavInputBox.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavInputBox>()
{
	return UUINavInputBox::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavInputBox);
UUINavInputBox::~UUINavInputBox() {}
// End Class UUINavInputBox

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavInputBox_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavInputBox, UUINavInputBox::StaticClass, TEXT("UUINavInputBox"), &Z_Registration_Info_UClass_UUINavInputBox, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavInputBox), 4259161399U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavInputBox_h_174749077(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavInputBox_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavInputBox_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
