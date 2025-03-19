// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavBlueprintFunctionLibrary.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavBlueprintFunctionLibrary() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
ENGINE_API UClass* Z_Construct_UClass_APlayerController_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
ENGINE_API UClass* Z_Construct_UClass_USoundClass_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
INPUTCORE_API UScriptStruct* Z_Construct_UScriptStruct_FKey();
UINAVIGATION_API UClass* Z_Construct_UClass_UPromptDataBinary_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavBlueprintFunctionLibrary();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavBlueprintFunctionLibrary_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavSettings_NoRegister();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputRestriction();
UMG_API UClass* Z_Construct_UClass_UPanelWidget_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget_NoRegister();
UMG_API UClass* Z_Construct_UClass_UWidget_NoRegister();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavBlueprintFunctionLibrary Function ApplyStyleRowToText
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics
{
	struct UINavBlueprintFunctionLibrary_eventApplyStyleRowToText_Parms
	{
		FText Text;
		FString StyleRowName;
		FText ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavigationLibrary" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Text_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StyleRowName_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_Text;
	static const UECodeGen_Private::FStrPropertyParams NewProp_StyleRowName;
	static const UECodeGen_Private::FTextPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::NewProp_Text = { "Text", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventApplyStyleRowToText_Parms, Text), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Text_MetaData), NewProp_Text_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::NewProp_StyleRowName = { "StyleRowName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventApplyStyleRowToText_Parms, StyleRowName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StyleRowName_MetaData), NewProp_StyleRowName_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventApplyStyleRowToText_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::NewProp_Text,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::NewProp_StyleRowName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "ApplyStyleRowToText", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::UINavBlueprintFunctionLibrary_eventApplyStyleRowToText_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::UINavBlueprintFunctionLibrary_eventApplyStyleRowToText_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execApplyStyleRowToText)
{
	P_GET_PROPERTY_REF(FTextProperty,Z_Param_Out_Text);
	P_GET_PROPERTY(FStrProperty,Z_Param_StyleRowName);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FText*)Z_Param__Result=UUINavBlueprintFunctionLibrary::ApplyStyleRowToText(Z_Param_Out_Text,Z_Param_StyleRowName);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function ApplyStyleRowToText

// Begin Class UUINavBlueprintFunctionLibrary Function CreateBinaryPromptData
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics
{
	struct UINavBlueprintFunctionLibrary_eventCreateBinaryPromptData_Parms
	{
		bool bAccept;
		UPromptDataBinary* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavigationLibrary" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAccept_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bAccept_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAccept;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::NewProp_bAccept_SetBit(void* Obj)
{
	((UINavBlueprintFunctionLibrary_eventCreateBinaryPromptData_Parms*)Obj)->bAccept = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::NewProp_bAccept = { "bAccept", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavBlueprintFunctionLibrary_eventCreateBinaryPromptData_Parms), &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::NewProp_bAccept_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAccept_MetaData), NewProp_bAccept_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventCreateBinaryPromptData_Parms, ReturnValue), Z_Construct_UClass_UPromptDataBinary_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::NewProp_bAccept,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "CreateBinaryPromptData", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::UINavBlueprintFunctionLibrary_eventCreateBinaryPromptData_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::UINavBlueprintFunctionLibrary_eventCreateBinaryPromptData_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execCreateBinaryPromptData)
{
	P_GET_UBOOL(Z_Param_bAccept);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UPromptDataBinary**)Z_Param__Result=UUINavBlueprintFunctionLibrary::CreateBinaryPromptData(Z_Param_bAccept);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function CreateBinaryPromptData

// Begin Class UUINavBlueprintFunctionLibrary Function FindWidgetOfClassesInWidget
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics
{
	struct UINavBlueprintFunctionLibrary_eventFindWidgetOfClassesInWidget_Parms
	{
		UWidget* Widget;
		TArray<TSubclassOf<UWidget> > WidgetClasses;
		UWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Finds the first widget of any of the classes provided starting from the given widget\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Finds the first widget of any of the classes provided starting from the given widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Widget_MetaData[] = {
		{ "EditInline", "true" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WidgetClasses_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Widget;
	static const UECodeGen_Private::FClassPropertyParams NewProp_WidgetClasses_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_WidgetClasses;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::NewProp_Widget = { "Widget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventFindWidgetOfClassesInWidget_Parms, Widget), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Widget_MetaData), NewProp_Widget_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::NewProp_WidgetClasses_Inner = { "WidgetClasses", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UClass, Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::NewProp_WidgetClasses = { "WidgetClasses", nullptr, (EPropertyFlags)0x0014000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventFindWidgetOfClassesInWidget_Parms, WidgetClasses), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WidgetClasses_MetaData), NewProp_WidgetClasses_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventFindWidgetOfClassesInWidget_Parms, ReturnValue), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::NewProp_Widget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::NewProp_WidgetClasses_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::NewProp_WidgetClasses,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "FindWidgetOfClassesInWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::UINavBlueprintFunctionLibrary_eventFindWidgetOfClassesInWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::UINavBlueprintFunctionLibrary_eventFindWidgetOfClassesInWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execFindWidgetOfClassesInWidget)
{
	P_GET_OBJECT(UWidget,Z_Param_Widget);
	P_GET_TARRAY_REF(TSubclassOf<UWidget>,Z_Param_Out_WidgetClasses);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UWidget**)Z_Param__Result=UUINavBlueprintFunctionLibrary::FindWidgetOfClassesInWidget(Z_Param_Widget,Z_Param_Out_WidgetClasses);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function FindWidgetOfClassesInWidget

// Begin Class UUINavBlueprintFunctionLibrary Function GetFirstWidgetInUserWidget
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics
{
	struct UINavBlueprintFunctionLibrary_eventGetFirstWidgetInUserWidget_Parms
	{
		const UUserWidget* UserWidget;
		UWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Returns first widget of the given widget tree\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Returns first widget of the given widget tree" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UserWidget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UserWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::NewProp_UserWidget = { "UserWidget", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetFirstWidgetInUserWidget_Parms, UserWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UserWidget_MetaData), NewProp_UserWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetFirstWidgetInUserWidget_Parms, ReturnValue), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::NewProp_UserWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "GetFirstWidgetInUserWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::UINavBlueprintFunctionLibrary_eventGetFirstWidgetInUserWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::UINavBlueprintFunctionLibrary_eventGetFirstWidgetInUserWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execGetFirstWidgetInUserWidget)
{
	P_GET_OBJECT(UUserWidget,Z_Param_UserWidget);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UWidget**)Z_Param__Result=UUINavBlueprintFunctionLibrary::GetFirstWidgetInUserWidget(Z_Param_UserWidget);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function GetFirstWidgetInUserWidget

// Begin Class UUINavBlueprintFunctionLibrary Function GetIndexInPanelWidget
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics
{
	struct UINavBlueprintFunctionLibrary_eventGetIndexInPanelWidget_Parms
	{
		const UWidget* Widget;
		TSubclassOf<UPanelWidget> PanelWidgetSubclass;
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Returns the index of the given widget in its parent panel widget\n// Will assume UPanelWidget if PanelWidgetSubclass is null\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Returns the index of the given widget in its parent panel widget\nWill assume UPanelWidget if PanelWidgetSubclass is null" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Widget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Widget;
	static const UECodeGen_Private::FClassPropertyParams NewProp_PanelWidgetSubclass;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::NewProp_Widget = { "Widget", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetIndexInPanelWidget_Parms, Widget), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Widget_MetaData), NewProp_Widget_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::NewProp_PanelWidgetSubclass = { "PanelWidgetSubclass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetIndexInPanelWidget_Parms, PanelWidgetSubclass), Z_Construct_UClass_UClass, Z_Construct_UClass_UPanelWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetIndexInPanelWidget_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::NewProp_Widget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::NewProp_PanelWidgetSubclass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "GetIndexInPanelWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::UINavBlueprintFunctionLibrary_eventGetIndexInPanelWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::UINavBlueprintFunctionLibrary_eventGetIndexInPanelWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execGetIndexInPanelWidget)
{
	P_GET_OBJECT(UWidget,Z_Param_Widget);
	P_GET_OBJECT(UClass,Z_Param_PanelWidgetSubclass);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=UUINavBlueprintFunctionLibrary::GetIndexInPanelWidget(Z_Param_Widget,Z_Param_PanelWidgetSubclass);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function GetIndexInPanelWidget

// Begin Class UUINavBlueprintFunctionLibrary Function GetIndexInUniformGridWidget
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics
{
	struct UINavBlueprintFunctionLibrary_eventGetIndexInUniformGridWidget_Parms
	{
		const UWidget* Widget;
		int32 Column;
		int32 Row;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Returns the index of the given widget in its parent panel widget\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Returns the index of the given widget in its parent panel widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Widget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Widget;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Column;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Row;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::NewProp_Widget = { "Widget", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetIndexInUniformGridWidget_Parms, Widget), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Widget_MetaData), NewProp_Widget_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::NewProp_Column = { "Column", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetIndexInUniformGridWidget_Parms, Column), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::NewProp_Row = { "Row", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetIndexInUniformGridWidget_Parms, Row), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::NewProp_Widget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::NewProp_Column,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::NewProp_Row,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "GetIndexInUniformGridWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::UINavBlueprintFunctionLibrary_eventGetIndexInUniformGridWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::UINavBlueprintFunctionLibrary_eventGetIndexInUniformGridWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execGetIndexInUniformGridWidget)
{
	P_GET_OBJECT(UWidget,Z_Param_Widget);
	P_GET_PROPERTY_REF(FIntProperty,Z_Param_Out_Column);
	P_GET_PROPERTY_REF(FIntProperty,Z_Param_Out_Row);
	P_FINISH;
	P_NATIVE_BEGIN;
	UUINavBlueprintFunctionLibrary::GetIndexInUniformGridWidget(Z_Param_Widget,Z_Param_Out_Column,Z_Param_Out_Row);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function GetIndexInUniformGridWidget

// Begin Class UUINavBlueprintFunctionLibrary Function GetPanelWidgetChild
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics
{
	struct UINavBlueprintFunctionLibrary_eventGetPanelWidgetChild_Parms
	{
		const UWidget* Widget;
		int32 ChildIndex;
		UWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Returns the child of the given panel widget at the given index\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Returns the child of the given panel widget at the given index" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Widget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ChildIndex_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Widget;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ChildIndex;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::NewProp_Widget = { "Widget", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetPanelWidgetChild_Parms, Widget), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Widget_MetaData), NewProp_Widget_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::NewProp_ChildIndex = { "ChildIndex", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetPanelWidgetChild_Parms, ChildIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ChildIndex_MetaData), NewProp_ChildIndex_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetPanelWidgetChild_Parms, ReturnValue), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::NewProp_Widget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::NewProp_ChildIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "GetPanelWidgetChild", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::UINavBlueprintFunctionLibrary_eventGetPanelWidgetChild_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::UINavBlueprintFunctionLibrary_eventGetPanelWidgetChild_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execGetPanelWidgetChild)
{
	P_GET_OBJECT(UWidget,Z_Param_Widget);
	P_GET_PROPERTY(FIntProperty,Z_Param_ChildIndex);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UWidget**)Z_Param__Result=UUINavBlueprintFunctionLibrary::GetPanelWidgetChild(Z_Param_Widget,Z_Param_ChildIndex);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function GetPanelWidgetChild

// Begin Class UUINavBlueprintFunctionLibrary Function GetParentPanelWidget
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics
{
	struct UINavBlueprintFunctionLibrary_eventGetParentPanelWidget_Parms
	{
		const UWidget* Widget;
		TSubclassOf<UPanelWidget> PanelWidgetSubclass;
		UPanelWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Returns the first parent of this widget thats a Panel Widget of the specified class\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Returns the first parent of this widget thats a Panel Widget of the specified class" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Widget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Widget;
	static const UECodeGen_Private::FClassPropertyParams NewProp_PanelWidgetSubclass;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::NewProp_Widget = { "Widget", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetParentPanelWidget_Parms, Widget), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Widget_MetaData), NewProp_Widget_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::NewProp_PanelWidgetSubclass = { "PanelWidgetSubclass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetParentPanelWidget_Parms, PanelWidgetSubclass), Z_Construct_UClass_UClass, Z_Construct_UClass_UPanelWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetParentPanelWidget_Parms, ReturnValue), Z_Construct_UClass_UPanelWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::NewProp_Widget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::NewProp_PanelWidgetSubclass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "GetParentPanelWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::UINavBlueprintFunctionLibrary_eventGetParentPanelWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::UINavBlueprintFunctionLibrary_eventGetParentPanelWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execGetParentPanelWidget)
{
	P_GET_OBJECT(UWidget,Z_Param_Widget);
	P_GET_OBJECT(UClass,Z_Param_PanelWidgetSubclass);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UPanelWidget**)Z_Param__Result=UUINavBlueprintFunctionLibrary::GetParentPanelWidget(Z_Param_Widget,Z_Param_PanelWidgetSubclass);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function GetParentPanelWidget

// Begin Class UUINavBlueprintFunctionLibrary Function GetPostProcessSettings
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics
{
	struct UINavBlueprintFunctionLibrary_eventGetPostProcessSettings_Parms
	{
		FString Variable;
		FString ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavigationLibrary" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Variable_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_Variable;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::NewProp_Variable = { "Variable", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetPostProcessSettings_Parms, Variable), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Variable_MetaData), NewProp_Variable_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetPostProcessSettings_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::NewProp_Variable,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "GetPostProcessSettings", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::UINavBlueprintFunctionLibrary_eventGetPostProcessSettings_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::UINavBlueprintFunctionLibrary_eventGetPostProcessSettings_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execGetPostProcessSettings)
{
	P_GET_PROPERTY(FStrProperty,Z_Param_Variable);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FString*)Z_Param__Result=UUINavBlueprintFunctionLibrary::GetPostProcessSettings(Z_Param_Variable);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function GetPostProcessSettings

// Begin Class UUINavBlueprintFunctionLibrary Function GetRawTextFromRichText
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics
{
	struct UINavBlueprintFunctionLibrary_eventGetRawTextFromRichText_Parms
	{
		FText RichText;
		FText ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavigationLibrary" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RichText_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_RichText;
	static const UECodeGen_Private::FTextPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::NewProp_RichText = { "RichText", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetRawTextFromRichText_Parms, RichText), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RichText_MetaData), NewProp_RichText_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetRawTextFromRichText_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::NewProp_RichText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "GetRawTextFromRichText", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::UINavBlueprintFunctionLibrary_eventGetRawTextFromRichText_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::UINavBlueprintFunctionLibrary_eventGetRawTextFromRichText_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execGetRawTextFromRichText)
{
	P_GET_PROPERTY_REF(FTextProperty,Z_Param_Out_RichText);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FText*)Z_Param__Result=UUINavBlueprintFunctionLibrary::GetRawTextFromRichText(Z_Param_Out_RichText);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function GetRawTextFromRichText

// Begin Class UUINavBlueprintFunctionLibrary Function GetSoundClassVolume
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics
{
	struct UINavBlueprintFunctionLibrary_eventGetSoundClassVolume_Parms
	{
		USoundClass* TargetClass;
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavigationLibrary" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TargetClass;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::NewProp_TargetClass = { "TargetClass", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetSoundClassVolume_Parms, TargetClass), Z_Construct_UClass_USoundClass_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetSoundClassVolume_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::NewProp_TargetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "GetSoundClassVolume", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::UINavBlueprintFunctionLibrary_eventGetSoundClassVolume_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::UINavBlueprintFunctionLibrary_eventGetSoundClassVolume_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execGetSoundClassVolume)
{
	P_GET_OBJECT(USoundClass,Z_Param_TargetClass);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UUINavBlueprintFunctionLibrary::GetSoundClassVolume(Z_Param_TargetClass);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function GetSoundClassVolume

// Begin Class UUINavBlueprintFunctionLibrary Function GetUINavSettings
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics
{
	struct UINavBlueprintFunctionLibrary_eventGetUINavSettings_Parms
	{
		const UUINavSettings* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000582, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetUINavSettings_Parms, ReturnValue), Z_Construct_UClass_UUINavSettings_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "GetUINavSettings", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::UINavBlueprintFunctionLibrary_eventGetUINavSettings_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::UINavBlueprintFunctionLibrary_eventGetUINavSettings_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execGetUINavSettings)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(const UUINavSettings**)Z_Param__Result=UUINavBlueprintFunctionLibrary::GetUINavSettings();
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function GetUINavSettings

// Begin Class UUINavBlueprintFunctionLibrary Function GetUniformGridChild
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics
{
	struct UINavBlueprintFunctionLibrary_eventGetUniformGridChild_Parms
	{
		const UWidget* Widget;
		int32 Column;
		int32 Row;
		UWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Returns the child of the given uniform grid panel widget with the given column and row\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Returns the child of the given uniform grid panel widget with the given column and row" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Widget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Column_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Row_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Widget;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Column;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Row;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::NewProp_Widget = { "Widget", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetUniformGridChild_Parms, Widget), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Widget_MetaData), NewProp_Widget_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::NewProp_Column = { "Column", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetUniformGridChild_Parms, Column), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Column_MetaData), NewProp_Column_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::NewProp_Row = { "Row", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetUniformGridChild_Parms, Row), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Row_MetaData), NewProp_Row_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventGetUniformGridChild_Parms, ReturnValue), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::NewProp_Widget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::NewProp_Column,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::NewProp_Row,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "GetUniformGridChild", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::UINavBlueprintFunctionLibrary_eventGetUniformGridChild_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::UINavBlueprintFunctionLibrary_eventGetUniformGridChild_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execGetUniformGridChild)
{
	P_GET_OBJECT(UWidget,Z_Param_Widget);
	P_GET_PROPERTY(FIntProperty,Z_Param_Column);
	P_GET_PROPERTY(FIntProperty,Z_Param_Row);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UWidget**)Z_Param__Result=UUINavBlueprintFunctionLibrary::GetUniformGridChild(Z_Param_Widget,Z_Param_Column,Z_Param_Row);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function GetUniformGridChild

// Begin Class UUINavBlueprintFunctionLibrary Function IsGamepadConnected
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics
{
	struct UINavBlueprintFunctionLibrary_eventIsGamepadConnected_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavigationLibrary" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Checks whether a gamepad is connected\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Checks whether a gamepad is connected" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavBlueprintFunctionLibrary_eventIsGamepadConnected_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavBlueprintFunctionLibrary_eventIsGamepadConnected_Parms), &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "IsGamepadConnected", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::UINavBlueprintFunctionLibrary_eventIsGamepadConnected_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::UINavBlueprintFunctionLibrary_eventIsGamepadConnected_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execIsGamepadConnected)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UUINavBlueprintFunctionLibrary::IsGamepadConnected();
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function IsGamepadConnected

// Begin Class UUINavBlueprintFunctionLibrary Function IsKeyInCategory
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics
{
	struct UINavBlueprintFunctionLibrary_eventIsKeyInCategory_Parms
	{
		FKey Key;
		FString Category;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavigationLibrary" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Category_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Category;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventIsKeyInCategory_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::NewProp_Category = { "Category", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventIsKeyInCategory_Parms, Category), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Category_MetaData), NewProp_Category_MetaData) };
void Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavBlueprintFunctionLibrary_eventIsKeyInCategory_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavBlueprintFunctionLibrary_eventIsKeyInCategory_Parms), &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::NewProp_Category,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "IsKeyInCategory", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::UINavBlueprintFunctionLibrary_eventIsKeyInCategory_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::UINavBlueprintFunctionLibrary_eventIsKeyInCategory_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execIsKeyInCategory)
{
	P_GET_STRUCT(FKey,Z_Param_Key);
	P_GET_PROPERTY(FStrProperty,Z_Param_Category);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UUINavBlueprintFunctionLibrary::IsKeyInCategory(Z_Param_Key,Z_Param_Category);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function IsKeyInCategory

// Begin Class UUINavBlueprintFunctionLibrary Function IsUINavInputAction
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics
{
	struct UINavBlueprintFunctionLibrary_eventIsUINavInputAction_Parms
	{
		const UInputAction* InputAction;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavInput" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputAction_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputAction;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::NewProp_InputAction = { "InputAction", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventIsUINavInputAction_Parms, InputAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputAction_MetaData), NewProp_InputAction_MetaData) };
void Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavBlueprintFunctionLibrary_eventIsUINavInputAction_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavBlueprintFunctionLibrary_eventIsUINavInputAction_Parms), &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::NewProp_InputAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "IsUINavInputAction", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::UINavBlueprintFunctionLibrary_eventIsUINavInputAction_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::UINavBlueprintFunctionLibrary_eventIsUINavInputAction_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execIsUINavInputAction)
{
	P_GET_OBJECT(UInputAction,Z_Param_InputAction);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UUINavBlueprintFunctionLibrary::IsUINavInputAction(Z_Param_InputAction);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function IsUINavInputAction

// Begin Class UUINavBlueprintFunctionLibrary Function IsVRKey
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics
{
	struct UINavBlueprintFunctionLibrary_eventIsVRKey_Parms
	{
		FKey Key;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavigationLibrary" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventIsVRKey_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
void Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavBlueprintFunctionLibrary_eventIsVRKey_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavBlueprintFunctionLibrary_eventIsVRKey_Parms), &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "IsVRKey", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::UINavBlueprintFunctionLibrary_eventIsVRKey_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::UINavBlueprintFunctionLibrary_eventIsVRKey_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execIsVRKey)
{
	P_GET_STRUCT(FKey,Z_Param_Key);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UUINavBlueprintFunctionLibrary::IsVRKey(Z_Param_Key);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function IsVRKey

// Begin Class UUINavBlueprintFunctionLibrary Function ResetInputSettings
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics
{
	struct UINavBlueprintFunctionLibrary_eventResetInputSettings_Parms
	{
		APlayerController* PC;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavInput" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Resets the input settings to their default state\n" },
#endif
		{ "CPP_Default_PC", "None" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Resets the input settings to their default state" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PC;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::NewProp_PC = { "PC", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventResetInputSettings_Parms, PC), Z_Construct_UClass_APlayerController_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::NewProp_PC,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "ResetInputSettings", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::UINavBlueprintFunctionLibrary_eventResetInputSettings_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::UINavBlueprintFunctionLibrary_eventResetInputSettings_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execResetInputSettings)
{
	P_GET_OBJECT(APlayerController,Z_Param_PC);
	P_FINISH;
	P_NATIVE_BEGIN;
	UUINavBlueprintFunctionLibrary::ResetInputSettings(Z_Param_PC);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function ResetInputSettings

// Begin Class UUINavBlueprintFunctionLibrary Function RespectsRestriction
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics
{
	struct UINavBlueprintFunctionLibrary_eventRespectsRestriction_Parms
	{
		FKey CompareKey;
		EInputRestriction Restriction;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINav Input" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CompareKey_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Restriction_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_CompareKey;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Restriction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Restriction;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::NewProp_CompareKey = { "CompareKey", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventRespectsRestriction_Parms, CompareKey), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CompareKey_MetaData), NewProp_CompareKey_MetaData) }; // 658672854
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::NewProp_Restriction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::NewProp_Restriction = { "Restriction", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventRespectsRestriction_Parms, Restriction), Z_Construct_UEnum_UINavigation_EInputRestriction, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Restriction_MetaData), NewProp_Restriction_MetaData) }; // 2300298404
void Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavBlueprintFunctionLibrary_eventRespectsRestriction_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavBlueprintFunctionLibrary_eventRespectsRestriction_Parms), &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::NewProp_CompareKey,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::NewProp_Restriction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::NewProp_Restriction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "RespectsRestriction", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::UINavBlueprintFunctionLibrary_eventRespectsRestriction_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::UINavBlueprintFunctionLibrary_eventRespectsRestriction_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execRespectsRestriction)
{
	P_GET_STRUCT(FKey,Z_Param_CompareKey);
	P_GET_ENUM(EInputRestriction,Z_Param_Restriction);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UUINavBlueprintFunctionLibrary::RespectsRestriction(Z_Param_CompareKey,EInputRestriction(Z_Param_Restriction));
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function RespectsRestriction

// Begin Class UUINavBlueprintFunctionLibrary Function SetPostProcessSettings
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics
{
	struct UINavBlueprintFunctionLibrary_eventSetPostProcessSettings_Parms
	{
		FString Variable;
		FString Value;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavigationLibrary" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Variable_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Value_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_Variable;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Value;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::NewProp_Variable = { "Variable", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventSetPostProcessSettings_Parms, Variable), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Variable_MetaData), NewProp_Variable_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventSetPostProcessSettings_Parms, Value), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Value_MetaData), NewProp_Value_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::NewProp_Variable,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::NewProp_Value,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "SetPostProcessSettings", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::UINavBlueprintFunctionLibrary_eventSetPostProcessSettings_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::UINavBlueprintFunctionLibrary_eventSetPostProcessSettings_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execSetPostProcessSettings)
{
	P_GET_PROPERTY(FStrProperty,Z_Param_Variable);
	P_GET_PROPERTY(FStrProperty,Z_Param_Value);
	P_FINISH;
	P_NATIVE_BEGIN;
	UUINavBlueprintFunctionLibrary::SetPostProcessSettings(Z_Param_Variable,Z_Param_Value);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function SetPostProcessSettings

// Begin Class UUINavBlueprintFunctionLibrary Function SetSoundClassVolume
struct Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics
{
	struct UINavBlueprintFunctionLibrary_eventSetSoundClassVolume_Parms
	{
		USoundClass* TargetClass;
		float NewVolume;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavigationLibrary" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewVolume_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TargetClass;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_NewVolume;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::NewProp_TargetClass = { "TargetClass", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventSetSoundClassVolume_Parms, TargetClass), Z_Construct_UClass_USoundClass_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::NewProp_NewVolume = { "NewVolume", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavBlueprintFunctionLibrary_eventSetSoundClassVolume_Parms, NewVolume), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewVolume_MetaData), NewProp_NewVolume_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::NewProp_TargetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::NewProp_NewVolume,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavBlueprintFunctionLibrary, nullptr, "SetSoundClassVolume", nullptr, nullptr, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::UINavBlueprintFunctionLibrary_eventSetSoundClassVolume_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::UINavBlueprintFunctionLibrary_eventSetSoundClassVolume_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavBlueprintFunctionLibrary::execSetSoundClassVolume)
{
	P_GET_OBJECT(USoundClass,Z_Param_TargetClass);
	P_GET_PROPERTY(FFloatProperty,Z_Param_NewVolume);
	P_FINISH;
	P_NATIVE_BEGIN;
	UUINavBlueprintFunctionLibrary::SetSoundClassVolume(Z_Param_TargetClass,Z_Param_NewVolume);
	P_NATIVE_END;
}
// End Class UUINavBlueprintFunctionLibrary Function SetSoundClassVolume

// Begin Class UUINavBlueprintFunctionLibrary
void UUINavBlueprintFunctionLibrary::StaticRegisterNativesUUINavBlueprintFunctionLibrary()
{
	UClass* Class = UUINavBlueprintFunctionLibrary::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "ApplyStyleRowToText", &UUINavBlueprintFunctionLibrary::execApplyStyleRowToText },
		{ "CreateBinaryPromptData", &UUINavBlueprintFunctionLibrary::execCreateBinaryPromptData },
		{ "FindWidgetOfClassesInWidget", &UUINavBlueprintFunctionLibrary::execFindWidgetOfClassesInWidget },
		{ "GetFirstWidgetInUserWidget", &UUINavBlueprintFunctionLibrary::execGetFirstWidgetInUserWidget },
		{ "GetIndexInPanelWidget", &UUINavBlueprintFunctionLibrary::execGetIndexInPanelWidget },
		{ "GetIndexInUniformGridWidget", &UUINavBlueprintFunctionLibrary::execGetIndexInUniformGridWidget },
		{ "GetPanelWidgetChild", &UUINavBlueprintFunctionLibrary::execGetPanelWidgetChild },
		{ "GetParentPanelWidget", &UUINavBlueprintFunctionLibrary::execGetParentPanelWidget },
		{ "GetPostProcessSettings", &UUINavBlueprintFunctionLibrary::execGetPostProcessSettings },
		{ "GetRawTextFromRichText", &UUINavBlueprintFunctionLibrary::execGetRawTextFromRichText },
		{ "GetSoundClassVolume", &UUINavBlueprintFunctionLibrary::execGetSoundClassVolume },
		{ "GetUINavSettings", &UUINavBlueprintFunctionLibrary::execGetUINavSettings },
		{ "GetUniformGridChild", &UUINavBlueprintFunctionLibrary::execGetUniformGridChild },
		{ "IsGamepadConnected", &UUINavBlueprintFunctionLibrary::execIsGamepadConnected },
		{ "IsKeyInCategory", &UUINavBlueprintFunctionLibrary::execIsKeyInCategory },
		{ "IsUINavInputAction", &UUINavBlueprintFunctionLibrary::execIsUINavInputAction },
		{ "IsVRKey", &UUINavBlueprintFunctionLibrary::execIsVRKey },
		{ "ResetInputSettings", &UUINavBlueprintFunctionLibrary::execResetInputSettings },
		{ "RespectsRestriction", &UUINavBlueprintFunctionLibrary::execRespectsRestriction },
		{ "SetPostProcessSettings", &UUINavBlueprintFunctionLibrary::execSetPostProcessSettings },
		{ "SetSoundClassVolume", &UUINavBlueprintFunctionLibrary::execSetSoundClassVolume },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavBlueprintFunctionLibrary);
UClass* Z_Construct_UClass_UUINavBlueprintFunctionLibrary_NoRegister()
{
	return UUINavBlueprintFunctionLibrary::StaticClass();
}
struct Z_Construct_UClass_UUINavBlueprintFunctionLibrary_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UINavBlueprintFunctionLibrary.h" },
		{ "ModuleRelativePath", "Public/UINavBlueprintFunctionLibrary.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ApplyStyleRowToText, "ApplyStyleRowToText" }, // 220902835
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_CreateBinaryPromptData, "CreateBinaryPromptData" }, // 2606002634
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_FindWidgetOfClassesInWidget, "FindWidgetOfClassesInWidget" }, // 1734862047
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetFirstWidgetInUserWidget, "GetFirstWidgetInUserWidget" }, // 717360270
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInPanelWidget, "GetIndexInPanelWidget" }, // 2836456681
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetIndexInUniformGridWidget, "GetIndexInUniformGridWidget" }, // 3841234311
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPanelWidgetChild, "GetPanelWidgetChild" }, // 2952974664
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetParentPanelWidget, "GetParentPanelWidget" }, // 1601815198
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetPostProcessSettings, "GetPostProcessSettings" }, // 1303727971
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetRawTextFromRichText, "GetRawTextFromRichText" }, // 3743870526
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetSoundClassVolume, "GetSoundClassVolume" }, // 2768785908
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUINavSettings, "GetUINavSettings" }, // 1419530153
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_GetUniformGridChild, "GetUniformGridChild" }, // 31954390
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsGamepadConnected, "IsGamepadConnected" }, // 4133761219
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsKeyInCategory, "IsKeyInCategory" }, // 2107903317
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsUINavInputAction, "IsUINavInputAction" }, // 4120578435
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_IsVRKey, "IsVRKey" }, // 3191291696
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_ResetInputSettings, "ResetInputSettings" }, // 1128302721
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_RespectsRestriction, "RespectsRestriction" }, // 3579347220
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetPostProcessSettings, "SetPostProcessSettings" }, // 2416303804
		{ &Z_Construct_UFunction_UUINavBlueprintFunctionLibrary_SetSoundClassVolume, "SetSoundClassVolume" }, // 4226907139
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavBlueprintFunctionLibrary>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UUINavBlueprintFunctionLibrary_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavBlueprintFunctionLibrary_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavBlueprintFunctionLibrary_Statics::ClassParams = {
	&UUINavBlueprintFunctionLibrary::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavBlueprintFunctionLibrary_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavBlueprintFunctionLibrary_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavBlueprintFunctionLibrary()
{
	if (!Z_Registration_Info_UClass_UUINavBlueprintFunctionLibrary.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavBlueprintFunctionLibrary.OuterSingleton, Z_Construct_UClass_UUINavBlueprintFunctionLibrary_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavBlueprintFunctionLibrary.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavBlueprintFunctionLibrary>()
{
	return UUINavBlueprintFunctionLibrary::StaticClass();
}
UUINavBlueprintFunctionLibrary::UUINavBlueprintFunctionLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavBlueprintFunctionLibrary);
UUINavBlueprintFunctionLibrary::~UUINavBlueprintFunctionLibrary() {}
// End Class UUINavBlueprintFunctionLibrary

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavBlueprintFunctionLibrary, UUINavBlueprintFunctionLibrary::StaticClass, TEXT("UUINavBlueprintFunctionLibrary"), &Z_Registration_Info_UClass_UUINavBlueprintFunctionLibrary, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavBlueprintFunctionLibrary), 3964910703U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_2590321635(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
