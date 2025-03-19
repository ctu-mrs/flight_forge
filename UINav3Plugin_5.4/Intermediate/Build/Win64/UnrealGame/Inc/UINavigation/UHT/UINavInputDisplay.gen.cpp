// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavInputDisplay.h"
#include "Runtime/SlateCore/Public/Fonts/SlateFontInfo.h"
#include "Runtime/SlateCore/Public/Styling/SlateColor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavInputDisplay() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
SLATECORE_API UScriptStruct* Z_Construct_UScriptStruct_FSlateColor();
SLATECORE_API UScriptStruct* Z_Construct_UScriptStruct_FSlateFontInfo();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavInputDisplay();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavInputDisplay_NoRegister();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EAxisType();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputAxis();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputDisplayType();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputRestriction();
UMG_API UClass* Z_Construct_UClass_UImage_NoRegister();
UMG_API UClass* Z_Construct_UClass_URichTextBlock_NoRegister();
UMG_API UClass* Z_Construct_UClass_UTextBlock_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavInputDisplay Function SetIconSize
struct Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics
{
	struct UINavInputDisplay_eventSetIconSize_Parms
	{
		FVector2D NewSize;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "InputDisplay" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewSize_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_NewSize;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::NewProp_NewSize = { "NewSize", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavInputDisplay_eventSetIconSize_Parms, NewSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewSize_MetaData), NewProp_NewSize_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::NewProp_NewSize,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavInputDisplay, nullptr, "SetIconSize", nullptr, nullptr, Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::UINavInputDisplay_eventSetIconSize_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::UINavInputDisplay_eventSetIconSize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavInputDisplay_SetIconSize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavInputDisplay_SetIconSize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavInputDisplay::execSetIconSize)
{
	P_GET_STRUCT_REF(FVector2D,Z_Param_Out_NewSize);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetIconSize(Z_Param_Out_NewSize);
	P_NATIVE_END;
}
// End Class UUINavInputDisplay Function SetIconSize

// Begin Class UUINavInputDisplay Function SetInputAction
struct Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics
{
	struct UINavInputDisplay_eventSetInputAction_Parms
	{
		UInputAction* NewAction;
		EInputAxis NewAxis;
		EAxisType NewScale;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "InputDisplay" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewAxis_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewScale_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewAction;
	static const UECodeGen_Private::FBytePropertyParams NewProp_NewAxis_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NewAxis;
	static const UECodeGen_Private::FBytePropertyParams NewProp_NewScale_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NewScale;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::NewProp_NewAction = { "NewAction", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavInputDisplay_eventSetInputAction_Parms, NewAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::NewProp_NewAxis_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::NewProp_NewAxis = { "NewAxis", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavInputDisplay_eventSetInputAction_Parms, NewAxis), Z_Construct_UEnum_UINavigation_EInputAxis, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewAxis_MetaData), NewProp_NewAxis_MetaData) }; // 2063529479
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::NewProp_NewScale_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::NewProp_NewScale = { "NewScale", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavInputDisplay_eventSetInputAction_Parms, NewScale), Z_Construct_UEnum_UINavigation_EAxisType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewScale_MetaData), NewProp_NewScale_MetaData) }; // 2012741077
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::NewProp_NewAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::NewProp_NewAxis_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::NewProp_NewAxis,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::NewProp_NewScale_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::NewProp_NewScale,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavInputDisplay, nullptr, "SetInputAction", nullptr, nullptr, Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::UINavInputDisplay_eventSetInputAction_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::UINavInputDisplay_eventSetInputAction_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavInputDisplay_SetInputAction()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavInputDisplay_SetInputAction_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavInputDisplay::execSetInputAction)
{
	P_GET_OBJECT(UInputAction,Z_Param_NewAction);
	P_GET_ENUM(EInputAxis,Z_Param_NewAxis);
	P_GET_ENUM(EAxisType,Z_Param_NewScale);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetInputAction(Z_Param_NewAction,EInputAxis(Z_Param_NewAxis),EAxisType(Z_Param_NewScale));
	P_NATIVE_END;
}
// End Class UUINavInputDisplay Function SetInputAction

// Begin Class UUINavInputDisplay Function UpdateInputVisuals
struct Z_Construct_UFunction_UUINavInputDisplay_UpdateInputVisuals_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "InputDisplay" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavInputDisplay_UpdateInputVisuals_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavInputDisplay, nullptr, "UpdateInputVisuals", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavInputDisplay_UpdateInputVisuals_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavInputDisplay_UpdateInputVisuals_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavInputDisplay_UpdateInputVisuals()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavInputDisplay_UpdateInputVisuals_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavInputDisplay::execUpdateInputVisuals)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->UpdateInputVisuals();
	P_NATIVE_END;
}
// End Class UUINavInputDisplay Function UpdateInputVisuals

// Begin Class UUINavInputDisplay
void UUINavInputDisplay::StaticRegisterNativesUUINavInputDisplay()
{
	UClass* Class = UUINavInputDisplay::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "SetIconSize", &UUINavInputDisplay::execSetIconSize },
		{ "SetInputAction", &UUINavInputDisplay::execSetInputAction },
		{ "UpdateInputVisuals", &UUINavInputDisplay::execUpdateInputVisuals },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavInputDisplay);
UClass* Z_Construct_UClass_UUINavInputDisplay_NoRegister()
{
	return UUINavInputDisplay::StaticClass();
}
struct Z_Construct_UClass_UUINavInputDisplay_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UINavInputDisplay.h" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputTypeRestriction_MetaData[] = {
		{ "Category", "InputDisplay" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Locks to a specific input type. If set to None, it is automatically detected\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Locks to a specific input type. If set to None, it is automatically detected" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputImage_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "InputDisplay" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputText_MetaData[] = {
		{ "BindWidgetOptional", "" },
		{ "Category", "InputDisplay" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputRichText_MetaData[] = {
		{ "BindWidgetOptional", "" },
		{ "Category", "InputDisplay" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputAction_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Axis_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Scale_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisplayType_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bMatchIconSize_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_IconSize_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "editcondition", "!bMatchIconSize" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TextColorOverride_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "editcondition", "bOverride_TextColor" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bOverride_TextColor_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "InlineEditConditionToggle", "" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FontOverride_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "editcondition", "bOverride_Font" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bOverride_Font_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "InlineEditConditionToggle", "" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StyleRowName_MetaData[] = {
		{ "Category", "InputDisplay" },
		{ "ModuleRelativePath", "Public/UINavInputDisplay.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_InputTypeRestriction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InputTypeRestriction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputImage;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputRichText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputAction;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Axis_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Axis;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Scale_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Scale;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DisplayType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DisplayType;
	static void NewProp_bMatchIconSize_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bMatchIconSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_IconSize;
	static const UECodeGen_Private::FStructPropertyParams NewProp_TextColorOverride;
	static void NewProp_bOverride_TextColor_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bOverride_TextColor;
	static const UECodeGen_Private::FStructPropertyParams NewProp_FontOverride;
	static void NewProp_bOverride_Font_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bOverride_Font;
	static const UECodeGen_Private::FStrPropertyParams NewProp_StyleRowName;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUINavInputDisplay_SetIconSize, "SetIconSize" }, // 3533683032
		{ &Z_Construct_UFunction_UUINavInputDisplay_SetInputAction, "SetInputAction" }, // 2439321463
		{ &Z_Construct_UFunction_UUINavInputDisplay_UpdateInputVisuals, "UpdateInputVisuals" }, // 81857468
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavInputDisplay>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputTypeRestriction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputTypeRestriction = { "InputTypeRestriction", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, InputTypeRestriction), Z_Construct_UEnum_UINavigation_EInputRestriction, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputTypeRestriction_MetaData), NewProp_InputTypeRestriction_MetaData) }; // 2300298404
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputImage = { "InputImage", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, InputImage), Z_Construct_UClass_UImage_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputImage_MetaData), NewProp_InputImage_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputText = { "InputText", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, InputText), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputText_MetaData), NewProp_InputText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputRichText = { "InputRichText", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, InputRichText), Z_Construct_UClass_URichTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputRichText_MetaData), NewProp_InputRichText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputAction = { "InputAction", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, InputAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputAction_MetaData), NewProp_InputAction_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_Axis_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_Axis = { "Axis", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, Axis), Z_Construct_UEnum_UINavigation_EInputAxis, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Axis_MetaData), NewProp_Axis_MetaData) }; // 2063529479
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_Scale_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_Scale = { "Scale", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, Scale), Z_Construct_UEnum_UINavigation_EAxisType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Scale_MetaData), NewProp_Scale_MetaData) }; // 2012741077
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_DisplayType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_DisplayType = { "DisplayType", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, DisplayType), Z_Construct_UEnum_UINavigation_EInputDisplayType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisplayType_MetaData), NewProp_DisplayType_MetaData) }; // 61414587
void Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bMatchIconSize_SetBit(void* Obj)
{
	((UUINavInputDisplay*)Obj)->bMatchIconSize = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bMatchIconSize = { "bMatchIconSize", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavInputDisplay), &Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bMatchIconSize_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bMatchIconSize_MetaData), NewProp_bMatchIconSize_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_IconSize = { "IconSize", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, IconSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_IconSize_MetaData), NewProp_IconSize_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_TextColorOverride = { "TextColorOverride", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, TextColorOverride), Z_Construct_UScriptStruct_FSlateColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TextColorOverride_MetaData), NewProp_TextColorOverride_MetaData) }; // 2024315916
void Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bOverride_TextColor_SetBit(void* Obj)
{
	((UUINavInputDisplay*)Obj)->bOverride_TextColor = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bOverride_TextColor = { "bOverride_TextColor", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Bool , RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(uint8), sizeof(UUINavInputDisplay), &Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bOverride_TextColor_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bOverride_TextColor_MetaData), NewProp_bOverride_TextColor_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_FontOverride = { "FontOverride", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, FontOverride), Z_Construct_UScriptStruct_FSlateFontInfo, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FontOverride_MetaData), NewProp_FontOverride_MetaData) }; // 2419385967
void Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bOverride_Font_SetBit(void* Obj)
{
	((UUINavInputDisplay*)Obj)->bOverride_Font = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bOverride_Font = { "bOverride_Font", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Bool , RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(uint8), sizeof(UUINavInputDisplay), &Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bOverride_Font_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bOverride_Font_MetaData), NewProp_bOverride_Font_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_StyleRowName = { "StyleRowName", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavInputDisplay, StyleRowName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StyleRowName_MetaData), NewProp_StyleRowName_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavInputDisplay_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputTypeRestriction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputTypeRestriction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputImage,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputRichText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_InputAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_Axis_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_Axis,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_Scale_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_Scale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_DisplayType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_DisplayType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bMatchIconSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_IconSize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_TextColorOverride,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bOverride_TextColor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_FontOverride,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_bOverride_Font,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavInputDisplay_Statics::NewProp_StyleRowName,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavInputDisplay_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavInputDisplay_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavInputDisplay_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavInputDisplay_Statics::ClassParams = {
	&UUINavInputDisplay::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UUINavInputDisplay_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavInputDisplay_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavInputDisplay_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavInputDisplay_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavInputDisplay()
{
	if (!Z_Registration_Info_UClass_UUINavInputDisplay.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavInputDisplay.OuterSingleton, Z_Construct_UClass_UUINavInputDisplay_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavInputDisplay.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavInputDisplay>()
{
	return UUINavInputDisplay::StaticClass();
}
UUINavInputDisplay::UUINavInputDisplay(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavInputDisplay);
UUINavInputDisplay::~UUINavInputDisplay() {}
// End Class UUINavInputDisplay

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavInputDisplay_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavInputDisplay, UUINavInputDisplay::StaticClass, TEXT("UUINavInputDisplay"), &Z_Registration_Info_UClass_UUINavInputDisplay, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavInputDisplay), 145093857U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavInputDisplay_h_1298095140(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavInputDisplay_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavInputDisplay_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
