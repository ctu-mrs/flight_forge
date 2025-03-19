// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavWidget.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavWidget() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
ENGINE_API UClass* Z_Construct_UClass_UCurveFloat_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavComponent_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPCComponent_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPromptWidget_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavWidget();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavWidget_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavWidgetComponent_NoRegister();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputType();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_ESelectorPosition();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EThumbstickAsMouse();
UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature();
UMG_API UClass* Z_Construct_UClass_UScrollBox_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UMG_API UClass* Z_Construct_UClass_UUserWidget_NoRegister();
UMG_API UClass* Z_Construct_UClass_UWidget_NoRegister();
UMG_API UClass* Z_Construct_UClass_UWidgetSwitcher_NoRegister();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavWidget Function ConfigureUINavPC
struct Z_Construct_UFunction_UUINavWidget_ConfigureUINavPC_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Configures the UINavPC\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Configures the UINavPC" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_ConfigureUINavPC_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "ConfigureUINavPC", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_ConfigureUINavPC_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_ConfigureUINavPC_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_ConfigureUINavPC()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_ConfigureUINavPC_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execConfigureUINavPC)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ConfigureUINavPC();
	P_NATIVE_END;
}
// End Class UUINavWidget Function ConfigureUINavPC

// Begin Class UUINavWidget Function GetInitialFocusComponent
struct UINavWidget_eventGetInitialFocusComponent_Parms
{
	UUINavComponent* ReturnValue;

	/** Constructor, initializes return property only **/
	UINavWidget_eventGetInitialFocusComponent_Parms()
		: ReturnValue(NULL)
	{
	}
};
static FName NAME_UUINavWidget_GetInitialFocusComponent = FName(TEXT("GetInitialFocusComponent"));
UUINavComponent* UUINavWidget::GetInitialFocusComponent()
{
	UINavWidget_eventGetInitialFocusComponent_Parms Parms;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_GetInitialFocusComponent),&Parms);
	return Parms.ReturnValue;
}
struct Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGetInitialFocusComponent_Parms, ReturnValue), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "GetInitialFocusComponent", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics::PropPointers), sizeof(UINavWidget_eventGetInitialFocusComponent_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventGetInitialFocusComponent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execGetInitialFocusComponent)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UUINavComponent**)Z_Param__Result=P_THIS->GetInitialFocusComponent_Implementation();
	P_NATIVE_END;
}
// End Class UUINavWidget Function GetInitialFocusComponent

// Begin Class UUINavWidget Function GetMostOuterUINavWidget
struct Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics
{
	struct UINavWidget_eventGetMostOuterUINavWidget_Parms
	{
		UUINavWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGetMostOuterUINavWidget_Parms, ReturnValue), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "GetMostOuterUINavWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::UINavWidget_eventGetMostOuterUINavWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::UINavWidget_eventGetMostOuterUINavWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execGetMostOuterUINavWidget)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UUINavWidget**)Z_Param__Result=P_THIS->GetMostOuterUINavWidget();
	P_NATIVE_END;
}
// End Class UUINavWidget Function GetMostOuterUINavWidget

// Begin Class UUINavWidget Function GetScrollBoxToFocus
struct UINavWidget_eventGetScrollBoxToFocus_Parms
{
	UScrollBox* ReturnValue;

	/** Constructor, initializes return property only **/
	UINavWidget_eventGetScrollBoxToFocus_Parms()
		: ReturnValue(NULL)
	{
	}
};
static FName NAME_UUINavWidget_GetScrollBoxToFocus = FName(TEXT("GetScrollBoxToFocus"));
UScrollBox* UUINavWidget::GetScrollBoxToFocus()
{
	UINavWidget_eventGetScrollBoxToFocus_Parms Parms;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_GetScrollBoxToFocus),&Parms);
	return Parms.ReturnValue;
}
struct Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGetScrollBoxToFocus_Parms, ReturnValue), Z_Construct_UClass_UScrollBox_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "GetScrollBoxToFocus", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics::PropPointers), sizeof(UINavWidget_eventGetScrollBoxToFocus_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x1C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventGetScrollBoxToFocus_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execGetScrollBoxToFocus)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UScrollBox**)Z_Param__Result=P_THIS->GetScrollBoxToFocus_Implementation();
	P_NATIVE_END;
}
// End Class UUINavWidget Function GetScrollBoxToFocus

// Begin Class UUINavWidget Function GoToBuiltWidget
struct Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics
{
	struct UINavWidget_eventGoToBuiltWidget_Parms
	{
		UUINavWidget* NewWidget;
		bool bRemoveParent;
		bool bDestroyParent;
		int32 ZOrder;
		UUINavWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "AdvancedDisplay", "2" },
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Adds given widget to screen (strongly recommended over manual alternative)\n\x09*\n\x09*\x09@param\x09NewWidget  Object instance of the UINavWidget to add to the screen\n\x09*\x09@param\x09""bRemoveParent  Whether to remove the parent widget (this widget) from the viewport\n\x09*\x09@param  bDestroyParent  Whether to destruct the parent widget (this widget)\n\x09*\x09@param  ZOrder Order to display the widget\n\x09*/" },
#endif
		{ "CPP_Default_bDestroyParent", "false" },
		{ "CPP_Default_ZOrder", "0" },
		{ "DeterminesOutputType", "NewWidgetClass" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Adds given widget to screen (strongly recommended over manual alternative)\n\n@param  NewWidget  Object instance of the UINavWidget to add to the screen\n@param  bRemoveParent  Whether to remove the parent widget (this widget) from the viewport\n@param  bDestroyParent  Whether to destruct the parent widget (this widget)\n@param  ZOrder Order to display the widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRemoveParent_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bDestroyParent_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ZOrder_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewWidget;
	static void NewProp_bRemoveParent_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRemoveParent;
	static void NewProp_bDestroyParent_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bDestroyParent;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ZOrder;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_NewWidget = { "NewWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToBuiltWidget_Parms, NewWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewWidget_MetaData), NewProp_NewWidget_MetaData) };
void Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_bRemoveParent_SetBit(void* Obj)
{
	((UINavWidget_eventGoToBuiltWidget_Parms*)Obj)->bRemoveParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_bRemoveParent = { "bRemoveParent", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventGoToBuiltWidget_Parms), &Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_bRemoveParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRemoveParent_MetaData), NewProp_bRemoveParent_MetaData) };
void Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_bDestroyParent_SetBit(void* Obj)
{
	((UINavWidget_eventGoToBuiltWidget_Parms*)Obj)->bDestroyParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_bDestroyParent = { "bDestroyParent", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventGoToBuiltWidget_Parms), &Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_bDestroyParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDestroyParent_MetaData), NewProp_bDestroyParent_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_ZOrder = { "ZOrder", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToBuiltWidget_Parms, ZOrder), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ZOrder_MetaData), NewProp_ZOrder_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToBuiltWidget_Parms, ReturnValue), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_NewWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_bRemoveParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_bDestroyParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_ZOrder,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "GoToBuiltWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::UINavWidget_eventGoToBuiltWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::UINavWidget_eventGoToBuiltWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execGoToBuiltWidget)
{
	P_GET_OBJECT(UUINavWidget,Z_Param_NewWidget);
	P_GET_UBOOL(Z_Param_bRemoveParent);
	P_GET_UBOOL(Z_Param_bDestroyParent);
	P_GET_PROPERTY(FIntProperty,Z_Param_ZOrder);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UUINavWidget**)Z_Param__Result=P_THIS->GoToBuiltWidget(Z_Param_NewWidget,Z_Param_bRemoveParent,Z_Param_bDestroyParent,Z_Param_ZOrder);
	P_NATIVE_END;
}
// End Class UUINavWidget Function GoToBuiltWidget

// Begin Class UUINavWidget Function GoToNextSection
struct Z_Construct_UFunction_UUINavWidget_GoToNextSection_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_GoToNextSection_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "GoToNextSection", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToNextSection_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_GoToNextSection_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_GoToNextSection()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_GoToNextSection_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execGoToNextSection)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->GoToNextSection();
	P_NATIVE_END;
}
// End Class UUINavWidget Function GoToNextSection

// Begin Class UUINavWidget Function GoToPreviousSection
struct Z_Construct_UFunction_UUINavWidget_GoToPreviousSection_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_GoToPreviousSection_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "GoToPreviousSection", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToPreviousSection_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_GoToPreviousSection_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_GoToPreviousSection()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_GoToPreviousSection_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execGoToPreviousSection)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->GoToPreviousSection();
	P_NATIVE_END;
}
// End Class UUINavWidget Function GoToPreviousSection

// Begin Class UUINavWidget Function GoToPromptWidget
struct Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics
{
	struct UINavWidget_eventGoToPromptWidget_Parms
	{
		TSubclassOf<UUINavPromptWidget> NewWidgetClass;
		FScriptDelegate Event;
		FText Title;
		FText Message;
		bool bRemoveParent;
		int32 ZOrder;
		UUINavWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "AdvancedDisplay", "4" },
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Adds given widget to screen (strongly recommended over manual alternative)\n\x09*\n\x09*\x09@param\x09NewWidgetClass  The class of the widget to add to the screen\n\x09*\x09@param\x09""bRemoveParent  Whether to remove the parent widget (this widget) from the viewport\n\x09*\x09@param  bDestroyParent  Whether to destruct the parent widget (this widget)\n\x09*\x09@param  ZOrder Order to display the widget\n\x09*/" },
#endif
		{ "CPP_Default_bRemoveParent", "false" },
		{ "CPP_Default_Message", "" },
		{ "CPP_Default_Title", "" },
		{ "CPP_Default_ZOrder", "0" },
		{ "DeterminesOutputType", "NewWidgetClass" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Adds given widget to screen (strongly recommended over manual alternative)\n\n@param  NewWidgetClass  The class of the widget to add to the screen\n@param  bRemoveParent  Whether to remove the parent widget (this widget) from the viewport\n@param  bDestroyParent  Whether to destruct the parent widget (this widget)\n@param  ZOrder Order to display the widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Event_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Title_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Message_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRemoveParent_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ZOrder_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_NewWidgetClass;
	static const UECodeGen_Private::FDelegatePropertyParams NewProp_Event;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Title;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Message;
	static void NewProp_bRemoveParent_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRemoveParent;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ZOrder;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_NewWidgetClass = { "NewWidgetClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToPromptWidget_Parms, NewWidgetClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UUINavPromptWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FDelegatePropertyParams Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_Event = { "Event", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Delegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToPromptWidget_Parms, Event), Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Event_MetaData), NewProp_Event_MetaData) }; // 68500594
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_Title = { "Title", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToPromptWidget_Parms, Title), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Title_MetaData), NewProp_Title_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_Message = { "Message", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToPromptWidget_Parms, Message), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Message_MetaData), NewProp_Message_MetaData) };
void Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_bRemoveParent_SetBit(void* Obj)
{
	((UINavWidget_eventGoToPromptWidget_Parms*)Obj)->bRemoveParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_bRemoveParent = { "bRemoveParent", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventGoToPromptWidget_Parms), &Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_bRemoveParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRemoveParent_MetaData), NewProp_bRemoveParent_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_ZOrder = { "ZOrder", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToPromptWidget_Parms, ZOrder), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ZOrder_MetaData), NewProp_ZOrder_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToPromptWidget_Parms, ReturnValue), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_NewWidgetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_Event,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_Title,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_Message,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_bRemoveParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_ZOrder,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "GoToPromptWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::UINavWidget_eventGoToPromptWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::UINavWidget_eventGoToPromptWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_GoToPromptWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_GoToPromptWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execGoToPromptWidget)
{
	P_GET_OBJECT(UClass,Z_Param_NewWidgetClass);
	P_GET_PROPERTY_REF(FDelegateProperty,Z_Param_Out_Event);
	P_GET_PROPERTY(FTextProperty,Z_Param_Title);
	P_GET_PROPERTY(FTextProperty,Z_Param_Message);
	P_GET_UBOOL(Z_Param_bRemoveParent);
	P_GET_PROPERTY(FIntProperty,Z_Param_ZOrder);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UUINavWidget**)Z_Param__Result=P_THIS->GoToPromptWidget(Z_Param_NewWidgetClass,FPromptWidgetDecided(Z_Param_Out_Event),Z_Param_Title,Z_Param_Message,Z_Param_bRemoveParent,Z_Param_ZOrder);
	P_NATIVE_END;
}
// End Class UUINavWidget Function GoToPromptWidget

// Begin Class UUINavWidget Function GoToSection
struct Z_Construct_UFunction_UUINavWidget_GoToSection_Statics
{
	struct UINavWidget_eventGoToSection_Parms
	{
		int32 SectionIndex;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SectionIndex_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_SectionIndex;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::NewProp_SectionIndex = { "SectionIndex", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToSection_Parms, SectionIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SectionIndex_MetaData), NewProp_SectionIndex_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::NewProp_SectionIndex,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "GoToSection", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::UINavWidget_eventGoToSection_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::UINavWidget_eventGoToSection_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_GoToSection()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_GoToSection_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execGoToSection)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_SectionIndex);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->GoToSection(Z_Param_SectionIndex);
	P_NATIVE_END;
}
// End Class UUINavWidget Function GoToSection

// Begin Class UUINavWidget Function GoToWidget
struct Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics
{
	struct UINavWidget_eventGoToWidget_Parms
	{
		TSubclassOf<UUINavWidget> NewWidgetClass;
		bool bRemoveParent;
		bool bDestroyParent;
		int32 ZOrder;
		UUINavWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "AdvancedDisplay", "2" },
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Adds given widget to screen (strongly recommended over manual alternative)\n\x09*\n\x09*\x09@param\x09NewWidgetClass  The class of the widget to add to the screen\n\x09*\x09@param\x09""bRemoveParent  Whether to remove the parent widget (this widget) from the viewport\n\x09*\x09@param  bDestroyParent  Whether to destruct the parent widget (this widget)\n\x09*\x09@param  ZOrder Order to display the widget\n\x09*/" },
#endif
		{ "CPP_Default_bDestroyParent", "false" },
		{ "CPP_Default_bRemoveParent", "true" },
		{ "CPP_Default_ZOrder", "0" },
		{ "DeterminesOutputType", "NewWidgetClass" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Adds given widget to screen (strongly recommended over manual alternative)\n\n@param  NewWidgetClass  The class of the widget to add to the screen\n@param  bRemoveParent  Whether to remove the parent widget (this widget) from the viewport\n@param  bDestroyParent  Whether to destruct the parent widget (this widget)\n@param  ZOrder Order to display the widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRemoveParent_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bDestroyParent_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ZOrder_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FClassPropertyParams NewProp_NewWidgetClass;
	static void NewProp_bRemoveParent_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRemoveParent;
	static void NewProp_bDestroyParent_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bDestroyParent;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ZOrder;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_NewWidgetClass = { "NewWidgetClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToWidget_Parms, NewWidgetClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_bRemoveParent_SetBit(void* Obj)
{
	((UINavWidget_eventGoToWidget_Parms*)Obj)->bRemoveParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_bRemoveParent = { "bRemoveParent", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventGoToWidget_Parms), &Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_bRemoveParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRemoveParent_MetaData), NewProp_bRemoveParent_MetaData) };
void Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_bDestroyParent_SetBit(void* Obj)
{
	((UINavWidget_eventGoToWidget_Parms*)Obj)->bDestroyParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_bDestroyParent = { "bDestroyParent", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventGoToWidget_Parms), &Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_bDestroyParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDestroyParent_MetaData), NewProp_bDestroyParent_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_ZOrder = { "ZOrder", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToWidget_Parms, ZOrder), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ZOrder_MetaData), NewProp_ZOrder_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventGoToWidget_Parms, ReturnValue), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_NewWidgetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_bRemoveParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_bDestroyParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_ZOrder,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "GoToWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::UINavWidget_eventGoToWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::UINavWidget_eventGoToWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_GoToWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_GoToWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execGoToWidget)
{
	P_GET_OBJECT(UClass,Z_Param_NewWidgetClass);
	P_GET_UBOOL(Z_Param_bRemoveParent);
	P_GET_UBOOL(Z_Param_bDestroyParent);
	P_GET_PROPERTY(FIntProperty,Z_Param_ZOrder);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UUINavWidget**)Z_Param__Result=P_THIS->GoToWidget(Z_Param_NewWidgetClass,Z_Param_bRemoveParent,Z_Param_bDestroyParent,Z_Param_ZOrder);
	P_NATIVE_END;
}
// End Class UUINavWidget Function GoToWidget

// Begin Class UUINavWidget Function IsRebindingInput
struct Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics
{
	struct UINavWidget_eventIsRebindingInput_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavWidget_eventIsRebindingInput_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventIsRebindingInput_Parms), &Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "IsRebindingInput", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::UINavWidget_eventIsRebindingInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::UINavWidget_eventIsRebindingInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_IsRebindingInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_IsRebindingInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execIsRebindingInput)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsRebindingInput();
	P_NATIVE_END;
}
// End Class UUINavWidget Function IsRebindingInput

// Begin Class UUINavWidget Function IsSelectorValid
struct Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics
{
	struct UINavWidget_eventIsSelectorValid_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavWidget_eventIsSelectorValid_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventIsSelectorValid_Parms), &Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "IsSelectorValid", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::UINavWidget_eventIsSelectorValid_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::UINavWidget_eventIsSelectorValid_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_IsSelectorValid()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_IsSelectorValid_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execIsSelectorValid)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsSelectorValid();
	P_NATIVE_END;
}
// End Class UUINavWidget Function IsSelectorValid

// Begin Class UUINavWidget Function IsSelectorVisible
struct Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics
{
	struct UINavWidget_eventIsSelectorVisible_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavWidget_eventIsSelectorVisible_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventIsSelectorVisible_Parms), &Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "IsSelectorVisible", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::UINavWidget_eventIsSelectorVisible_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::UINavWidget_eventIsSelectorVisible_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_IsSelectorVisible()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_IsSelectorVisible_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execIsSelectorVisible)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsSelectorVisible();
	P_NATIVE_END;
}
// End Class UUINavWidget Function IsSelectorVisible

// Begin Class UUINavWidget Function OnChangedSection
struct UINavWidget_eventOnChangedSection_Parms
{
	int32 FromIndex;
	int32 ToIndex;
};
static FName NAME_UUINavWidget_OnChangedSection = FName(TEXT("OnChangedSection"));
void UUINavWidget::OnChangedSection(const int32 FromIndex, const int32 ToIndex)
{
	UINavWidget_eventOnChangedSection_Parms Parms;
	Parms.FromIndex=FromIndex;
	Parms.ToIndex=ToIndex;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnChangedSection),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when player changes section, either through OnNext/OnPrevious or by pressing the corresponding section button\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when player changes section, either through OnNext/OnPrevious or by pressing the corresponding section button" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FromIndex_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ToIndex_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_FromIndex;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ToIndex;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::NewProp_FromIndex = { "FromIndex", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnChangedSection_Parms, FromIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FromIndex_MetaData), NewProp_FromIndex_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::NewProp_ToIndex = { "ToIndex", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnChangedSection_Parms, ToIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ToIndex_MetaData), NewProp_ToIndex_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::NewProp_FromIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::NewProp_ToIndex,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnChangedSection", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::PropPointers), sizeof(UINavWidget_eventOnChangedSection_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnChangedSection_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnChangedSection()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnChangedSection_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnChangedSection)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_FromIndex);
	P_GET_PROPERTY(FIntProperty,Z_Param_ToIndex);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnChangedSection_Implementation(Z_Param_FromIndex,Z_Param_ToIndex);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnChangedSection

// Begin Class UUINavWidget Function OnChildReturn
struct UINavWidget_eventOnChildReturn_Parms
{
	bool ReturnValue;

	/** Constructor, initializes return property only **/
	UINavWidget_eventOnChildReturn_Parms()
		: ReturnValue(false)
	{
	}
};
static FName NAME_UUINavWidget_OnChildReturn = FName(TEXT("OnChildReturn"));
bool UUINavWidget::OnChildReturn()
{
	UINavWidget_eventOnChildReturn_Parms Parms;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnChildReturn),&Parms);
	return !!Parms.ReturnValue;
}
struct Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when ReturnToParent is called (i.e. the player wants to exit the menu), only in child nested UINavWidgets.\n\x09*\x09Returns whether the Parent Widget's OnReturn/OnChildReturn should be called;\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when ReturnToParent is called (i.e. the player wants to exit the menu), only in child nested UINavWidgets.\nReturns whether the Parent Widget's OnReturn/OnChildReturn should be called;" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavWidget_eventOnChildReturn_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventOnChildReturn_Parms), &Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnChildReturn", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::PropPointers), sizeof(UINavWidget_eventOnChildReturn_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnChildReturn_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnChildReturn()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnChildReturn_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnChildReturn)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->OnChildReturn_Implementation();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnChildReturn

// Begin Class UUINavWidget Function OnGainedNavigation
struct UINavWidget_eventOnGainedNavigation_Parms
{
	UUINavWidget* PreviousActiveWidget;
	bool bFromChild;
};
static FName NAME_UUINavWidget_OnGainedNavigation = FName(TEXT("OnGainedNavigation"));
void UUINavWidget::OnGainedNavigation(UUINavWidget* PreviousActiveWidget, bool bFromChild)
{
	UINavWidget_eventOnGainedNavigation_Parms Parms;
	Parms.PreviousActiveWidget=PreviousActiveWidget;
	Parms.bFromChild=bFromChild ? true : false;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnGainedNavigation),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when navigation is gained\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when navigation is gained" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PreviousActiveWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bFromChild_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PreviousActiveWidget;
	static void NewProp_bFromChild_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bFromChild;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::NewProp_PreviousActiveWidget = { "PreviousActiveWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnGainedNavigation_Parms, PreviousActiveWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PreviousActiveWidget_MetaData), NewProp_PreviousActiveWidget_MetaData) };
void Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::NewProp_bFromChild_SetBit(void* Obj)
{
	((UINavWidget_eventOnGainedNavigation_Parms*)Obj)->bFromChild = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::NewProp_bFromChild = { "bFromChild", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventOnGainedNavigation_Parms), &Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::NewProp_bFromChild_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bFromChild_MetaData), NewProp_bFromChild_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::NewProp_PreviousActiveWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::NewProp_bFromChild,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnGainedNavigation", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::PropPointers), sizeof(UINavWidget_eventOnGainedNavigation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnGainedNavigation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnGainedNavigation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnGainedNavigation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnGainedNavigation)
{
	P_GET_OBJECT(UUINavWidget,Z_Param_PreviousActiveWidget);
	P_GET_UBOOL(Z_Param_bFromChild);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnGainedNavigation_Implementation(Z_Param_PreviousActiveWidget,Z_Param_bFromChild);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnGainedNavigation

// Begin Class UUINavWidget Function OnHorizCompNavigateLeft
struct UINavWidget_eventOnHorizCompNavigateLeft_Parms
{
	UUINavComponent* Component;
};
static FName NAME_UUINavWidget_OnHorizCompNavigateLeft = FName(TEXT("OnHorizCompNavigateLeft"));
void UUINavWidget::OnHorizCompNavigateLeft(UUINavComponent* Component)
{
	UINavWidget_eventOnHorizCompNavigateLeft_Parms Parms;
	Parms.Component=Component;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnHorizCompNavigateLeft),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the user navigates left on a UINavComponentBox\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the user navigates left on a UINavComponentBox" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnHorizCompNavigateLeft_Parms, Component), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics::NewProp_Component,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnHorizCompNavigateLeft", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics::PropPointers), sizeof(UINavWidget_eventOnHorizCompNavigateLeft_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnHorizCompNavigateLeft_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnHorizCompNavigateLeft)
{
	P_GET_OBJECT(UUINavComponent,Z_Param_Component);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnHorizCompNavigateLeft_Implementation(Z_Param_Component);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnHorizCompNavigateLeft

// Begin Class UUINavWidget Function OnHorizCompNavigateRight
struct UINavWidget_eventOnHorizCompNavigateRight_Parms
{
	UUINavComponent* Component;
};
static FName NAME_UUINavWidget_OnHorizCompNavigateRight = FName(TEXT("OnHorizCompNavigateRight"));
void UUINavWidget::OnHorizCompNavigateRight(UUINavComponent* Component)
{
	UINavWidget_eventOnHorizCompNavigateRight_Parms Parms;
	Parms.Component=Component;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnHorizCompNavigateRight),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the user navigates right on a UINavComponentBox\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the user navigates right on a UINavComponentBox" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnHorizCompNavigateRight_Parms, Component), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics::NewProp_Component,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnHorizCompNavigateRight", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics::PropPointers), sizeof(UINavWidget_eventOnHorizCompNavigateRight_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnHorizCompNavigateRight_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnHorizCompNavigateRight)
{
	P_GET_OBJECT(UUINavComponent,Z_Param_Component);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnHorizCompNavigateRight_Implementation(Z_Param_Component);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnHorizCompNavigateRight

// Begin Class UUINavWidget Function OnHorizCompUpdated
struct UINavWidget_eventOnHorizCompUpdated_Parms
{
	UUINavComponent* Component;
};
static FName NAME_UUINavWidget_OnHorizCompUpdated = FName(TEXT("OnHorizCompUpdated"));
void UUINavWidget::OnHorizCompUpdated(UUINavComponent* Component)
{
	UINavWidget_eventOnHorizCompUpdated_Parms Parms;
	Parms.Component=Component;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnHorizCompUpdated),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09* Called when a HorizontalComponent was updated\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when a HorizontalComponent was updated" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnHorizCompUpdated_Parms, Component), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics::NewProp_Component,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnHorizCompUpdated", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics::PropPointers), sizeof(UINavWidget_eventOnHorizCompUpdated_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnHorizCompUpdated_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnHorizCompUpdated)
{
	P_GET_OBJECT(UUINavComponent,Z_Param_Component);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnHorizCompUpdated_Implementation(Z_Param_Component);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnHorizCompUpdated

// Begin Class UUINavWidget Function OnInputChanged
struct UINavWidget_eventOnInputChanged_Parms
{
	EInputType From;
	EInputType To;
};
static FName NAME_UUINavWidget_OnInputChanged = FName(TEXT("OnInputChanged"));
void UUINavWidget::OnInputChanged(const EInputType From, const EInputType To)
{
	UINavWidget_eventOnInputChanged_Parms Parms;
	Parms.From=From;
	Parms.To=To;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnInputChanged),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the input type changed\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the input type changed" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_From_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_To_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_From_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_From;
	static const UECodeGen_Private::FBytePropertyParams NewProp_To_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_To;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::NewProp_From_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::NewProp_From = { "From", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnInputChanged_Parms, From), Z_Construct_UEnum_UINavigation_EInputType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_From_MetaData), NewProp_From_MetaData) }; // 346009216
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::NewProp_To_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::NewProp_To = { "To", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnInputChanged_Parms, To), Z_Construct_UEnum_UINavigation_EInputType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_To_MetaData), NewProp_To_MetaData) }; // 346009216
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::NewProp_From_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::NewProp_From,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::NewProp_To_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::NewProp_To,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnInputChanged", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::PropPointers), sizeof(UINavWidget_eventOnInputChanged_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnInputChanged_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnInputChanged()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnInputChanged_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnInputChanged)
{
	P_GET_ENUM(EInputType,Z_Param_From);
	P_GET_ENUM(EInputType,Z_Param_To);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnInputChanged_Implementation(EInputType(Z_Param_From),EInputType(Z_Param_To));
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnInputChanged

// Begin Class UUINavWidget Function OnLostNavigation
struct UINavWidget_eventOnLostNavigation_Parms
{
	UUINavWidget* NewActiveWidget;
	bool bToChild;
};
static FName NAME_UUINavWidget_OnLostNavigation = FName(TEXT("OnLostNavigation"));
void UUINavWidget::OnLostNavigation(UUINavWidget* NewActiveWidget, bool bToChild)
{
	UINavWidget_eventOnLostNavigation_Parms Parms;
	Parms.NewActiveWidget=NewActiveWidget;
	Parms.bToChild=bToChild ? true : false;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnLostNavigation),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when navigation is lost\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when navigation is lost" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewActiveWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bToChild_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewActiveWidget;
	static void NewProp_bToChild_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bToChild;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::NewProp_NewActiveWidget = { "NewActiveWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnLostNavigation_Parms, NewActiveWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewActiveWidget_MetaData), NewProp_NewActiveWidget_MetaData) };
void Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::NewProp_bToChild_SetBit(void* Obj)
{
	((UINavWidget_eventOnLostNavigation_Parms*)Obj)->bToChild = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::NewProp_bToChild = { "bToChild", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventOnLostNavigation_Parms), &Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::NewProp_bToChild_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bToChild_MetaData), NewProp_bToChild_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::NewProp_NewActiveWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::NewProp_bToChild,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnLostNavigation", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::PropPointers), sizeof(UINavWidget_eventOnLostNavigation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnLostNavigation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnLostNavigation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnLostNavigation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnLostNavigation)
{
	P_GET_OBJECT(UUINavWidget,Z_Param_NewActiveWidget);
	P_GET_UBOOL(Z_Param_bToChild);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnLostNavigation_Implementation(Z_Param_NewActiveWidget,Z_Param_bToChild);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnLostNavigation

// Begin Class UUINavWidget Function OnNavigate
struct UINavWidget_eventOnNavigate_Parms
{
	UUINavComponent* FromComponent;
	UUINavComponent* ToComponent;
};
static FName NAME_UUINavWidget_OnNavigate = FName(TEXT("OnNavigate"));
void UUINavWidget::OnNavigate(UUINavComponent* FromComponent, UUINavComponent* ToComponent)
{
	UINavWidget_eventOnNavigate_Parms Parms;
	Parms.FromComponent=FromComponent;
	Parms.ToComponent=ToComponent;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnNavigate),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the button with the specified index was navigated upon\n\x09*\n\x09*\x09@param\x09""From  The index of the button that was navigated from\n\x09*\x09@param\x09To  The index of the button that was navigated to\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the button with the specified index was navigated upon\n\n@param  From  The index of the button that was navigated from\n@param  To  The index of the button that was navigated to" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FromComponent_MetaData[] = {
		{ "EditInline", "true" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ToComponent_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FromComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ToComponent;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::NewProp_FromComponent = { "FromComponent", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnNavigate_Parms, FromComponent), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FromComponent_MetaData), NewProp_FromComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::NewProp_ToComponent = { "ToComponent", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnNavigate_Parms, ToComponent), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ToComponent_MetaData), NewProp_ToComponent_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::NewProp_FromComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::NewProp_ToComponent,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnNavigate", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::PropPointers), sizeof(UINavWidget_eventOnNavigate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnNavigate_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnNavigate()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnNavigate_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnNavigate)
{
	P_GET_OBJECT(UUINavComponent,Z_Param_FromComponent);
	P_GET_OBJECT(UUINavComponent,Z_Param_ToComponent);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnNavigate_Implementation(Z_Param_FromComponent,Z_Param_ToComponent);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnNavigate

// Begin Class UUINavWidget Function OnNext
static FName NAME_UUINavWidget_OnNext = FName(TEXT("OnNext"));
void UUINavWidget::OnNext()
{
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnNext),NULL);
}
struct Z_Construct_UFunction_UUINavWidget_OnNext_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when player navigates to the next section\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when player navigates to the next section" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnNext_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnNext", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnNext_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnNext_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnNext()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnNext_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnNext)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnNext_Implementation();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnNext

// Begin Class UUINavWidget Function OnPrevious
static FName NAME_UUINavWidget_OnPrevious = FName(TEXT("OnPrevious"));
void UUINavWidget::OnPrevious()
{
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnPrevious),NULL);
}
struct Z_Construct_UFunction_UUINavWidget_OnPrevious_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when player navigates to the previous section\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when player navigates to the previous section" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnPrevious_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnPrevious", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnPrevious_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnPrevious_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnPrevious()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnPrevious_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnPrevious)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnPrevious_Implementation();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnPrevious

// Begin Class UUINavWidget Function OnReturn
static FName NAME_UUINavWidget_OnReturn = FName(TEXT("OnReturn"));
void UUINavWidget::OnReturn()
{
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnReturn),NULL);
}
struct Z_Construct_UFunction_UUINavWidget_OnReturn_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when ReturnToParent is called (i.e. the player wants to exit the menu)\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when ReturnToParent is called (i.e. the player wants to exit the menu)" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnReturn_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnReturn", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnReturn_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnReturn_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnReturn()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnReturn_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnReturn)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnReturn_Implementation();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnReturn

// Begin Class UUINavWidget Function OnSectionButtonPressed1
struct Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed1_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed1_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSectionButtonPressed1", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed1_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed1_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed1()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed1_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSectionButtonPressed1)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSectionButtonPressed1();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSectionButtonPressed1

// Begin Class UUINavWidget Function OnSectionButtonPressed10
struct Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed10_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed10_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSectionButtonPressed10", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed10_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed10_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed10()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed10_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSectionButtonPressed10)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSectionButtonPressed10();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSectionButtonPressed10

// Begin Class UUINavWidget Function OnSectionButtonPressed2
struct Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed2_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed2_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSectionButtonPressed2", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed2_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed2_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed2()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed2_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSectionButtonPressed2)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSectionButtonPressed2();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSectionButtonPressed2

// Begin Class UUINavWidget Function OnSectionButtonPressed3
struct Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed3_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed3_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSectionButtonPressed3", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed3_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed3_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed3()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed3_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSectionButtonPressed3)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSectionButtonPressed3();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSectionButtonPressed3

// Begin Class UUINavWidget Function OnSectionButtonPressed4
struct Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed4_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed4_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSectionButtonPressed4", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed4_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed4_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed4()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed4_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSectionButtonPressed4)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSectionButtonPressed4();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSectionButtonPressed4

// Begin Class UUINavWidget Function OnSectionButtonPressed5
struct Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed5_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed5_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSectionButtonPressed5", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed5_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed5_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed5()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed5_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSectionButtonPressed5)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSectionButtonPressed5();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSectionButtonPressed5

// Begin Class UUINavWidget Function OnSectionButtonPressed6
struct Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed6_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed6_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSectionButtonPressed6", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed6_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed6_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed6()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed6_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSectionButtonPressed6)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSectionButtonPressed6();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSectionButtonPressed6

// Begin Class UUINavWidget Function OnSectionButtonPressed7
struct Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed7_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed7_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSectionButtonPressed7", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed7_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed7_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed7()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed7_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSectionButtonPressed7)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSectionButtonPressed7();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSectionButtonPressed7

// Begin Class UUINavWidget Function OnSectionButtonPressed8
struct Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed8_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed8_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSectionButtonPressed8", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed8_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed8_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed8()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed8_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSectionButtonPressed8)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSectionButtonPressed8();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSectionButtonPressed8

// Begin Class UUINavWidget Function OnSectionButtonPressed9
struct Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed9_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed9_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSectionButtonPressed9", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed9_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed9_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed9()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed9_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSectionButtonPressed9)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSectionButtonPressed9();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSectionButtonPressed9

// Begin Class UUINavWidget Function OnSelect
struct UINavWidget_eventOnSelect_Parms
{
	UUINavComponent* Component;
};
static FName NAME_UUINavWidget_OnSelect = FName(TEXT("OnSelect"));
void UUINavWidget::OnSelect(UUINavComponent* Component)
{
	UINavWidget_eventOnSelect_Parms Parms;
	Parms.Component=Component;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnSelect),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnSelect_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09Notifies that a button was selected, and indicates its index\n\x09*\n\x09*\x09@param\x09Index  The index of the button that was selected\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Notifies that a button was selected, and indicates its index\n\n@param  Index  The index of the button that was selected" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_OnSelect_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnSelect_Parms, Component), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnSelect_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnSelect_Statics::NewProp_Component,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSelect_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSelect_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSelect", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnSelect_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSelect_Statics::PropPointers), sizeof(UINavWidget_eventOnSelect_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSelect_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSelect_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnSelect_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnSelect()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSelect_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSelect)
{
	P_GET_OBJECT(UUINavComponent,Z_Param_Component);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSelect_Implementation(Z_Param_Component);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSelect

// Begin Class UUINavWidget Function OnSetupCompleted
static FName NAME_UUINavWidget_OnSetupCompleted = FName(TEXT("OnSetupCompleted"));
void UUINavWidget::OnSetupCompleted()
{
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnSetupCompleted),NULL);
}
struct Z_Construct_UFunction_UUINavWidget_OnSetupCompleted_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when this widget completed UINavSetup\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when this widget completed UINavSetup" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnSetupCompleted_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnSetupCompleted", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnSetupCompleted_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnSetupCompleted_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavWidget_OnSetupCompleted()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnSetupCompleted_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnSetupCompleted)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSetupCompleted_Implementation();
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnSetupCompleted

// Begin Class UUINavWidget Function OnStartSelect
struct UINavWidget_eventOnStartSelect_Parms
{
	UUINavComponent* Component;
};
static FName NAME_UUINavWidget_OnStartSelect = FName(TEXT("OnStartSelect"));
void UUINavWidget::OnStartSelect(UUINavComponent* Component)
{
	UINavWidget_eventOnStartSelect_Parms Parms;
	Parms.Component=Component;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnStartSelect),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnStartSelect_Parms, Component), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics::NewProp_Component,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnStartSelect", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics::PropPointers), sizeof(UINavWidget_eventOnStartSelect_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnStartSelect_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnStartSelect()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnStartSelect_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnStartSelect)
{
	P_GET_OBJECT(UUINavComponent,Z_Param_Component);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnStartSelect_Implementation(Z_Param_Component);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnStartSelect

// Begin Class UUINavWidget Function OnStopSelect
struct UINavWidget_eventOnStopSelect_Parms
{
	UUINavComponent* Component;
};
static FName NAME_UUINavWidget_OnStopSelect = FName(TEXT("OnStopSelect"));
void UUINavWidget::OnStopSelect(UUINavComponent* Component)
{
	UINavWidget_eventOnStopSelect_Parms Parms;
	Parms.Component=Component;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnStopSelect),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnStopSelect_Parms, Component), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics::NewProp_Component,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnStopSelect", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics::PropPointers), sizeof(UINavWidget_eventOnStopSelect_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnStopSelect_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnStopSelect()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnStopSelect_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnStopSelect)
{
	P_GET_OBJECT(UUINavComponent,Z_Param_Component);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnStopSelect_Implementation(Z_Param_Component);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnStopSelect

// Begin Class UUINavWidget Function OnThumbstickCursorInput
struct UINavWidget_eventOnThumbstickCursorInput_Parms
{
	FVector2D ThumbstickDelta;
};
static FName NAME_UUINavWidget_OnThumbstickCursorInput = FName(TEXT("OnThumbstickCursorInput"));
void UUINavWidget::OnThumbstickCursorInput(FVector2D const& ThumbstickDelta)
{
	UINavWidget_eventOnThumbstickCursorInput_Parms Parms;
	Parms.ThumbstickDelta=ThumbstickDelta;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_OnThumbstickCursorInput),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the gamepad's thumbstick moves\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the gamepad's thumbstick moves" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ThumbstickDelta_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_ThumbstickDelta;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics::NewProp_ThumbstickDelta = { "ThumbstickDelta", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventOnThumbstickCursorInput_Parms, ThumbstickDelta), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ThumbstickDelta_MetaData), NewProp_ThumbstickDelta_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics::NewProp_ThumbstickDelta,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "OnThumbstickCursorInput", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics::PropPointers), sizeof(UINavWidget_eventOnThumbstickCursorInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08C20C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventOnThumbstickCursorInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execOnThumbstickCursorInput)
{
	P_GET_STRUCT_REF(FVector2D,Z_Param_Out_ThumbstickDelta);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnThumbstickCursorInput_Implementation(Z_Param_Out_ThumbstickDelta);
	P_NATIVE_END;
}
// End Class UUINavWidget Function OnThumbstickCursorInput

// Begin Class UUINavWidget Function PreSetup
struct UINavWidget_eventPreSetup_Parms
{
	bool bFirstSetup;
};
static FName NAME_UUINavWidget_PreSetup = FName(TEXT("PreSetup"));
void UUINavWidget::PreSetup(bool bFirstSetup)
{
	UINavWidget_eventPreSetup_Parms Parms;
	Parms.bFirstSetup=bFirstSetup ? true : false;
	ProcessEvent(FindFunctionChecked(NAME_UUINavWidget_PreSetup),&Parms);
}
struct Z_Construct_UFunction_UUINavWidget_PreSetup_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called before this widget is setup for UINav logic\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called before this widget is setup for UINav logic" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bFirstSetup_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bFirstSetup_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bFirstSetup;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::NewProp_bFirstSetup_SetBit(void* Obj)
{
	((UINavWidget_eventPreSetup_Parms*)Obj)->bFirstSetup = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::NewProp_bFirstSetup = { "bFirstSetup", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventPreSetup_Parms), &Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::NewProp_bFirstSetup_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bFirstSetup_MetaData), NewProp_bFirstSetup_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::NewProp_bFirstSetup,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "PreSetup", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::PropPointers), sizeof(UINavWidget_eventPreSetup_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavWidget_eventPreSetup_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_PreSetup()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_PreSetup_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execPreSetup)
{
	P_GET_UBOOL(Z_Param_bFirstSetup);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->PreSetup_Implementation(Z_Param_bFirstSetup);
	P_NATIVE_END;
}
// End Class UUINavWidget Function PreSetup

// Begin Class UUINavWidget Function ReturnToParent
struct Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics
{
	struct UINavWidget_eventReturnToParent_Parms
	{
		bool bRemoveAllParents;
		int32 ZOrder;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "AdvancedDisplay", "1" },
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Adds this widget's parent to the viewport (if applicable)\n\x09*\x09""and removes this widget from viewport\n\x09*/" },
#endif
		{ "CPP_Default_bRemoveAllParents", "false" },
		{ "CPP_Default_ZOrder", "0" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Adds this widget's parent to the viewport (if applicable)\nand removes this widget from viewport" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRemoveAllParents_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ZOrder_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bRemoveAllParents_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRemoveAllParents;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ZOrder;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::NewProp_bRemoveAllParents_SetBit(void* Obj)
{
	((UINavWidget_eventReturnToParent_Parms*)Obj)->bRemoveAllParents = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::NewProp_bRemoveAllParents = { "bRemoveAllParents", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventReturnToParent_Parms), &Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::NewProp_bRemoveAllParents_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRemoveAllParents_MetaData), NewProp_bRemoveAllParents_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::NewProp_ZOrder = { "ZOrder", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventReturnToParent_Parms, ZOrder), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ZOrder_MetaData), NewProp_ZOrder_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::NewProp_bRemoveAllParents,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::NewProp_ZOrder,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "ReturnToParent", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::UINavWidget_eventReturnToParent_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::UINavWidget_eventReturnToParent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_ReturnToParent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_ReturnToParent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execReturnToParent)
{
	P_GET_UBOOL(Z_Param_bRemoveAllParents);
	P_GET_PROPERTY(FIntProperty,Z_Param_ZOrder);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ReturnToParent(Z_Param_bRemoveAllParents,Z_Param_ZOrder);
	P_NATIVE_END;
}
// End Class UUINavWidget Function ReturnToParent

// Begin Class UUINavWidget Function SetSelectorScale
struct Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics
{
	struct UINavWidget_eventSetSelectorScale_Parms
	{
		FVector2D NewScale;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Changes the selector's scale to the scale given\n\x09*\n\x09*\x09@param\x09NewScale  The selector's new scale\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Changes the selector's scale to the scale given\n\n@param  NewScale  The selector's new scale" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_NewScale;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::NewProp_NewScale = { "NewScale", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventSetSelectorScale_Parms, NewScale), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::NewProp_NewScale,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "SetSelectorScale", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::UINavWidget_eventSetSelectorScale_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::UINavWidget_eventSetSelectorScale_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_SetSelectorScale()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_SetSelectorScale_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execSetSelectorScale)
{
	P_GET_STRUCT(FVector2D,Z_Param_NewScale);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetSelectorScale(Z_Param_NewScale);
	P_NATIVE_END;
}
// End Class UUINavWidget Function SetSelectorScale

// Begin Class UUINavWidget Function SetSelectorVisible
struct Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics
{
	struct UINavWidget_eventSetSelectorVisible_Parms
	{
		bool bVisible;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Changes the selector's visibility\n\x09*\n\x09*\x09@param\x09""bVisible Whether the selector will be visible\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Changes the selector's visibility\n\n@param  bVisible Whether the selector will be visible" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bVisible_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bVisible_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bVisible;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::NewProp_bVisible_SetBit(void* Obj)
{
	((UINavWidget_eventSetSelectorVisible_Parms*)Obj)->bVisible = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::NewProp_bVisible = { "bVisible", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavWidget_eventSetSelectorVisible_Parms), &Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::NewProp_bVisible_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bVisible_MetaData), NewProp_bVisible_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::NewProp_bVisible,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "SetSelectorVisible", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::UINavWidget_eventSetSelectorVisible_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::UINavWidget_eventSetSelectorVisible_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_SetSelectorVisible()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_SetSelectorVisible_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execSetSelectorVisible)
{
	P_GET_UBOOL(Z_Param_bVisible);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetSelectorVisible(Z_Param_bVisible);
	P_NATIVE_END;
}
// End Class UUINavWidget Function SetSelectorVisible

// Begin Class UUINavWidget Function UpdateSelectorLocation
struct Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics
{
	struct UINavWidget_eventUpdateSelectorLocation_Parms
	{
		UUINavComponent* Component;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Changes the selector's location to that of the button with the given index in the Button's array\n\x09*\n\x09*\x09@param\x09Index  The new button's index in the Button's array\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Changes the selector's location to that of the button with the given index in the Button's array\n\n@param  Index  The new button's index in the Button's array" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavWidget_eventUpdateSelectorLocation_Parms, Component), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::NewProp_Component,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavWidget, nullptr, "UpdateSelectorLocation", nullptr, nullptr, Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::UINavWidget_eventUpdateSelectorLocation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::UINavWidget_eventUpdateSelectorLocation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavWidget::execUpdateSelectorLocation)
{
	P_GET_OBJECT(UUINavComponent,Z_Param_Component);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->UpdateSelectorLocation(Z_Param_Component);
	P_NATIVE_END;
}
// End Class UUINavWidget Function UpdateSelectorLocation

// Begin Class UUINavWidget
void UUINavWidget::StaticRegisterNativesUUINavWidget()
{
	UClass* Class = UUINavWidget::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "ConfigureUINavPC", &UUINavWidget::execConfigureUINavPC },
		{ "GetInitialFocusComponent", &UUINavWidget::execGetInitialFocusComponent },
		{ "GetMostOuterUINavWidget", &UUINavWidget::execGetMostOuterUINavWidget },
		{ "GetScrollBoxToFocus", &UUINavWidget::execGetScrollBoxToFocus },
		{ "GoToBuiltWidget", &UUINavWidget::execGoToBuiltWidget },
		{ "GoToNextSection", &UUINavWidget::execGoToNextSection },
		{ "GoToPreviousSection", &UUINavWidget::execGoToPreviousSection },
		{ "GoToPromptWidget", &UUINavWidget::execGoToPromptWidget },
		{ "GoToSection", &UUINavWidget::execGoToSection },
		{ "GoToWidget", &UUINavWidget::execGoToWidget },
		{ "IsRebindingInput", &UUINavWidget::execIsRebindingInput },
		{ "IsSelectorValid", &UUINavWidget::execIsSelectorValid },
		{ "IsSelectorVisible", &UUINavWidget::execIsSelectorVisible },
		{ "OnChangedSection", &UUINavWidget::execOnChangedSection },
		{ "OnChildReturn", &UUINavWidget::execOnChildReturn },
		{ "OnGainedNavigation", &UUINavWidget::execOnGainedNavigation },
		{ "OnHorizCompNavigateLeft", &UUINavWidget::execOnHorizCompNavigateLeft },
		{ "OnHorizCompNavigateRight", &UUINavWidget::execOnHorizCompNavigateRight },
		{ "OnHorizCompUpdated", &UUINavWidget::execOnHorizCompUpdated },
		{ "OnInputChanged", &UUINavWidget::execOnInputChanged },
		{ "OnLostNavigation", &UUINavWidget::execOnLostNavigation },
		{ "OnNavigate", &UUINavWidget::execOnNavigate },
		{ "OnNext", &UUINavWidget::execOnNext },
		{ "OnPrevious", &UUINavWidget::execOnPrevious },
		{ "OnReturn", &UUINavWidget::execOnReturn },
		{ "OnSectionButtonPressed1", &UUINavWidget::execOnSectionButtonPressed1 },
		{ "OnSectionButtonPressed10", &UUINavWidget::execOnSectionButtonPressed10 },
		{ "OnSectionButtonPressed2", &UUINavWidget::execOnSectionButtonPressed2 },
		{ "OnSectionButtonPressed3", &UUINavWidget::execOnSectionButtonPressed3 },
		{ "OnSectionButtonPressed4", &UUINavWidget::execOnSectionButtonPressed4 },
		{ "OnSectionButtonPressed5", &UUINavWidget::execOnSectionButtonPressed5 },
		{ "OnSectionButtonPressed6", &UUINavWidget::execOnSectionButtonPressed6 },
		{ "OnSectionButtonPressed7", &UUINavWidget::execOnSectionButtonPressed7 },
		{ "OnSectionButtonPressed8", &UUINavWidget::execOnSectionButtonPressed8 },
		{ "OnSectionButtonPressed9", &UUINavWidget::execOnSectionButtonPressed9 },
		{ "OnSelect", &UUINavWidget::execOnSelect },
		{ "OnSetupCompleted", &UUINavWidget::execOnSetupCompleted },
		{ "OnStartSelect", &UUINavWidget::execOnStartSelect },
		{ "OnStopSelect", &UUINavWidget::execOnStopSelect },
		{ "OnThumbstickCursorInput", &UUINavWidget::execOnThumbstickCursorInput },
		{ "PreSetup", &UUINavWidget::execPreSetup },
		{ "ReturnToParent", &UUINavWidget::execReturnToParent },
		{ "SetSelectorScale", &UUINavWidget::execSetSelectorScale },
		{ "SetSelectorVisible", &UUINavWidget::execSetSelectorVisible },
		{ "UpdateSelectorLocation", &UUINavWidget::execUpdateSelectorLocation },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavWidget);
UClass* Z_Construct_UClass_UUINavWidget_NoRegister()
{
	return UUINavWidget::StaticClass();
}
struct Z_Construct_UClass_UUINavWidget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n* This class contains the logic for UserWidget navigation\n*/" },
#endif
		{ "IncludePath", "UINavWidget.h" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This class contains the logic for UserWidget navigation" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UpdateSelectorPrevComponent_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UpdateSelectorNextComponent_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FirstComponent_MetaData[] = {
		{ "Category", "UINavWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentComponent_MetaData[] = {
		{ "Category", "UINavWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HoveredComponent_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SelectedComponent_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_IgnoreHoverComponent_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TheSelector_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//The UserWidget object that will move along the Widget\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
		{ "OptionalWidget", "TRUE" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The UserWidget object that will move along the Widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ChildUINavWidgets_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//All the child UINavWidgets in this Widget\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "All the child UINavWidgets in this Widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ParentWidget_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Reference to the parent widget that created this widget\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Reference to the parent widget that created this widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OuterUINavWidget_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Reference to the widget that encapsulates this widget\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Reference to the widget that encapsulates this widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UINavPC_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Current player controller\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Current player controller" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnedFromWidget_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Widget that created this widget (if returned from a child)\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Widget that created this widget (if returned from a child)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WidgetComp_MetaData[] = {
		{ "Category", "UINavWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UINavSwitcher_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//The Widget Switcher that will be used to switch between sections\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
		{ "OptionalWidget", "TRUE" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The Widget Switcher that will be used to switch between sections" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UINavSectionsPanel_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//The PanelWidget or UserWidget that contains the Buttons (not UINavComponents) that will switch between the sections\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
		{ "OptionalWidget", "TRUE" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The PanelWidget or UserWidget that contains the Buttons (not UINavComponents) that will switch between the sections" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bParentRemoved_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Should this widget remove its parent from the viewport when created?\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Should this widget remove its parent from the viewport when created?" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bShouldDestroyParent_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Should this widget destroy its parent\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Should this widget destroy its parent" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAllowRemoveIfRoot_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//If set to true, this widget will be removed if it has no ParentWidget and is returned from\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "If set to true, this widget will be removed if it has no ParentWidget and is returned from" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bShowSelector_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//If set to true, this widget will show the selector it has, otherwise it will hide it.\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "If set to true, this widget will show the selector it has, otherwise it will hide it." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bMaintainNavigationForChild_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09* If set to true, the UINavWidget will maintain its navigated state when navigation moves to a child nested widget,\n\x09* otherwise, the button being navigated to at that moment will be navigated out of.\n\x09* Only applies when using nested UINavWidgets.\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "* If set to true, the UINavWidget will maintain its navigated state when navigation moves to a child nested widget,\n* otherwise, the button being navigated to at that moment will be navigated out of.\n* Only applies when using nested UINavWidgets." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bClearNavigationStateWhenChild_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09* If set to true, the UINavWidget will clear its navigation state if it's a child UINavWidget.\n\x09* Only applies when using nested UINavWidgets.\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "* If set to true, the UINavWidget will clear its navigation state if it's a child UINavWidget.\n* Only applies when using nested UINavWidgets." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bWrapAutoSectionNavigation_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//If set to true, this widget will go from the first section to the last and vice-versa when using auto section switching.\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "If set to true, this widget will go from the first section to the last and vice-versa when using auto section switching." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UINavInputContextOverrides_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09* Input Context to be used to replace the default one. for each platform, in this specific widget (assuming a child widget doesn't override that)\n\x09* The Map's Key (String) should be the name of the platform you want to override. Leave blank if this applies to all platforms.\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "* Input Context to be used to replace the default one. for each platform, in this specific widget (assuming a child widget doesn't override that)\n* The Map's Key (String) should be the name of the platform you want to override. Leave blank if this applies to all platforms." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputContextsToAdd_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09* Input Contexts to be applied when this widget becomes active (and to be removed when it becomes inactive)\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "* Input Contexts to be applied when this widget becomes active (and to be removed when it becomes inactive)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UseThumbstickAsMouse_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*If set to Left or Right, the gamepad's left or right thumbstick will be used to move the mouse when this widget is active */" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "If set to Left or Right, the gamepad's left or right thumbstick will be used to move the mouse when this widget is active" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseFullscreenWhenSplitscreen_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*If set to true, the widget will be set to fullscreen even when using split screen */" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "If set to true, the widget will be set to fullscreen even when using split screen" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bForceUsePlayerScreen_MetaData[] = {
		{ "Category", "UINavWidget" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// If set to true, will always use AddToPlayerScreen instead of AddToViewport, even if not in split screen\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "If set to true, will always use AddToPlayerScreen instead of AddToViewport, even if not in split screen" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MoveCurve_MetaData[] = {
		{ "Category", "UINavigation Selector" },
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SelectorPositioning_MetaData[] = {
		{ "Category", "UINavigation Selector" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//The position the selector will be in relative to the button\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The position the selector will be in relative to the button" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SelectorOffset_MetaData[] = {
		{ "Category", "UINavigation Selector" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//The offset to apply when positioning the selector on a button\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The offset to apply when positioning the selector on a button" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UpdateSelectorPrevComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UpdateSelectorNextComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FirstComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CurrentComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_HoveredComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SelectedComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_IgnoreHoverComponent;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TheSelector;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ChildUINavWidgets_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ChildUINavWidgets;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ParentWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OuterUINavWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UINavPC;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnedFromWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_WidgetComp;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UINavSwitcher;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UINavSectionsPanel;
	static void NewProp_bParentRemoved_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bParentRemoved;
	static void NewProp_bShouldDestroyParent_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bShouldDestroyParent;
	static void NewProp_bAllowRemoveIfRoot_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAllowRemoveIfRoot;
	static void NewProp_bShowSelector_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bShowSelector;
	static void NewProp_bMaintainNavigationForChild_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bMaintainNavigationForChild;
	static void NewProp_bClearNavigationStateWhenChild_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bClearNavigationStateWhenChild;
	static void NewProp_bWrapAutoSectionNavigation_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bWrapAutoSectionNavigation;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UINavInputContextOverrides_ValueProp;
	static const UECodeGen_Private::FStrPropertyParams NewProp_UINavInputContextOverrides_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_UINavInputContextOverrides;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InputContextsToAdd_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_InputContextsToAdd;
	static const UECodeGen_Private::FBytePropertyParams NewProp_UseThumbstickAsMouse_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_UseThumbstickAsMouse;
	static void NewProp_bUseFullscreenWhenSplitscreen_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseFullscreenWhenSplitscreen;
	static void NewProp_bForceUsePlayerScreen_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bForceUsePlayerScreen;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MoveCurve;
	static const UECodeGen_Private::FBytePropertyParams NewProp_SelectorPositioning_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_SelectorPositioning;
	static const UECodeGen_Private::FStructPropertyParams NewProp_SelectorOffset;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUINavWidget_ConfigureUINavPC, "ConfigureUINavPC" }, // 3801102349
		{ &Z_Construct_UFunction_UUINavWidget_GetInitialFocusComponent, "GetInitialFocusComponent" }, // 36223990
		{ &Z_Construct_UFunction_UUINavWidget_GetMostOuterUINavWidget, "GetMostOuterUINavWidget" }, // 3323091311
		{ &Z_Construct_UFunction_UUINavWidget_GetScrollBoxToFocus, "GetScrollBoxToFocus" }, // 2601653741
		{ &Z_Construct_UFunction_UUINavWidget_GoToBuiltWidget, "GoToBuiltWidget" }, // 2914343597
		{ &Z_Construct_UFunction_UUINavWidget_GoToNextSection, "GoToNextSection" }, // 3047736266
		{ &Z_Construct_UFunction_UUINavWidget_GoToPreviousSection, "GoToPreviousSection" }, // 2428571547
		{ &Z_Construct_UFunction_UUINavWidget_GoToPromptWidget, "GoToPromptWidget" }, // 1533373949
		{ &Z_Construct_UFunction_UUINavWidget_GoToSection, "GoToSection" }, // 3042604874
		{ &Z_Construct_UFunction_UUINavWidget_GoToWidget, "GoToWidget" }, // 3608797806
		{ &Z_Construct_UFunction_UUINavWidget_IsRebindingInput, "IsRebindingInput" }, // 214457364
		{ &Z_Construct_UFunction_UUINavWidget_IsSelectorValid, "IsSelectorValid" }, // 241998418
		{ &Z_Construct_UFunction_UUINavWidget_IsSelectorVisible, "IsSelectorVisible" }, // 249028693
		{ &Z_Construct_UFunction_UUINavWidget_OnChangedSection, "OnChangedSection" }, // 2142553052
		{ &Z_Construct_UFunction_UUINavWidget_OnChildReturn, "OnChildReturn" }, // 514315846
		{ &Z_Construct_UFunction_UUINavWidget_OnGainedNavigation, "OnGainedNavigation" }, // 922609986
		{ &Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateLeft, "OnHorizCompNavigateLeft" }, // 4031589848
		{ &Z_Construct_UFunction_UUINavWidget_OnHorizCompNavigateRight, "OnHorizCompNavigateRight" }, // 2420039899
		{ &Z_Construct_UFunction_UUINavWidget_OnHorizCompUpdated, "OnHorizCompUpdated" }, // 2225702579
		{ &Z_Construct_UFunction_UUINavWidget_OnInputChanged, "OnInputChanged" }, // 3841160910
		{ &Z_Construct_UFunction_UUINavWidget_OnLostNavigation, "OnLostNavigation" }, // 1750804396
		{ &Z_Construct_UFunction_UUINavWidget_OnNavigate, "OnNavigate" }, // 952558761
		{ &Z_Construct_UFunction_UUINavWidget_OnNext, "OnNext" }, // 3884208148
		{ &Z_Construct_UFunction_UUINavWidget_OnPrevious, "OnPrevious" }, // 1215209602
		{ &Z_Construct_UFunction_UUINavWidget_OnReturn, "OnReturn" }, // 4030968882
		{ &Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed1, "OnSectionButtonPressed1" }, // 1406085922
		{ &Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed10, "OnSectionButtonPressed10" }, // 2463807621
		{ &Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed2, "OnSectionButtonPressed2" }, // 79328044
		{ &Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed3, "OnSectionButtonPressed3" }, // 36686943
		{ &Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed4, "OnSectionButtonPressed4" }, // 429082843
		{ &Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed5, "OnSectionButtonPressed5" }, // 3708018159
		{ &Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed6, "OnSectionButtonPressed6" }, // 2848021470
		{ &Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed7, "OnSectionButtonPressed7" }, // 1170291350
		{ &Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed8, "OnSectionButtonPressed8" }, // 4163583663
		{ &Z_Construct_UFunction_UUINavWidget_OnSectionButtonPressed9, "OnSectionButtonPressed9" }, // 1551676188
		{ &Z_Construct_UFunction_UUINavWidget_OnSelect, "OnSelect" }, // 4075118014
		{ &Z_Construct_UFunction_UUINavWidget_OnSetupCompleted, "OnSetupCompleted" }, // 1877766134
		{ &Z_Construct_UFunction_UUINavWidget_OnStartSelect, "OnStartSelect" }, // 3157250236
		{ &Z_Construct_UFunction_UUINavWidget_OnStopSelect, "OnStopSelect" }, // 1533996038
		{ &Z_Construct_UFunction_UUINavWidget_OnThumbstickCursorInput, "OnThumbstickCursorInput" }, // 3523471772
		{ &Z_Construct_UFunction_UUINavWidget_PreSetup, "PreSetup" }, // 3296963237
		{ &Z_Construct_UFunction_UUINavWidget_ReturnToParent, "ReturnToParent" }, // 318654782
		{ &Z_Construct_UFunction_UUINavWidget_SetSelectorScale, "SetSelectorScale" }, // 4138059221
		{ &Z_Construct_UFunction_UUINavWidget_SetSelectorVisible, "SetSelectorVisible" }, // 3829333094
		{ &Z_Construct_UFunction_UUINavWidget_UpdateSelectorLocation, "UpdateSelectorLocation" }, // 3432747140
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavWidget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_UpdateSelectorPrevComponent = { "UpdateSelectorPrevComponent", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, UpdateSelectorPrevComponent), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UpdateSelectorPrevComponent_MetaData), NewProp_UpdateSelectorPrevComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_UpdateSelectorNextComponent = { "UpdateSelectorNextComponent", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, UpdateSelectorNextComponent), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UpdateSelectorNextComponent_MetaData), NewProp_UpdateSelectorNextComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_FirstComponent = { "FirstComponent", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, FirstComponent), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FirstComponent_MetaData), NewProp_FirstComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_CurrentComponent = { "CurrentComponent", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, CurrentComponent), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentComponent_MetaData), NewProp_CurrentComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_HoveredComponent = { "HoveredComponent", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, HoveredComponent), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HoveredComponent_MetaData), NewProp_HoveredComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_SelectedComponent = { "SelectedComponent", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, SelectedComponent), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SelectedComponent_MetaData), NewProp_SelectedComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_IgnoreHoverComponent = { "IgnoreHoverComponent", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, IgnoreHoverComponent), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_IgnoreHoverComponent_MetaData), NewProp_IgnoreHoverComponent_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_TheSelector = { "TheSelector", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, TheSelector), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TheSelector_MetaData), NewProp_TheSelector_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_ChildUINavWidgets_Inner = { "ChildUINavWidgets", nullptr, (EPropertyFlags)0x0000000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_ChildUINavWidgets = { "ChildUINavWidgets", nullptr, (EPropertyFlags)0x001000800000001c, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, ChildUINavWidgets), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ChildUINavWidgets_MetaData), NewProp_ChildUINavWidgets_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_ParentWidget = { "ParentWidget", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, ParentWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ParentWidget_MetaData), NewProp_ParentWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_OuterUINavWidget = { "OuterUINavWidget", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, OuterUINavWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OuterUINavWidget_MetaData), NewProp_OuterUINavWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavPC = { "UINavPC", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, UINavPC), Z_Construct_UClass_UUINavPCComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UINavPC_MetaData), NewProp_UINavPC_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_ReturnedFromWidget = { "ReturnedFromWidget", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, ReturnedFromWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnedFromWidget_MetaData), NewProp_ReturnedFromWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_WidgetComp = { "WidgetComp", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, WidgetComp), Z_Construct_UClass_UUINavWidgetComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WidgetComp_MetaData), NewProp_WidgetComp_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavSwitcher = { "UINavSwitcher", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, UINavSwitcher), Z_Construct_UClass_UWidgetSwitcher_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UINavSwitcher_MetaData), NewProp_UINavSwitcher_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavSectionsPanel = { "UINavSectionsPanel", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, UINavSectionsPanel), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UINavSectionsPanel_MetaData), NewProp_UINavSectionsPanel_MetaData) };
void Z_Construct_UClass_UUINavWidget_Statics::NewProp_bParentRemoved_SetBit(void* Obj)
{
	((UUINavWidget*)Obj)->bParentRemoved = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_bParentRemoved = { "bParentRemoved", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavWidget), &Z_Construct_UClass_UUINavWidget_Statics::NewProp_bParentRemoved_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bParentRemoved_MetaData), NewProp_bParentRemoved_MetaData) };
void Z_Construct_UClass_UUINavWidget_Statics::NewProp_bShouldDestroyParent_SetBit(void* Obj)
{
	((UUINavWidget*)Obj)->bShouldDestroyParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_bShouldDestroyParent = { "bShouldDestroyParent", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavWidget), &Z_Construct_UClass_UUINavWidget_Statics::NewProp_bShouldDestroyParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bShouldDestroyParent_MetaData), NewProp_bShouldDestroyParent_MetaData) };
void Z_Construct_UClass_UUINavWidget_Statics::NewProp_bAllowRemoveIfRoot_SetBit(void* Obj)
{
	((UUINavWidget*)Obj)->bAllowRemoveIfRoot = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_bAllowRemoveIfRoot = { "bAllowRemoveIfRoot", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavWidget), &Z_Construct_UClass_UUINavWidget_Statics::NewProp_bAllowRemoveIfRoot_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAllowRemoveIfRoot_MetaData), NewProp_bAllowRemoveIfRoot_MetaData) };
void Z_Construct_UClass_UUINavWidget_Statics::NewProp_bShowSelector_SetBit(void* Obj)
{
	((UUINavWidget*)Obj)->bShowSelector = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_bShowSelector = { "bShowSelector", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavWidget), &Z_Construct_UClass_UUINavWidget_Statics::NewProp_bShowSelector_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bShowSelector_MetaData), NewProp_bShowSelector_MetaData) };
void Z_Construct_UClass_UUINavWidget_Statics::NewProp_bMaintainNavigationForChild_SetBit(void* Obj)
{
	((UUINavWidget*)Obj)->bMaintainNavigationForChild = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_bMaintainNavigationForChild = { "bMaintainNavigationForChild", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavWidget), &Z_Construct_UClass_UUINavWidget_Statics::NewProp_bMaintainNavigationForChild_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bMaintainNavigationForChild_MetaData), NewProp_bMaintainNavigationForChild_MetaData) };
void Z_Construct_UClass_UUINavWidget_Statics::NewProp_bClearNavigationStateWhenChild_SetBit(void* Obj)
{
	((UUINavWidget*)Obj)->bClearNavigationStateWhenChild = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_bClearNavigationStateWhenChild = { "bClearNavigationStateWhenChild", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavWidget), &Z_Construct_UClass_UUINavWidget_Statics::NewProp_bClearNavigationStateWhenChild_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bClearNavigationStateWhenChild_MetaData), NewProp_bClearNavigationStateWhenChild_MetaData) };
void Z_Construct_UClass_UUINavWidget_Statics::NewProp_bWrapAutoSectionNavigation_SetBit(void* Obj)
{
	((UUINavWidget*)Obj)->bWrapAutoSectionNavigation = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_bWrapAutoSectionNavigation = { "bWrapAutoSectionNavigation", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavWidget), &Z_Construct_UClass_UUINavWidget_Statics::NewProp_bWrapAutoSectionNavigation_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bWrapAutoSectionNavigation_MetaData), NewProp_bWrapAutoSectionNavigation_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavInputContextOverrides_ValueProp = { "UINavInputContextOverrides", nullptr, (EPropertyFlags)0x0104000000000001, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavInputContextOverrides_Key_KeyProp = { "UINavInputContextOverrides_Key", nullptr, (EPropertyFlags)0x0100000000000001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavInputContextOverrides = { "UINavInputContextOverrides", nullptr, (EPropertyFlags)0x0114000000010015, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, UINavInputContextOverrides), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UINavInputContextOverrides_MetaData), NewProp_UINavInputContextOverrides_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_InputContextsToAdd_Inner = { "InputContextsToAdd", nullptr, (EPropertyFlags)0x0104000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_InputContextsToAdd = { "InputContextsToAdd", nullptr, (EPropertyFlags)0x0114000000010015, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, InputContextsToAdd), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputContextsToAdd_MetaData), NewProp_InputContextsToAdd_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_UseThumbstickAsMouse_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_UseThumbstickAsMouse = { "UseThumbstickAsMouse", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, UseThumbstickAsMouse), Z_Construct_UEnum_UINavigation_EThumbstickAsMouse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UseThumbstickAsMouse_MetaData), NewProp_UseThumbstickAsMouse_MetaData) }; // 819177189
void Z_Construct_UClass_UUINavWidget_Statics::NewProp_bUseFullscreenWhenSplitscreen_SetBit(void* Obj)
{
	((UUINavWidget*)Obj)->bUseFullscreenWhenSplitscreen = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_bUseFullscreenWhenSplitscreen = { "bUseFullscreenWhenSplitscreen", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavWidget), &Z_Construct_UClass_UUINavWidget_Statics::NewProp_bUseFullscreenWhenSplitscreen_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseFullscreenWhenSplitscreen_MetaData), NewProp_bUseFullscreenWhenSplitscreen_MetaData) };
void Z_Construct_UClass_UUINavWidget_Statics::NewProp_bForceUsePlayerScreen_SetBit(void* Obj)
{
	((UUINavWidget*)Obj)->bForceUsePlayerScreen = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_bForceUsePlayerScreen = { "bForceUsePlayerScreen", nullptr, (EPropertyFlags)0x0010000000010015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavWidget), &Z_Construct_UClass_UUINavWidget_Statics::NewProp_bForceUsePlayerScreen_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bForceUsePlayerScreen_MetaData), NewProp_bForceUsePlayerScreen_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_MoveCurve = { "MoveCurve", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, MoveCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MoveCurve_MetaData), NewProp_MoveCurve_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_SelectorPositioning_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_SelectorPositioning = { "SelectorPositioning", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, SelectorPositioning), Z_Construct_UEnum_UINavigation_ESelectorPosition, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SelectorPositioning_MetaData), NewProp_SelectorPositioning_MetaData) }; // 1879829295
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UUINavWidget_Statics::NewProp_SelectorOffset = { "SelectorOffset", nullptr, (EPropertyFlags)0x0010000000010005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavWidget, SelectorOffset), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SelectorOffset_MetaData), NewProp_SelectorOffset_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_UpdateSelectorPrevComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_UpdateSelectorNextComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_FirstComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_CurrentComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_HoveredComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_SelectedComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_IgnoreHoverComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_TheSelector,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_ChildUINavWidgets_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_ChildUINavWidgets,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_ParentWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_OuterUINavWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavPC,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_ReturnedFromWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_WidgetComp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavSwitcher,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavSectionsPanel,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_bParentRemoved,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_bShouldDestroyParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_bAllowRemoveIfRoot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_bShowSelector,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_bMaintainNavigationForChild,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_bClearNavigationStateWhenChild,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_bWrapAutoSectionNavigation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavInputContextOverrides_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavInputContextOverrides_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_UINavInputContextOverrides,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_InputContextsToAdd_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_InputContextsToAdd,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_UseThumbstickAsMouse_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_UseThumbstickAsMouse,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_bUseFullscreenWhenSplitscreen,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_bForceUsePlayerScreen,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_MoveCurve,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_SelectorPositioning_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_SelectorPositioning,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavWidget_Statics::NewProp_SelectorOffset,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavWidget_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavWidget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavWidget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavWidget_Statics::ClassParams = {
	&UUINavWidget::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UUINavWidget_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavWidget_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavWidget_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavWidget_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavWidget()
{
	if (!Z_Registration_Info_UClass_UUINavWidget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavWidget.OuterSingleton, Z_Construct_UClass_UUINavWidget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavWidget.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavWidget>()
{
	return UUINavWidget::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavWidget);
UUINavWidget::~UUINavWidget() {}
// End Class UUINavWidget

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavWidget, UUINavWidget::StaticClass, TEXT("UUINavWidget"), &Z_Registration_Info_UClass_UUINavWidget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavWidget), 2344659352U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_1834643863(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
