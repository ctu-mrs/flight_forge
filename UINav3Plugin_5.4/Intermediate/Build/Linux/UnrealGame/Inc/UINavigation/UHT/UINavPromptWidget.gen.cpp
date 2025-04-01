// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavPromptWidget.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavPromptWidget() {}

// Begin Cross Module References
UINAVIGATION_API UClass* Z_Construct_UClass_UPromptDataBase_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavComponent_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPromptWidget();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPromptWidget_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavWidget();
UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature();
UMG_API UClass* Z_Construct_UClass_URichTextBlock_NoRegister();
UMG_API UClass* Z_Construct_UClass_UTextBlock_NoRegister();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavPromptWidget Function ExecuteCallback
struct Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics
{
	struct UINavPromptWidget_eventExecuteCallback_Parms
	{
		UPromptDataBase* InPromptData;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavPromptWidget" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InPromptData;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::NewProp_InPromptData = { "InPromptData", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPromptWidget_eventExecuteCallback_Parms, InPromptData), Z_Construct_UClass_UPromptDataBase_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::NewProp_InPromptData,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPromptWidget, nullptr, "ExecuteCallback", nullptr, nullptr, Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::UINavPromptWidget_eventExecuteCallback_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::UINavPromptWidget_eventExecuteCallback_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPromptWidget::execExecuteCallback)
{
	P_GET_OBJECT(UPromptDataBase,Z_Param_InPromptData);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ExecuteCallback(Z_Param_InPromptData);
	P_NATIVE_END;
}
// End Class UUINavPromptWidget Function ExecuteCallback

// Begin Class UUINavPromptWidget Function IsAcceptComponent
struct Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics
{
	struct UINavPromptWidget_eventIsAcceptComponent_Parms
	{
		UUINavComponent* Component;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavPromptWidget" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPromptWidget_eventIsAcceptComponent_Parms, Component), Z_Construct_UClass_UUINavComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
void Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPromptWidget_eventIsAcceptComponent_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPromptWidget_eventIsAcceptComponent_Parms), &Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::NewProp_Component,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPromptWidget, nullptr, "IsAcceptComponent", nullptr, nullptr, Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::UINavPromptWidget_eventIsAcceptComponent_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::UINavPromptWidget_eventIsAcceptComponent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPromptWidget::execIsAcceptComponent)
{
	P_GET_OBJECT(UUINavComponent,Z_Param_Component);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsAcceptComponent(Z_Param_Component);
	P_NATIVE_END;
}
// End Class UUINavPromptWidget Function IsAcceptComponent

// Begin Class UUINavPromptWidget Function ProcessPromptWidgetSelected
struct UINavPromptWidget_eventProcessPromptWidgetSelected_Parms
{
	UPromptDataBase* InPromptData;
};
static FName NAME_UUINavPromptWidget_ProcessPromptWidgetSelected = FName(TEXT("ProcessPromptWidgetSelected"));
void UUINavPromptWidget::ProcessPromptWidgetSelected(UPromptDataBase* InPromptData)
{
	UINavPromptWidget_eventProcessPromptWidgetSelected_Parms Parms;
	Parms.InPromptData=InPromptData;
	ProcessEvent(FindFunctionChecked(NAME_UUINavPromptWidget_ProcessPromptWidgetSelected),&Parms);
}
struct Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavPromptWidget" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InPromptData;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics::NewProp_InPromptData = { "InPromptData", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPromptWidget_eventProcessPromptWidgetSelected_Parms, InPromptData), Z_Construct_UClass_UPromptDataBase_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics::NewProp_InPromptData,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPromptWidget, nullptr, "ProcessPromptWidgetSelected", nullptr, nullptr, Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics::PropPointers), sizeof(UINavPromptWidget_eventProcessPromptWidgetSelected_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavPromptWidget_eventProcessPromptWidgetSelected_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPromptWidget::execProcessPromptWidgetSelected)
{
	P_GET_OBJECT(UPromptDataBase,Z_Param_InPromptData);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ProcessPromptWidgetSelected_Implementation(Z_Param_InPromptData);
	P_NATIVE_END;
}
// End Class UUINavPromptWidget Function ProcessPromptWidgetSelected

// Begin Class UUINavPromptWidget Function SetCallback
struct Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics
{
	struct UINavPromptWidget_eventSetCallback_Parms
	{
		FScriptDelegate InCallback;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavPromptWidget" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InCallback_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FDelegatePropertyParams NewProp_InCallback;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FDelegatePropertyParams Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::NewProp_InCallback = { "InCallback", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Delegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPromptWidget_eventSetCallback_Parms, InCallback), Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InCallback_MetaData), NewProp_InCallback_MetaData) }; // 68500594
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::NewProp_InCallback,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPromptWidget, nullptr, "SetCallback", nullptr, nullptr, Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::UINavPromptWidget_eventSetCallback_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::UINavPromptWidget_eventSetCallback_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPromptWidget_SetCallback()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPromptWidget_SetCallback_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPromptWidget::execSetCallback)
{
	P_GET_PROPERTY_REF(FDelegateProperty,Z_Param_Out_InCallback);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetCallback(FPromptWidgetDecided(Z_Param_Out_InCallback));
	P_NATIVE_END;
}
// End Class UUINavPromptWidget Function SetCallback

// Begin Class UUINavPromptWidget
void UUINavPromptWidget::StaticRegisterNativesUUINavPromptWidget()
{
	UClass* Class = UUINavPromptWidget::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "ExecuteCallback", &UUINavPromptWidget::execExecuteCallback },
		{ "IsAcceptComponent", &UUINavPromptWidget::execIsAcceptComponent },
		{ "ProcessPromptWidgetSelected", &UUINavPromptWidget::execProcessPromptWidgetSelected },
		{ "SetCallback", &UUINavPromptWidget::execSetCallback },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavPromptWidget);
UClass* Z_Construct_UClass_UUINavPromptWidget_NoRegister()
{
	return UUINavPromptWidget::StaticClass();
}
struct Z_Construct_UClass_UUINavPromptWidget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "UINavPromptWidget.h" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Title_MetaData[] = {
		{ "Category", "UINavPromptWidget" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Message_MetaData[] = {
		{ "Category", "UINavPromptWidget" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FirstComponentIsAccept_MetaData[] = {
		{ "Category", "UINavPromptWidget" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TitleStyleRowName_MetaData[] = {
		{ "Category", "UINavPromptWidget" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MessageStyleRowName_MetaData[] = {
		{ "Category", "UINavPromptWidget" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TitleText_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavPromptWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
		{ "OptionalWidget", "TRUE" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MessageText_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavPromptWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
		{ "OptionalWidget", "TRUE" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TitleRichText_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavPromptWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
		{ "OptionalWidget", "TRUE" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MessageRichText_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavPromptWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavPromptWidget.h" },
		{ "OptionalWidget", "TRUE" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_Title;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Message;
	static void NewProp_FirstComponentIsAccept_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_FirstComponentIsAccept;
	static const UECodeGen_Private::FStrPropertyParams NewProp_TitleStyleRowName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_MessageStyleRowName;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TitleText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MessageText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TitleRichText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MessageRichText;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUINavPromptWidget_ExecuteCallback, "ExecuteCallback" }, // 4203928112
		{ &Z_Construct_UFunction_UUINavPromptWidget_IsAcceptComponent, "IsAcceptComponent" }, // 2030687762
		{ &Z_Construct_UFunction_UUINavPromptWidget_ProcessPromptWidgetSelected, "ProcessPromptWidgetSelected" }, // 3683503869
		{ &Z_Construct_UFunction_UUINavPromptWidget_SetCallback, "SetCallback" }, // 527646149
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavPromptWidget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FTextPropertyParams Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_Title = { "Title", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPromptWidget, Title), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Title_MetaData), NewProp_Title_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_Message = { "Message", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPromptWidget, Message), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Message_MetaData), NewProp_Message_MetaData) };
void Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_FirstComponentIsAccept_SetBit(void* Obj)
{
	((UUINavPromptWidget*)Obj)->FirstComponentIsAccept = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_FirstComponentIsAccept = { "FirstComponentIsAccept", nullptr, (EPropertyFlags)0x0020080000010001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavPromptWidget), &Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_FirstComponentIsAccept_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FirstComponentIsAccept_MetaData), NewProp_FirstComponentIsAccept_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_TitleStyleRowName = { "TitleStyleRowName", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPromptWidget, TitleStyleRowName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TitleStyleRowName_MetaData), NewProp_TitleStyleRowName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_MessageStyleRowName = { "MessageStyleRowName", nullptr, (EPropertyFlags)0x0020080000000015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPromptWidget, MessageStyleRowName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MessageStyleRowName_MetaData), NewProp_MessageStyleRowName_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_TitleText = { "TitleText", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPromptWidget, TitleText), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TitleText_MetaData), NewProp_TitleText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_MessageText = { "MessageText", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPromptWidget, MessageText), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MessageText_MetaData), NewProp_MessageText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_TitleRichText = { "TitleRichText", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPromptWidget, TitleRichText), Z_Construct_UClass_URichTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TitleRichText_MetaData), NewProp_TitleRichText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_MessageRichText = { "MessageRichText", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPromptWidget, MessageRichText), Z_Construct_UClass_URichTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MessageRichText_MetaData), NewProp_MessageRichText_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavPromptWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_Title,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_Message,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_FirstComponentIsAccept,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_TitleStyleRowName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_MessageStyleRowName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_TitleText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_MessageText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_TitleRichText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPromptWidget_Statics::NewProp_MessageRichText,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavPromptWidget_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavPromptWidget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUINavWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavPromptWidget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavPromptWidget_Statics::ClassParams = {
	&UUINavPromptWidget::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UUINavPromptWidget_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavPromptWidget_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavPromptWidget_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavPromptWidget_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavPromptWidget()
{
	if (!Z_Registration_Info_UClass_UUINavPromptWidget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavPromptWidget.OuterSingleton, Z_Construct_UClass_UUINavPromptWidget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavPromptWidget.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavPromptWidget>()
{
	return UUINavPromptWidget::StaticClass();
}
UUINavPromptWidget::UUINavPromptWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavPromptWidget);
UUINavPromptWidget::~UUINavPromptWidget() {}
// End Class UUINavPromptWidget

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavPromptWidget, UUINavPromptWidget::StaticClass, TEXT("UUINavPromptWidget"), &Z_Registration_Info_UClass_UUINavPromptWidget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavPromptWidget), 3585012517U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_1942201155(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
