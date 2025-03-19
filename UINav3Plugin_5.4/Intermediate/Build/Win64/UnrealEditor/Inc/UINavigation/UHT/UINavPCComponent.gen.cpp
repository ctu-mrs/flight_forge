// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavPCComponent.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"
#include "UINavigation/Public/Data/PlatformConfigData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavPCComponent() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
ENGINE_API UClass* Z_Construct_UClass_APlayerController_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
ENGINE_API UClass* Z_Construct_UClass_UCurveFloat_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UDataTable_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UTexture2D_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UEnhancedInputComponent_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
INPUTCORE_API UScriptStruct* Z_Construct_UScriptStruct_FKey();
SLATECORE_API UEnum* Z_Construct_UEnum_SlateCore_EUINavigation();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavInputBox_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPCComponent();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPCComponent_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPromptWidget_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavWidget_NoRegister();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EAutoHideMouse();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EAxisType();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputAxis();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputMode();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputRestriction();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputType();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EThumbstickAsMouse();
UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature();
UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature();
UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_UpdateInputIconsDelegate__DelegateSignature();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FAxis2D_Keys();
UINAVIGATION_API UScriptStruct* Z_Construct_UScriptStruct_FPlatformConfigData();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Delegate FInputTypeChangedDelegate
struct Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics
{
	struct _Script_UINavigation_eventInputTypeChangedDelegate_Parms
	{
		EInputType InputType;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_InputType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InputType;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::NewProp_InputType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::NewProp_InputType = { "InputType", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_UINavigation_eventInputTypeChangedDelegate_Parms, InputType), Z_Construct_UEnum_UINavigation_EInputType, METADATA_PARAMS(0, nullptr) }; // 346009216
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::NewProp_InputType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::NewProp_InputType,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_UINavigation, nullptr, "InputTypeChangedDelegate__DelegateSignature", nullptr, nullptr, Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::_Script_UINavigation_eventInputTypeChangedDelegate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::_Script_UINavigation_eventInputTypeChangedDelegate_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FInputTypeChangedDelegate_DelegateWrapper(const FMulticastScriptDelegate& InputTypeChangedDelegate, EInputType InputType)
{
	struct _Script_UINavigation_eventInputTypeChangedDelegate_Parms
	{
		EInputType InputType;
	};
	_Script_UINavigation_eventInputTypeChangedDelegate_Parms Parms;
	Parms.InputType=InputType;
	InputTypeChangedDelegate.ProcessMulticastDelegate<UObject>(&Parms);
}
// End Delegate FInputTypeChangedDelegate

// Begin Delegate FUpdateInputIconsDelegate
struct Z_Construct_UDelegateFunction_UINavigation_UpdateInputIconsDelegate__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_UINavigation_UpdateInputIconsDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_UINavigation, nullptr, "UpdateInputIconsDelegate__DelegateSignature", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UINavigation_UpdateInputIconsDelegate__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UINavigation_UpdateInputIconsDelegate__DelegateSignature_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UDelegateFunction_UINavigation_UpdateInputIconsDelegate__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UINavigation_UpdateInputIconsDelegate__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FUpdateInputIconsDelegate_DelegateWrapper(const FMulticastScriptDelegate& UpdateInputIconsDelegate)
{
	UpdateInputIconsDelegate.ProcessMulticastDelegate<UObject>(NULL);
}
// End Delegate FUpdateInputIconsDelegate

// Begin ScriptStruct FAxis2D_Keys
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_Axis2D_Keys;
class UScriptStruct* FAxis2D_Keys::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_Axis2D_Keys.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_Axis2D_Keys.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FAxis2D_Keys, (UObject*)Z_Construct_UPackage__Script_UINavigation(), TEXT("Axis2D_Keys"));
	}
	return Z_Registration_Info_UScriptStruct_Axis2D_Keys.OuterSingleton;
}
template<> UINAVIGATION_API UScriptStruct* StaticStruct<FAxis2D_Keys>()
{
	return FAxis2D_Keys::StaticStruct();
}
struct Z_Construct_UScriptStruct_FAxis2D_Keys_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FAxis2D_Keys>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FAxis2D_Keys_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
	nullptr,
	&NewStructOps,
	"Axis2D_Keys",
	nullptr,
	0,
	sizeof(FAxis2D_Keys),
	alignof(FAxis2D_Keys),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxis2D_Keys_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FAxis2D_Keys_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FAxis2D_Keys()
{
	if (!Z_Registration_Info_UScriptStruct_Axis2D_Keys.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_Axis2D_Keys.InnerSingleton, Z_Construct_UScriptStruct_FAxis2D_Keys_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_Axis2D_Keys.InnerSingleton;
}
// End ScriptStruct FAxis2D_Keys

// Begin Class UUINavPCComponent Function AddInputContext
struct Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics
{
	struct UINavPCComponent_eventAddInputContext_Parms
	{
		const UInputMappingContext* Context;
		int32 Priority;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "CPP_Default_Priority", "0" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Priority_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Context;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Priority;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventAddInputContext_Parms, Context), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::NewProp_Priority = { "Priority", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventAddInputContext_Parms, Priority), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Priority_MetaData), NewProp_Priority_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::NewProp_Context,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::NewProp_Priority,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "AddInputContext", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::UINavPCComponent_eventAddInputContext_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::UINavPCComponent_eventAddInputContext_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_AddInputContext()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_AddInputContext_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execAddInputContext)
{
	P_GET_OBJECT(UInputMappingContext,Z_Param_Context);
	P_GET_PROPERTY(FIntProperty,Z_Param_Priority);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AddInputContext(Z_Param_Context,Z_Param_Priority);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function AddInputContext

// Begin Class UUINavPCComponent Function AllowsAllMenuInput
struct Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics
{
	struct UINavPCComponent_eventAllowsAllMenuInput_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventAllowsAllMenuInput_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventAllowsAllMenuInput_Parms), &Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "AllowsAllMenuInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::UINavPCComponent_eventAllowsAllMenuInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::UINavPCComponent_eventAllowsAllMenuInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execAllowsAllMenuInput)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->AllowsAllMenuInput();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function AllowsAllMenuInput

// Begin Class UUINavPCComponent Function AllowsDirectionalInput
struct Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics
{
	struct UINavPCComponent_eventAllowsDirectionalInput_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventAllowsDirectionalInput_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventAllowsDirectionalInput_Parms), &Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "AllowsDirectionalInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::UINavPCComponent_eventAllowsDirectionalInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::UINavPCComponent_eventAllowsDirectionalInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execAllowsDirectionalInput)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->AllowsDirectionalInput();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function AllowsDirectionalInput

// Begin Class UUINavPCComponent Function AllowsReturnInput
struct Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics
{
	struct UINavPCComponent_eventAllowsReturnInput_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventAllowsReturnInput_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventAllowsReturnInput_Parms), &Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "AllowsReturnInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::UINavPCComponent_eventAllowsReturnInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::UINavPCComponent_eventAllowsReturnInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execAllowsReturnInput)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->AllowsReturnInput();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function AllowsReturnInput

// Begin Class UUINavPCComponent Function AllowsSectionInput
struct Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics
{
	struct UINavPCComponent_eventAllowsSectionInput_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventAllowsSectionInput_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventAllowsSectionInput_Parms), &Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "AllowsSectionInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::UINavPCComponent_eventAllowsSectionInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::UINavPCComponent_eventAllowsSectionInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execAllowsSectionInput)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->AllowsSectionInput();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function AllowsSectionInput

// Begin Class UUINavPCComponent Function AllowsSelectInput
struct Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics
{
	struct UINavPCComponent_eventAllowsSelectInput_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventAllowsSelectInput_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventAllowsSelectInput_Parms), &Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "AllowsSelectInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::UINavPCComponent_eventAllowsSelectInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::UINavPCComponent_eventAllowsSelectInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execAllowsSelectInput)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->AllowsSelectInput();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function AllowsSelectInput

// Begin Class UUINavPCComponent Function ForceUpdateAllInputDisplays
struct Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics
{
	struct UINavPCComponent_eventForceUpdateAllInputDisplays_Parms
	{
		bool bOnlyTopLevel;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09*\x09""Fetches all UINavInputDisplays and forces them to update their visuals.\n\x09* \n\x09*\x09@param bOnlyTopLevel Whether to update only direct children of the viewport.\n\x09*/" },
#endif
		{ "CPP_Default_bOnlyTopLevel", "false" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "*       Fetches all UINavInputDisplays and forces them to update their visuals.\n*\n*       @param bOnlyTopLevel Whether to update only direct children of the viewport." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bOnlyTopLevel_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bOnlyTopLevel_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bOnlyTopLevel;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::NewProp_bOnlyTopLevel_SetBit(void* Obj)
{
	((UINavPCComponent_eventForceUpdateAllInputDisplays_Parms*)Obj)->bOnlyTopLevel = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::NewProp_bOnlyTopLevel = { "bOnlyTopLevel", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventForceUpdateAllInputDisplays_Parms), &Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::NewProp_bOnlyTopLevel_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bOnlyTopLevel_MetaData), NewProp_bOnlyTopLevel_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::NewProp_bOnlyTopLevel,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "ForceUpdateAllInputDisplays", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::UINavPCComponent_eventForceUpdateAllInputDisplays_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::UINavPCComponent_eventForceUpdateAllInputDisplays_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execForceUpdateAllInputDisplays)
{
	P_GET_UBOOL(Z_Param_bOnlyTopLevel);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ForceUpdateAllInputDisplays(Z_Param_bOnlyTopLevel);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function ForceUpdateAllInputDisplays

// Begin Class UUINavPCComponent Function GetActiveSubWidget
struct Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics
{
	struct UINavPCComponent_eventGetActiveSubWidget_Parms
	{
		UUINavWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetActiveSubWidget_Parms, ReturnValue), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetActiveSubWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::UINavPCComponent_eventGetActiveSubWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::UINavPCComponent_eventGetActiveSubWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetActiveSubWidget)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UUINavWidget**)Z_Param__Result=P_THIS->GetActiveSubWidget();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetActiveSubWidget

// Begin Class UUINavPCComponent Function GetActiveWidget
struct Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics
{
	struct UINavPCComponent_eventGetActiveWidget_Parms
	{
		UUINavWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetActiveWidget_Parms, ReturnValue), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetActiveWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::UINavPCComponent_eventGetActiveWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::UINavPCComponent_eventGetActiveWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetActiveWidget)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UUINavWidget**)Z_Param__Result=P_THIS->GetActiveWidget();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetActiveWidget

// Begin Class UUINavPCComponent Function GetAxis1DFromAxis2D
struct Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics
{
	struct UINavPCComponent_eventGetAxis1DFromAxis2D_Parms
	{
		FKey Key;
		EInputAxis Axis;
		FKey ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Axis_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Axis_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Axis;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetAxis1DFromAxis2D_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::NewProp_Axis_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::NewProp_Axis = { "Axis", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetAxis1DFromAxis2D_Parms, Axis), Z_Construct_UEnum_UINavigation_EInputAxis, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Axis_MetaData), NewProp_Axis_MetaData) }; // 2063529479
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000582, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetAxis1DFromAxis2D_Parms, ReturnValue), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) }; // 658672854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::NewProp_Axis_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::NewProp_Axis,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetAxis1DFromAxis2D", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::UINavPCComponent_eventGetAxis1DFromAxis2D_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::UINavPCComponent_eventGetAxis1DFromAxis2D_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetAxis1DFromAxis2D)
{
	P_GET_STRUCT_REF(FKey,Z_Param_Out_Key);
	P_GET_ENUM(EInputAxis,Z_Param_Axis);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FKey*)Z_Param__Result=P_THIS->GetAxis1DFromAxis2D(Z_Param_Out_Key,EInputAxis(Z_Param_Axis));
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetAxis1DFromAxis2D

// Begin Class UUINavPCComponent Function GetAxis2DFromAxis1D
struct Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics
{
	struct UINavPCComponent_eventGetAxis2DFromAxis1D_Parms
	{
		FKey Key;
		FKey ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetAxis2DFromAxis1D_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000582, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetAxis2DFromAxis1D_Parms, ReturnValue), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) }; // 658672854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetAxis2DFromAxis1D", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::UINavPCComponent_eventGetAxis2DFromAxis1D_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::UINavPCComponent_eventGetAxis2DFromAxis1D_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetAxis2DFromAxis1D)
{
	P_GET_STRUCT_REF(FKey,Z_Param_Out_Key);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FKey*)Z_Param__Result=P_THIS->GetAxis2DFromAxis1D(Z_Param_Out_Key);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetAxis2DFromAxis1D

// Begin Class UUINavPCComponent Function GetAxisFromKey
struct Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics
{
	struct UINavPCComponent_eventGetAxisFromKey_Parms
	{
		FKey Key;
		FKey ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetAxisFromKey_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000582, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetAxisFromKey_Parms, ReturnValue), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) }; // 658672854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetAxisFromKey", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::UINavPCComponent_eventGetAxisFromKey_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::UINavPCComponent_eventGetAxisFromKey_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetAxisFromKey)
{
	P_GET_STRUCT_REF(FKey,Z_Param_Out_Key);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FKey*)Z_Param__Result=P_THIS->GetAxisFromKey(Z_Param_Out_Key);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetAxisFromKey

// Begin Class UUINavPCComponent Function GetAxisFromScaledKey
struct Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics
{
	struct UINavPCComponent_eventGetAxisFromScaledKey_Parms
	{
		FKey Key;
		bool bInclude1DAxis;
		bool OutbPositive;
		FKey ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bInclude1DAxis_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static void NewProp_bInclude1DAxis_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bInclude1DAxis;
	static void NewProp_OutbPositive_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_OutbPositive;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetAxisFromScaledKey_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
void Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_bInclude1DAxis_SetBit(void* Obj)
{
	((UINavPCComponent_eventGetAxisFromScaledKey_Parms*)Obj)->bInclude1DAxis = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_bInclude1DAxis = { "bInclude1DAxis", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventGetAxisFromScaledKey_Parms), &Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_bInclude1DAxis_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bInclude1DAxis_MetaData), NewProp_bInclude1DAxis_MetaData) };
void Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_OutbPositive_SetBit(void* Obj)
{
	((UINavPCComponent_eventGetAxisFromScaledKey_Parms*)Obj)->OutbPositive = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_OutbPositive = { "OutbPositive", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventGetAxisFromScaledKey_Parms), &Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_OutbPositive_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000582, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetAxisFromScaledKey_Parms, ReturnValue), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) }; // 658672854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_bInclude1DAxis,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_OutbPositive,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetAxisFromScaledKey", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::UINavPCComponent_eventGetAxisFromScaledKey_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::UINavPCComponent_eventGetAxisFromScaledKey_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetAxisFromScaledKey)
{
	P_GET_STRUCT_REF(FKey,Z_Param_Out_Key);
	P_GET_UBOOL(Z_Param_bInclude1DAxis);
	P_GET_UBOOL_REF(Z_Param_Out_OutbPositive);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FKey*)Z_Param__Result=P_THIS->GetAxisFromScaledKey(Z_Param_Out_Key,Z_Param_bInclude1DAxis,Z_Param_Out_OutbPositive);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetAxisFromScaledKey

// Begin Class UUINavPCComponent Function GetCurrentInputType
struct Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics
{
	struct UINavPCComponent_eventGetCurrentInputType_Parms
	{
		EInputType ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetCurrentInputType_Parms, ReturnValue), Z_Construct_UEnum_UINavigation_EInputType, METADATA_PARAMS(0, nullptr) }; // 346009216
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetCurrentInputType", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::UINavPCComponent_eventGetCurrentInputType_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::UINavPCComponent_eventGetCurrentInputType_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetCurrentInputType)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(EInputType*)Z_Param__Result=P_THIS->GetCurrentInputType();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetCurrentInputType

// Begin Class UUINavPCComponent Function GetCurrentPlatformData
struct Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics
{
	struct UINavPCComponent_eventGetCurrentPlatformData_Parms
	{
		FPlatformConfigData ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000008000582, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetCurrentPlatformData_Parms, ReturnValue), Z_Construct_UScriptStruct_FPlatformConfigData, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) }; // 232835755
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetCurrentPlatformData", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::UINavPCComponent_eventGetCurrentPlatformData_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::UINavPCComponent_eventGetCurrentPlatformData_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetCurrentPlatformData)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FPlatformConfigData*)Z_Param__Result=P_THIS->GetCurrentPlatformData();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetCurrentPlatformData

// Begin Class UUINavPCComponent Function GetEnhancedInputComponent
struct Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics
{
	struct UINavPCComponent_eventGetEnhancedInputComponent_Parms
	{
		UEnhancedInputComponent* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputComponent_Parms, ReturnValue), Z_Construct_UClass_UEnhancedInputComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetEnhancedInputComponent", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::UINavPCComponent_eventGetEnhancedInputComponent_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::UINavPCComponent_eventGetEnhancedInputComponent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetEnhancedInputComponent)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UEnhancedInputComponent**)Z_Param__Result=P_THIS->GetEnhancedInputComponent();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetEnhancedInputComponent

// Begin Class UUINavPCComponent Function GetEnhancedInputIcon
struct Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics
{
	struct UINavPCComponent_eventGetEnhancedInputIcon_Parms
	{
		const UInputAction* Action;
		EInputAxis Axis;
		EAxisType Scale;
		EInputRestriction InputRestriction;
		UTexture2D* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Get first found Icon associated with the given enhanced input action\n//Will search the icon table\n" },
#endif
		{ "CPP_Default_Axis", "X" },
		{ "CPP_Default_InputRestriction", "None" },
		{ "CPP_Default_Scale", "None" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Get first found Icon associated with the given enhanced input action\nWill search the icon table" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Action_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Axis_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Scale_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputRestriction_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Action;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Axis_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Axis;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Scale_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Scale;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InputRestriction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InputRestriction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_Action = { "Action", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputIcon_Parms, Action), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Action_MetaData), NewProp_Action_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_Axis_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_Axis = { "Axis", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputIcon_Parms, Axis), Z_Construct_UEnum_UINavigation_EInputAxis, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Axis_MetaData), NewProp_Axis_MetaData) }; // 2063529479
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_Scale_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_Scale = { "Scale", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputIcon_Parms, Scale), Z_Construct_UEnum_UINavigation_EAxisType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Scale_MetaData), NewProp_Scale_MetaData) }; // 2012741077
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_InputRestriction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_InputRestriction = { "InputRestriction", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputIcon_Parms, InputRestriction), Z_Construct_UEnum_UINavigation_EInputRestriction, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputRestriction_MetaData), NewProp_InputRestriction_MetaData) }; // 2300298404
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputIcon_Parms, ReturnValue), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_Action,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_Axis_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_Axis,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_Scale_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_Scale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_InputRestriction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_InputRestriction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetEnhancedInputIcon", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::UINavPCComponent_eventGetEnhancedInputIcon_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::UINavPCComponent_eventGetEnhancedInputIcon_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetEnhancedInputIcon)
{
	P_GET_OBJECT(UInputAction,Z_Param_Action);
	P_GET_ENUM(EInputAxis,Z_Param_Axis);
	P_GET_ENUM(EAxisType,Z_Param_Scale);
	P_GET_ENUM(EInputRestriction,Z_Param_InputRestriction);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UTexture2D**)Z_Param__Result=P_THIS->GetEnhancedInputIcon(Z_Param_Action,EInputAxis(Z_Param_Axis),EAxisType(Z_Param_Scale),EInputRestriction(Z_Param_InputRestriction));
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetEnhancedInputIcon

// Begin Class UUINavPCComponent Function GetEnhancedInputKey
struct Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics
{
	struct UINavPCComponent_eventGetEnhancedInputKey_Parms
	{
		const UInputAction* Action;
		EInputAxis Axis;
		EAxisType Scale;
		EInputRestriction InputRestriction;
		FKey ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Receives the action and returns the first key that respects the given restriction.\n" },
#endif
		{ "CPP_Default_Axis", "X" },
		{ "CPP_Default_InputRestriction", "None" },
		{ "CPP_Default_Scale", "None" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Receives the action and returns the first key that respects the given restriction." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Action_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Axis_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Scale_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputRestriction_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Action;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Axis_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Axis;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Scale_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Scale;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InputRestriction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InputRestriction;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_Action = { "Action", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputKey_Parms, Action), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Action_MetaData), NewProp_Action_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_Axis_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_Axis = { "Axis", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputKey_Parms, Axis), Z_Construct_UEnum_UINavigation_EInputAxis, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Axis_MetaData), NewProp_Axis_MetaData) }; // 2063529479
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_Scale_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_Scale = { "Scale", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputKey_Parms, Scale), Z_Construct_UEnum_UINavigation_EAxisType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Scale_MetaData), NewProp_Scale_MetaData) }; // 2012741077
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_InputRestriction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_InputRestriction = { "InputRestriction", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputKey_Parms, InputRestriction), Z_Construct_UEnum_UINavigation_EInputRestriction, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputRestriction_MetaData), NewProp_InputRestriction_MetaData) }; // 2300298404
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputKey_Parms, ReturnValue), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(0, nullptr) }; // 658672854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_Action,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_Axis_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_Axis,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_Scale_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_Scale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_InputRestriction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_InputRestriction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetEnhancedInputKey", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::UINavPCComponent_eventGetEnhancedInputKey_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::UINavPCComponent_eventGetEnhancedInputKey_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetEnhancedInputKey)
{
	P_GET_OBJECT(UInputAction,Z_Param_Action);
	P_GET_ENUM(EInputAxis,Z_Param_Axis);
	P_GET_ENUM(EAxisType,Z_Param_Scale);
	P_GET_ENUM(EInputRestriction,Z_Param_InputRestriction);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FKey*)Z_Param__Result=P_THIS->GetEnhancedInputKey(Z_Param_Action,EInputAxis(Z_Param_Axis),EAxisType(Z_Param_Scale),EInputRestriction(Z_Param_InputRestriction));
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetEnhancedInputKey

// Begin Class UUINavPCComponent Function GetEnhancedInputKeys
struct Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics
{
	struct UINavPCComponent_eventGetEnhancedInputKeys_Parms
	{
		const UInputAction* Action;
		TArray<FKey> OutKeys;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Receives the enhanced input action and returns all the keys associated with that action\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Receives the enhanced input action and returns all the keys associated with that action" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Action_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Action;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutKeys_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_OutKeys;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::NewProp_Action = { "Action", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputKeys_Parms, Action), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Action_MetaData), NewProp_Action_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::NewProp_OutKeys_Inner = { "OutKeys", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(0, nullptr) }; // 658672854
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::NewProp_OutKeys = { "OutKeys", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputKeys_Parms, OutKeys), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 658672854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::NewProp_Action,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::NewProp_OutKeys_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::NewProp_OutKeys,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetEnhancedInputKeys", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::UINavPCComponent_eventGetEnhancedInputKeys_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::UINavPCComponent_eventGetEnhancedInputKeys_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetEnhancedInputKeys)
{
	P_GET_OBJECT(UInputAction,Z_Param_Action);
	P_GET_TARRAY_REF(FKey,Z_Param_Out_OutKeys);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->GetEnhancedInputKeys(Z_Param_Action,Z_Param_Out_OutKeys);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetEnhancedInputKeys

// Begin Class UUINavPCComponent Function GetEnhancedInputText
struct Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics
{
	struct UINavPCComponent_eventGetEnhancedInputText_Parms
	{
		const UInputAction* Action;
		EInputAxis Axis;
		EAxisType Scale;
		EInputRestriction InputRestriction;
		FText ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Get first found Icon associated with the given enhanced input action\n//Will search the icon table\n" },
#endif
		{ "CPP_Default_Axis", "X" },
		{ "CPP_Default_InputRestriction", "None" },
		{ "CPP_Default_Scale", "None" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Get first found Icon associated with the given enhanced input action\nWill search the icon table" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Action_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Axis_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Scale_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputRestriction_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Action;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Axis_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Axis;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Scale_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Scale;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InputRestriction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InputRestriction;
	static const UECodeGen_Private::FTextPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_Action = { "Action", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputText_Parms, Action), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Action_MetaData), NewProp_Action_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_Axis_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_Axis = { "Axis", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputText_Parms, Axis), Z_Construct_UEnum_UINavigation_EInputAxis, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Axis_MetaData), NewProp_Axis_MetaData) }; // 2063529479
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_Scale_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_Scale = { "Scale", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputText_Parms, Scale), Z_Construct_UEnum_UINavigation_EAxisType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Scale_MetaData), NewProp_Scale_MetaData) }; // 2012741077
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_InputRestriction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_InputRestriction = { "InputRestriction", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputText_Parms, InputRestriction), Z_Construct_UEnum_UINavigation_EInputRestriction, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputRestriction_MetaData), NewProp_InputRestriction_MetaData) }; // 2300298404
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetEnhancedInputText_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_Action,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_Axis_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_Axis,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_Scale_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_Scale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_InputRestriction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_InputRestriction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetEnhancedInputText", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::UINavPCComponent_eventGetEnhancedInputText_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::UINavPCComponent_eventGetEnhancedInputText_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetEnhancedInputText)
{
	P_GET_OBJECT(UInputAction,Z_Param_Action);
	P_GET_ENUM(EInputAxis,Z_Param_Axis);
	P_GET_ENUM(EAxisType,Z_Param_Scale);
	P_GET_ENUM(EInputRestriction,Z_Param_InputRestriction);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FText*)Z_Param__Result=P_THIS->GetEnhancedInputText(Z_Param_Action,EInputAxis(Z_Param_Axis),EAxisType(Z_Param_Scale),EInputRestriction(Z_Param_InputRestriction));
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetEnhancedInputText

// Begin Class UUINavPCComponent Function GetInputMode
struct Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics
{
	struct UINavPCComponent_eventGetInputMode_Parms
	{
		EInputMode ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Returns the currently used input mode\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Returns the currently used input mode" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetInputMode_Parms, ReturnValue), Z_Construct_UEnum_UINavigation_EInputMode, METADATA_PARAMS(0, nullptr) }; // 2006005193
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetInputMode", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::UINavPCComponent_eventGetInputMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::UINavPCComponent_eventGetInputMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetInputMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetInputMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetInputMode)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(EInputMode*)Z_Param__Result=P_THIS->GetInputMode();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetInputMode

// Begin Class UUINavPCComponent Function GetKeyFromAxis
struct Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics
{
	struct UINavPCComponent_eventGetKeyFromAxis_Parms
	{
		FKey Key;
		bool bPositive;
		EInputAxis Axis;
		FKey ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "CPP_Default_Axis", "X" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Axis_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static void NewProp_bPositive_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bPositive;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Axis_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Axis;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetKeyFromAxis_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
void Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_bPositive_SetBit(void* Obj)
{
	((UINavPCComponent_eventGetKeyFromAxis_Parms*)Obj)->bPositive = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_bPositive = { "bPositive", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventGetKeyFromAxis_Parms), &Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_bPositive_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_Axis_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_Axis = { "Axis", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetKeyFromAxis_Parms, Axis), Z_Construct_UEnum_UINavigation_EInputAxis, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Axis_MetaData), NewProp_Axis_MetaData) }; // 2063529479
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000582, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetKeyFromAxis_Parms, ReturnValue), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) }; // 658672854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_bPositive,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_Axis_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_Axis,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetKeyFromAxis", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::UINavPCComponent_eventGetKeyFromAxis_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::UINavPCComponent_eventGetKeyFromAxis_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetKeyFromAxis)
{
	P_GET_STRUCT_REF(FKey,Z_Param_Out_Key);
	P_GET_UBOOL(Z_Param_bPositive);
	P_GET_ENUM(EInputAxis,Z_Param_Axis);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FKey*)Z_Param__Result=P_THIS->GetKeyFromAxis(Z_Param_Out_Key,Z_Param_bPositive,EInputAxis(Z_Param_Axis));
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetKeyFromAxis

// Begin Class UUINavPCComponent Function GetKeyIcon
struct Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics
{
	struct UINavPCComponent_eventGetKeyIcon_Parms
	{
		FKey Key;
		UTexture2D* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetKeyIcon_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetKeyIcon_Parms, ReturnValue), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetKeyIcon", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::UINavPCComponent_eventGetKeyIcon_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::UINavPCComponent_eventGetKeyIcon_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetKeyIcon)
{
	P_GET_STRUCT(FKey,Z_Param_Key);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UTexture2D**)Z_Param__Result=P_THIS->GetKeyIcon(Z_Param_Key);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetKeyIcon

// Begin Class UUINavPCComponent Function GetKeyText
struct Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics
{
	struct UINavPCComponent_eventGetKeyText_Parms
	{
		FKey Key;
		FText ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static const UECodeGen_Private::FTextPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetKeyText_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetKeyText_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetKeyText", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::UINavPCComponent_eventGetKeyText_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::UINavPCComponent_eventGetKeyText_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetKeyText()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetKeyText_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetKeyText)
{
	P_GET_STRUCT(FKey,Z_Param_Key);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FText*)Z_Param__Result=P_THIS->GetKeyText(Z_Param_Key);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetKeyText

// Begin Class UUINavPCComponent Function GetOppositeAxis2DAxis
struct Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics
{
	struct UINavPCComponent_eventGetOppositeAxis2DAxis_Parms
	{
		FKey Key;
		FKey ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetOppositeAxis2DAxis_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000582, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetOppositeAxis2DAxis_Parms, ReturnValue), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) }; // 658672854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetOppositeAxis2DAxis", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::UINavPCComponent_eventGetOppositeAxis2DAxis_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::UINavPCComponent_eventGetOppositeAxis2DAxis_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetOppositeAxis2DAxis)
{
	P_GET_STRUCT_REF(FKey,Z_Param_Out_Key);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FKey*)Z_Param__Result=P_THIS->GetOppositeAxis2DAxis(Z_Param_Out_Key);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetOppositeAxis2DAxis

// Begin Class UUINavPCComponent Function GetOppositeAxisKey
struct Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics
{
	struct UINavPCComponent_eventGetOppositeAxisKey_Parms
	{
		FKey Key;
		bool bOutIsPositive;
		FKey ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static void NewProp_bOutIsPositive_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bOutIsPositive;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetOppositeAxisKey_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
void Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::NewProp_bOutIsPositive_SetBit(void* Obj)
{
	((UINavPCComponent_eventGetOppositeAxisKey_Parms*)Obj)->bOutIsPositive = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::NewProp_bOutIsPositive = { "bOutIsPositive", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventGetOppositeAxisKey_Parms), &Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::NewProp_bOutIsPositive_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000582, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetOppositeAxisKey_Parms, ReturnValue), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) }; // 658672854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::NewProp_bOutIsPositive,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetOppositeAxisKey", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::UINavPCComponent_eventGetOppositeAxisKey_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::UINavPCComponent_eventGetOppositeAxisKey_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetOppositeAxisKey)
{
	P_GET_STRUCT_REF(FKey,Z_Param_Out_Key);
	P_GET_UBOOL_REF(Z_Param_Out_bOutIsPositive);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FKey*)Z_Param__Result=P_THIS->GetOppositeAxisKey(Z_Param_Out_Key,Z_Param_Out_bOutIsPositive);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetOppositeAxisKey

// Begin Class UUINavPCComponent Function GetPC
struct Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics
{
	struct UINavPCComponent_eventGetPC_Parms
	{
		APlayerController* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetPC_Parms, ReturnValue), Z_Construct_UClass_APlayerController_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetPC", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::UINavPCComponent_eventGetPC_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::UINavPCComponent_eventGetPC_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetPC()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetPC_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetPC)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(APlayerController**)Z_Param__Result=P_THIS->GetPC();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetPC

// Begin Class UUINavPCComponent Function GetSoftEnhancedInputIcon
struct Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics
{
	struct UINavPCComponent_eventGetSoftEnhancedInputIcon_Parms
	{
		const UInputAction* Action;
		EInputAxis Axis;
		EAxisType Scale;
		EInputRestriction InputRestriction;
		TSoftObjectPtr<UTexture2D> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Get first found Icon associated with the given enhanced input action\n//Will search the icon table\n" },
#endif
		{ "CPP_Default_Axis", "X" },
		{ "CPP_Default_InputRestriction", "None" },
		{ "CPP_Default_Scale", "None" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Get first found Icon associated with the given enhanced input action\nWill search the icon table" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Action_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Axis_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Scale_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputRestriction_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Action;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Axis_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Axis;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Scale_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Scale;
	static const UECodeGen_Private::FBytePropertyParams NewProp_InputRestriction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_InputRestriction;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_Action = { "Action", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetSoftEnhancedInputIcon_Parms, Action), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Action_MetaData), NewProp_Action_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_Axis_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_Axis = { "Axis", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetSoftEnhancedInputIcon_Parms, Axis), Z_Construct_UEnum_UINavigation_EInputAxis, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Axis_MetaData), NewProp_Axis_MetaData) }; // 2063529479
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_Scale_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_Scale = { "Scale", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetSoftEnhancedInputIcon_Parms, Scale), Z_Construct_UEnum_UINavigation_EAxisType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Scale_MetaData), NewProp_Scale_MetaData) }; // 2012741077
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_InputRestriction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_InputRestriction = { "InputRestriction", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetSoftEnhancedInputIcon_Parms, InputRestriction), Z_Construct_UEnum_UINavigation_EInputRestriction, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputRestriction_MetaData), NewProp_InputRestriction_MetaData) }; // 2300298404
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0014000000000580, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetSoftEnhancedInputIcon_Parms, ReturnValue), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_Action,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_Axis_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_Axis,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_Scale_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_Scale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_InputRestriction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_InputRestriction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetSoftEnhancedInputIcon", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::UINavPCComponent_eventGetSoftEnhancedInputIcon_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::UINavPCComponent_eventGetSoftEnhancedInputIcon_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetSoftEnhancedInputIcon)
{
	P_GET_OBJECT(UInputAction,Z_Param_Action);
	P_GET_ENUM(EInputAxis,Z_Param_Axis);
	P_GET_ENUM(EAxisType,Z_Param_Scale);
	P_GET_ENUM(EInputRestriction,Z_Param_InputRestriction);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TSoftObjectPtr<UTexture2D>*)Z_Param__Result=P_THIS->GetSoftEnhancedInputIcon(Z_Param_Action,EInputAxis(Z_Param_Axis),EAxisType(Z_Param_Scale),EInputRestriction(Z_Param_InputRestriction));
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetSoftEnhancedInputIcon

// Begin Class UUINavPCComponent Function GetSoftKeyIcon
struct Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics
{
	struct UINavPCComponent_eventGetSoftKeyIcon_Parms
	{
		FKey Key;
		TSoftObjectPtr<UTexture2D> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Key;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetSoftKeyIcon_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0014000000000580, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetSoftKeyIcon_Parms, ReturnValue), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetSoftKeyIcon", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::UINavPCComponent_eventGetSoftKeyIcon_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::UINavPCComponent_eventGetSoftKeyIcon_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetSoftKeyIcon)
{
	P_GET_STRUCT(FKey,Z_Param_Key);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TSoftObjectPtr<UTexture2D>*)Z_Param__Result=P_THIS->GetSoftKeyIcon(Z_Param_Key);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetSoftKeyIcon

// Begin Class UUINavPCComponent Function GetThumbstickDelta
struct Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics
{
	struct UINavPCComponent_eventGetThumbstickDelta_Parms
	{
		FVector2D ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetThumbstickDelta_Parms, ReturnValue), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetThumbstickDelta", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::UINavPCComponent_eventGetThumbstickDelta_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::UINavPCComponent_eventGetThumbstickDelta_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetThumbstickDelta)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FVector2D*)Z_Param__Result=P_THIS->GetThumbstickDelta();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetThumbstickDelta

// Begin Class UUINavPCComponent Function GetUINavInputContext
struct Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics
{
	struct UINavPCComponent_eventGetUINavInputContext_Parms
	{
		UInputMappingContext* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGetUINavInputContext_Parms, ReturnValue), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GetUINavInputContext", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::UINavPCComponent_eventGetUINavInputContext_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::UINavPCComponent_eventGetUINavInputContext_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGetUINavInputContext)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UInputMappingContext**)Z_Param__Result=P_THIS->GetUINavInputContext();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function GetUINavInputContext

// Begin Class UUINavPCComponent Function GoToBuiltWidget
struct Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics
{
	struct UINavPCComponent_eventGoToBuiltWidget_Parms
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
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Adds given widget to screen (strongly recommended over manual alternative)\n\x09*\n\x09*\x09@param\x09NewWidget  Object instance of the UINavWidget to add to the screen\n\x09*\x09@param\x09""bRemoveParent  Whether to remove the parent widget (this widget) from the viewport\n\x09*\x09@param  bDestroyParent  Whether to destruct the parent widget (this widget)\n\x09*\x09@param  ZOrder Order to display the widget\n\x09*/" },
#endif
		{ "CPP_Default_bDestroyParent", "false" },
		{ "CPP_Default_ZOrder", "0" },
		{ "DeterminesOutputType", "NewWidgetClass" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
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
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_NewWidget = { "NewWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToBuiltWidget_Parms, NewWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewWidget_MetaData), NewProp_NewWidget_MetaData) };
void Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_bRemoveParent_SetBit(void* Obj)
{
	((UINavPCComponent_eventGoToBuiltWidget_Parms*)Obj)->bRemoveParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_bRemoveParent = { "bRemoveParent", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventGoToBuiltWidget_Parms), &Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_bRemoveParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRemoveParent_MetaData), NewProp_bRemoveParent_MetaData) };
void Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_bDestroyParent_SetBit(void* Obj)
{
	((UINavPCComponent_eventGoToBuiltWidget_Parms*)Obj)->bDestroyParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_bDestroyParent = { "bDestroyParent", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventGoToBuiltWidget_Parms), &Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_bDestroyParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDestroyParent_MetaData), NewProp_bDestroyParent_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_ZOrder = { "ZOrder", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToBuiltWidget_Parms, ZOrder), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ZOrder_MetaData), NewProp_ZOrder_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToBuiltWidget_Parms, ReturnValue), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_NewWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_bRemoveParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_bDestroyParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_ZOrder,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GoToBuiltWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::UINavPCComponent_eventGoToBuiltWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::UINavPCComponent_eventGoToBuiltWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGoToBuiltWidget)
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
// End Class UUINavPCComponent Function GoToBuiltWidget

// Begin Class UUINavPCComponent Function GoToPromptWidget
struct Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics
{
	struct UINavPCComponent_eventGoToPromptWidget_Parms
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
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
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
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_NewWidgetClass = { "NewWidgetClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToPromptWidget_Parms, NewWidgetClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UUINavPromptWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FDelegatePropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_Event = { "Event", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Delegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToPromptWidget_Parms, Event), Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Event_MetaData), NewProp_Event_MetaData) }; // 68500594
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_Title = { "Title", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToPromptWidget_Parms, Title), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Title_MetaData), NewProp_Title_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_Message = { "Message", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToPromptWidget_Parms, Message), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Message_MetaData), NewProp_Message_MetaData) };
void Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_bRemoveParent_SetBit(void* Obj)
{
	((UINavPCComponent_eventGoToPromptWidget_Parms*)Obj)->bRemoveParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_bRemoveParent = { "bRemoveParent", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventGoToPromptWidget_Parms), &Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_bRemoveParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRemoveParent_MetaData), NewProp_bRemoveParent_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_ZOrder = { "ZOrder", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToPromptWidget_Parms, ZOrder), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ZOrder_MetaData), NewProp_ZOrder_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToPromptWidget_Parms, ReturnValue), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_NewWidgetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_Event,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_Title,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_Message,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_bRemoveParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_ZOrder,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GoToPromptWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::UINavPCComponent_eventGoToPromptWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::UINavPCComponent_eventGoToPromptWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGoToPromptWidget)
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
// End Class UUINavPCComponent Function GoToPromptWidget

// Begin Class UUINavPCComponent Function GoToWidget
struct Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics
{
	struct UINavPCComponent_eventGoToWidget_Parms
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
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Adds given widget to screen (strongly recommended over manual alternative)\n\x09*\n\x09*\x09@param\x09NewWidgetClass  The class of the widget to add to the screen\n\x09*\x09@param\x09""bRemoveParent  Whether to remove the parent widget (this widget) from the viewport\n\x09*\x09@param  bDestroyParent  Whether to destruct the parent widget (this widget)\n\x09*\x09@param  ZOrder Order to display the widget\n\x09*/" },
#endif
		{ "CPP_Default_bDestroyParent", "false" },
		{ "CPP_Default_ZOrder", "0" },
		{ "DeterminesOutputType", "NewWidgetClass" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
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
const UECodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_NewWidgetClass = { "NewWidgetClass", nullptr, (EPropertyFlags)0x0014000000000080, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToWidget_Parms, NewWidgetClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_bRemoveParent_SetBit(void* Obj)
{
	((UINavPCComponent_eventGoToWidget_Parms*)Obj)->bRemoveParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_bRemoveParent = { "bRemoveParent", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventGoToWidget_Parms), &Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_bRemoveParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRemoveParent_MetaData), NewProp_bRemoveParent_MetaData) };
void Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_bDestroyParent_SetBit(void* Obj)
{
	((UINavPCComponent_eventGoToWidget_Parms*)Obj)->bDestroyParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_bDestroyParent = { "bDestroyParent", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventGoToWidget_Parms), &Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_bDestroyParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDestroyParent_MetaData), NewProp_bDestroyParent_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_ZOrder = { "ZOrder", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToWidget_Parms, ZOrder), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ZOrder_MetaData), NewProp_ZOrder_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventGoToWidget_Parms, ReturnValue), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_NewWidgetClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_bRemoveParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_bDestroyParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_ZOrder,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "GoToWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::UINavPCComponent_eventGoToWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::UINavPCComponent_eventGoToWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_GoToWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_GoToWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execGoToWidget)
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
// End Class UUINavPCComponent Function GoToWidget

// Begin Class UUINavPCComponent Function HidingMouseCursor
struct Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics
{
	struct UINavPCComponent_eventHidingMouseCursor_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventHidingMouseCursor_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventHidingMouseCursor_Parms), &Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "HidingMouseCursor", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::UINavPCComponent_eventHidingMouseCursor_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::UINavPCComponent_eventHidingMouseCursor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execHidingMouseCursor)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->HidingMouseCursor();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function HidingMouseCursor

// Begin Class UUINavPCComponent Function IsAxis
struct Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics
{
	struct UINavPCComponent_eventIsAxis_Parms
	{
		FKey Key;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
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
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventIsAxis_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
void Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventIsAxis_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventIsAxis_Parms), &Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "IsAxis", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::UINavPCComponent_eventIsAxis_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::UINavPCComponent_eventIsAxis_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_IsAxis()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_IsAxis_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execIsAxis)
{
	P_GET_STRUCT_REF(FKey,Z_Param_Out_Key);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsAxis(Z_Param_Out_Key);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function IsAxis

// Begin Class UUINavPCComponent Function IsAxis2D
struct Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics
{
	struct UINavPCComponent_eventIsAxis2D_Parms
	{
		FKey Key;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
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
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventIsAxis2D_Parms, Key), Z_Construct_UScriptStruct_FKey, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Key_MetaData), NewProp_Key_MetaData) }; // 658672854
void Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventIsAxis2D_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventIsAxis2D_Parms), &Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::NewProp_Key,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "IsAxis2D", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::UINavPCComponent_eventIsAxis2D_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::UINavPCComponent_eventIsAxis2D_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_IsAxis2D()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_IsAxis2D_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execIsAxis2D)
{
	P_GET_STRUCT_REF(FKey,Z_Param_Out_Key);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsAxis2D(Z_Param_Out_Key);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function IsAxis2D

// Begin Class UUINavPCComponent Function IsMovingThumbstick
struct Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics
{
	struct UINavPCComponent_eventIsMovingThumbstick_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventIsMovingThumbstick_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventIsMovingThumbstick_Parms), &Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "IsMovingThumbstick", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::UINavPCComponent_eventIsMovingThumbstick_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::UINavPCComponent_eventIsMovingThumbstick_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execIsMovingThumbstick)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsMovingThumbstick();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function IsMovingThumbstick

// Begin Class UUINavPCComponent Function IsUsingGamepad
struct Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics
{
	struct UINavPCComponent_eventIsUsingGamepad_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventIsUsingGamepad_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventIsUsingGamepad_Parms), &Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "IsUsingGamepad", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::UINavPCComponent_eventIsUsingGamepad_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::UINavPCComponent_eventIsUsingGamepad_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execIsUsingGamepad)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsUsingGamepad();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function IsUsingGamepad

// Begin Class UUINavPCComponent Function IsUsingKeyboard
struct Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics
{
	struct UINavPCComponent_eventIsUsingKeyboard_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventIsUsingKeyboard_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventIsUsingKeyboard_Parms), &Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "IsUsingKeyboard", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::UINavPCComponent_eventIsUsingKeyboard_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::UINavPCComponent_eventIsUsingKeyboard_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execIsUsingKeyboard)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsUsingKeyboard();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function IsUsingKeyboard

// Begin Class UUINavPCComponent Function IsUsingMouse
struct Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics
{
	struct UINavPCComponent_eventIsUsingMouse_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventIsUsingMouse_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventIsUsingMouse_Parms), &Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "IsUsingMouse", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::UINavPCComponent_eventIsUsingMouse_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::UINavPCComponent_eventIsUsingMouse_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execIsUsingMouse)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsUsingMouse();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function IsUsingMouse

// Begin Class UUINavPCComponent Function IsWidgetActive
struct Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics
{
	struct UINavPCComponent_eventIsWidgetActive_Parms
	{
		const UUINavWidget* UINavWidget;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UINavWidget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UINavWidget;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::NewProp_UINavWidget = { "UINavWidget", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventIsWidgetActive_Parms, UINavWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UINavWidget_MetaData), NewProp_UINavWidget_MetaData) };
void Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventIsWidgetActive_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventIsWidgetActive_Parms), &Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::NewProp_UINavWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "IsWidgetActive", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::UINavPCComponent_eventIsWidgetActive_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::UINavPCComponent_eventIsWidgetActive_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execIsWidgetActive)
{
	P_GET_OBJECT(UUINavWidget,Z_Param_UINavWidget);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsWidgetActive(Z_Param_UINavWidget);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function IsWidgetActive

// Begin Class UUINavPCComponent Function IsWidgetChild
struct Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics
{
	struct UINavPCComponent_eventIsWidgetChild_Parms
	{
		const UUINavWidget* ParentWidget;
		const UUINavWidget* ChildWidget;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ParentWidget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ChildWidget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ParentWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ChildWidget;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::NewProp_ParentWidget = { "ParentWidget", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventIsWidgetChild_Parms, ParentWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ParentWidget_MetaData), NewProp_ParentWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::NewProp_ChildWidget = { "ChildWidget", nullptr, (EPropertyFlags)0x0010000000080082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventIsWidgetChild_Parms, ChildWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ChildWidget_MetaData), NewProp_ChildWidget_MetaData) };
void Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavPCComponent_eventIsWidgetChild_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventIsWidgetChild_Parms), &Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::NewProp_ParentWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::NewProp_ChildWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "IsWidgetChild", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::UINavPCComponent_eventIsWidgetChild_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::UINavPCComponent_eventIsWidgetChild_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execIsWidgetChild)
{
	P_GET_OBJECT(UUINavWidget,Z_Param_ParentWidget);
	P_GET_OBJECT(UUINavWidget,Z_Param_ChildWidget);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsWidgetChild(Z_Param_ParentWidget,Z_Param_ChildWidget);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function IsWidgetChild

// Begin Class UUINavPCComponent Function NavigateInDirection
struct Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics
{
	struct UINavPCComponent_eventNavigateInDirection_Parms
	{
		EUINavigation Direction;
		int32 UserIndex;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "AdvancedDisplay", "1" },
		{ "Category", "UINavController" },
		{ "CPP_Default_UserIndex", "0" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Direction_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UserIndex_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_Direction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Direction;
	static const UECodeGen_Private::FIntPropertyParams NewProp_UserIndex;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::NewProp_Direction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::NewProp_Direction = { "Direction", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventNavigateInDirection_Parms, Direction), Z_Construct_UEnum_SlateCore_EUINavigation, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Direction_MetaData), NewProp_Direction_MetaData) }; // 2993106650
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::NewProp_UserIndex = { "UserIndex", nullptr, (EPropertyFlags)0x0010040000000082, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventNavigateInDirection_Parms, UserIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UserIndex_MetaData), NewProp_UserIndex_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::NewProp_Direction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::NewProp_Direction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::NewProp_UserIndex,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "NavigateInDirection", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::UINavPCComponent_eventNavigateInDirection_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::UINavPCComponent_eventNavigateInDirection_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execNavigateInDirection)
{
	P_GET_ENUM(EUINavigation,Z_Param_Direction);
	P_GET_PROPERTY(FIntProperty,Z_Param_UserIndex);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->NavigateInDirection(EUINavigation(Z_Param_Direction),Z_Param_UserIndex);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function NavigateInDirection

// Begin Class UUINavPCComponent Function NotifyNavigatedTo
struct Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics
{
	struct UINavPCComponent_eventNotifyNavigatedTo_Parms
	{
		UUINavWidget* NavigatedWidget;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NavigatedWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NavigatedWidget;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::NewProp_NavigatedWidget = { "NavigatedWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventNotifyNavigatedTo_Parms, NavigatedWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NavigatedWidget_MetaData), NewProp_NavigatedWidget_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::NewProp_NavigatedWidget,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "NotifyNavigatedTo", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::UINavPCComponent_eventNotifyNavigatedTo_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::UINavPCComponent_eventNotifyNavigatedTo_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execNotifyNavigatedTo)
{
	P_GET_OBJECT(UUINavWidget,Z_Param_NavigatedWidget);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->NotifyNavigatedTo(Z_Param_NavigatedWidget);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function NotifyNavigatedTo

// Begin Class UUINavPCComponent Function RefreshNavigationKeys
struct Z_Construct_UFunction_UUINavPCComponent_RefreshNavigationKeys_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_RefreshNavigationKeys_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "RefreshNavigationKeys", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_RefreshNavigationKeys_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_RefreshNavigationKeys_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavPCComponent_RefreshNavigationKeys()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_RefreshNavigationKeys_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execRefreshNavigationKeys)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RefreshNavigationKeys();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function RefreshNavigationKeys

// Begin Class UUINavPCComponent Function RemoveInputContext
struct Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics
{
	struct UINavPCComponent_eventRemoveInputContext_Parms
	{
		const UInputMappingContext* Context;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventRemoveInputContext_Parms, Context), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "RemoveInputContext", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::UINavPCComponent_eventRemoveInputContext_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::UINavPCComponent_eventRemoveInputContext_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execRemoveInputContext)
{
	P_GET_OBJECT(UInputMappingContext,Z_Param_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->RemoveInputContext(Z_Param_Context);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function RemoveInputContext

// Begin Class UUINavPCComponent Function SetActiveWidget
struct Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics
{
	struct UINavPCComponent_eventSetActiveWidget_Parms
	{
		UUINavWidget* NewActiveWidget;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewActiveWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewActiveWidget;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::NewProp_NewActiveWidget = { "NewActiveWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventSetActiveWidget_Parms, NewActiveWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewActiveWidget_MetaData), NewProp_NewActiveWidget_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::NewProp_NewActiveWidget,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SetActiveWidget", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::UINavPCComponent_eventSetActiveWidget_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::UINavPCComponent_eventSetActiveWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSetActiveWidget)
{
	P_GET_OBJECT(UUINavWidget,Z_Param_NewActiveWidget);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetActiveWidget(Z_Param_NewActiveWidget);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SetActiveWidget

// Begin Class UUINavPCComponent Function SetAllowAllMenuInput
struct Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics
{
	struct UINavPCComponent_eventSetAllowAllMenuInput_Parms
	{
		bool bAllowInput;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAllowInput_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bAllowInput_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAllowInput;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::NewProp_bAllowInput_SetBit(void* Obj)
{
	((UINavPCComponent_eventSetAllowAllMenuInput_Parms*)Obj)->bAllowInput = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::NewProp_bAllowInput = { "bAllowInput", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventSetAllowAllMenuInput_Parms), &Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::NewProp_bAllowInput_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAllowInput_MetaData), NewProp_bAllowInput_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::NewProp_bAllowInput,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SetAllowAllMenuInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::UINavPCComponent_eventSetAllowAllMenuInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::UINavPCComponent_eventSetAllowAllMenuInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSetAllowAllMenuInput)
{
	P_GET_UBOOL(Z_Param_bAllowInput);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetAllowAllMenuInput(Z_Param_bAllowInput);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SetAllowAllMenuInput

// Begin Class UUINavPCComponent Function SetAllowDirectionalInput
struct Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics
{
	struct UINavPCComponent_eventSetAllowDirectionalInput_Parms
	{
		bool bAllowInput;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAllowInput_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bAllowInput_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAllowInput;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::NewProp_bAllowInput_SetBit(void* Obj)
{
	((UINavPCComponent_eventSetAllowDirectionalInput_Parms*)Obj)->bAllowInput = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::NewProp_bAllowInput = { "bAllowInput", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventSetAllowDirectionalInput_Parms), &Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::NewProp_bAllowInput_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAllowInput_MetaData), NewProp_bAllowInput_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::NewProp_bAllowInput,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SetAllowDirectionalInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::UINavPCComponent_eventSetAllowDirectionalInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::UINavPCComponent_eventSetAllowDirectionalInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSetAllowDirectionalInput)
{
	P_GET_UBOOL(Z_Param_bAllowInput);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetAllowDirectionalInput(Z_Param_bAllowInput);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SetAllowDirectionalInput

// Begin Class UUINavPCComponent Function SetAllowReturnInput
struct Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics
{
	struct UINavPCComponent_eventSetAllowReturnInput_Parms
	{
		bool bAllowInput;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAllowInput_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bAllowInput_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAllowInput;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::NewProp_bAllowInput_SetBit(void* Obj)
{
	((UINavPCComponent_eventSetAllowReturnInput_Parms*)Obj)->bAllowInput = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::NewProp_bAllowInput = { "bAllowInput", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventSetAllowReturnInput_Parms), &Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::NewProp_bAllowInput_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAllowInput_MetaData), NewProp_bAllowInput_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::NewProp_bAllowInput,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SetAllowReturnInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::UINavPCComponent_eventSetAllowReturnInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::UINavPCComponent_eventSetAllowReturnInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSetAllowReturnInput)
{
	P_GET_UBOOL(Z_Param_bAllowInput);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetAllowReturnInput(Z_Param_bAllowInput);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SetAllowReturnInput

// Begin Class UUINavPCComponent Function SetAllowSectionInput
struct Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics
{
	struct UINavPCComponent_eventSetAllowSectionInput_Parms
	{
		bool bAllowInput;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAllowInput_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bAllowInput_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAllowInput;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::NewProp_bAllowInput_SetBit(void* Obj)
{
	((UINavPCComponent_eventSetAllowSectionInput_Parms*)Obj)->bAllowInput = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::NewProp_bAllowInput = { "bAllowInput", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventSetAllowSectionInput_Parms), &Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::NewProp_bAllowInput_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAllowInput_MetaData), NewProp_bAllowInput_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::NewProp_bAllowInput,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SetAllowSectionInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::UINavPCComponent_eventSetAllowSectionInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::UINavPCComponent_eventSetAllowSectionInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSetAllowSectionInput)
{
	P_GET_UBOOL(Z_Param_bAllowInput);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetAllowSectionInput(Z_Param_bAllowInput);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SetAllowSectionInput

// Begin Class UUINavPCComponent Function SetAllowSelectInput
struct Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics
{
	struct UINavPCComponent_eventSetAllowSelectInput_Parms
	{
		bool bAllowInput;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAllowInput_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bAllowInput_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAllowInput;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::NewProp_bAllowInput_SetBit(void* Obj)
{
	((UINavPCComponent_eventSetAllowSelectInput_Parms*)Obj)->bAllowInput = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::NewProp_bAllowInput = { "bAllowInput", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventSetAllowSelectInput_Parms), &Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::NewProp_bAllowInput_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAllowInput_MetaData), NewProp_bAllowInput_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::NewProp_bAllowInput,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SetAllowSelectInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::UINavPCComponent_eventSetAllowSelectInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::UINavPCComponent_eventSetAllowSelectInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSetAllowSelectInput)
{
	P_GET_UBOOL(Z_Param_bAllowInput);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetAllowSelectInput(Z_Param_bAllowInput);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SetAllowSelectInput

// Begin Class UUINavPCComponent Function SetGamepadInputDataTables
struct Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics
{
	struct UINavPCComponent_eventSetGamepadInputDataTables_Parms
	{
		UDataTable* NewKeyIconTable;
		UDataTable* NewKeyNameTable;
		bool bUpdateInputDisplays;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "CPP_Default_bUpdateInputDisplays", "true" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUpdateInputDisplays_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewKeyIconTable;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewKeyNameTable;
	static void NewProp_bUpdateInputDisplays_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUpdateInputDisplays;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::NewProp_NewKeyIconTable = { "NewKeyIconTable", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventSetGamepadInputDataTables_Parms, NewKeyIconTable), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::NewProp_NewKeyNameTable = { "NewKeyNameTable", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventSetGamepadInputDataTables_Parms, NewKeyNameTable), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::NewProp_bUpdateInputDisplays_SetBit(void* Obj)
{
	((UINavPCComponent_eventSetGamepadInputDataTables_Parms*)Obj)->bUpdateInputDisplays = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::NewProp_bUpdateInputDisplays = { "bUpdateInputDisplays", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventSetGamepadInputDataTables_Parms), &Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::NewProp_bUpdateInputDisplays_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUpdateInputDisplays_MetaData), NewProp_bUpdateInputDisplays_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::NewProp_NewKeyIconTable,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::NewProp_NewKeyNameTable,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::NewProp_bUpdateInputDisplays,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SetGamepadInputDataTables", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::UINavPCComponent_eventSetGamepadInputDataTables_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::UINavPCComponent_eventSetGamepadInputDataTables_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSetGamepadInputDataTables)
{
	P_GET_OBJECT(UDataTable,Z_Param_NewKeyIconTable);
	P_GET_OBJECT(UDataTable,Z_Param_NewKeyNameTable);
	P_GET_UBOOL(Z_Param_bUpdateInputDisplays);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetGamepadInputDataTables(Z_Param_NewKeyIconTable,Z_Param_NewKeyNameTable,Z_Param_bUpdateInputDisplays);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SetGamepadInputDataTables

// Begin Class UUINavPCComponent Function SetKeyboardInputDataTables
struct Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics
{
	struct UINavPCComponent_eventSetKeyboardInputDataTables_Parms
	{
		UDataTable* NewKeyIconTable;
		UDataTable* NewKeyNameTable;
		bool bUpdateInputDisplays;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "CPP_Default_bUpdateInputDisplays", "true" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUpdateInputDisplays_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewKeyIconTable;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewKeyNameTable;
	static void NewProp_bUpdateInputDisplays_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUpdateInputDisplays;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::NewProp_NewKeyIconTable = { "NewKeyIconTable", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventSetKeyboardInputDataTables_Parms, NewKeyIconTable), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::NewProp_NewKeyNameTable = { "NewKeyNameTable", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCComponent_eventSetKeyboardInputDataTables_Parms, NewKeyNameTable), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::NewProp_bUpdateInputDisplays_SetBit(void* Obj)
{
	((UINavPCComponent_eventSetKeyboardInputDataTables_Parms*)Obj)->bUpdateInputDisplays = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::NewProp_bUpdateInputDisplays = { "bUpdateInputDisplays", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventSetKeyboardInputDataTables_Parms), &Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::NewProp_bUpdateInputDisplays_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUpdateInputDisplays_MetaData), NewProp_bUpdateInputDisplays_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::NewProp_NewKeyIconTable,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::NewProp_NewKeyNameTable,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::NewProp_bUpdateInputDisplays,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SetKeyboardInputDataTables", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::UINavPCComponent_eventSetKeyboardInputDataTables_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::UINavPCComponent_eventSetKeyboardInputDataTables_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSetKeyboardInputDataTables)
{
	P_GET_OBJECT(UDataTable,Z_Param_NewKeyIconTable);
	P_GET_OBJECT(UDataTable,Z_Param_NewKeyNameTable);
	P_GET_UBOOL(Z_Param_bUpdateInputDisplays);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetKeyboardInputDataTables(Z_Param_NewKeyIconTable,Z_Param_NewKeyNameTable,Z_Param_bUpdateInputDisplays);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SetKeyboardInputDataTables

// Begin Class UUINavPCComponent Function SetShowMouseCursor
struct Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics
{
	struct UINavPCComponent_eventSetShowMouseCursor_Parms
	{
		bool bShowMouse;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bShowMouse_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bShowMouse_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bShowMouse;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::NewProp_bShowMouse_SetBit(void* Obj)
{
	((UINavPCComponent_eventSetShowMouseCursor_Parms*)Obj)->bShowMouse = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::NewProp_bShowMouse = { "bShowMouse", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCComponent_eventSetShowMouseCursor_Parms), &Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::NewProp_bShowMouse_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bShowMouse_MetaData), NewProp_bShowMouse_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::NewProp_bShowMouse,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SetShowMouseCursor", nullptr, nullptr, Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::UINavPCComponent_eventSetShowMouseCursor_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::UINavPCComponent_eventSetShowMouseCursor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSetShowMouseCursor)
{
	P_GET_UBOOL(Z_Param_bShowMouse);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetShowMouseCursor(Z_Param_bShowMouse);
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SetShowMouseCursor

// Begin Class UUINavPCComponent Function SimulateMouseClick
struct Z_Construct_UFunction_UUINavPCComponent_SimulateMouseClick_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SimulateMouseClick_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SimulateMouseClick", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SimulateMouseClick_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SimulateMouseClick_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavPCComponent_SimulateMouseClick()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SimulateMouseClick_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSimulateMouseClick)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SimulateMouseClick();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SimulateMouseClick

// Begin Class UUINavPCComponent Function SimulateMousePress
struct Z_Construct_UFunction_UUINavPCComponent_SimulateMousePress_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SimulateMousePress_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SimulateMousePress", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SimulateMousePress_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SimulateMousePress_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavPCComponent_SimulateMousePress()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SimulateMousePress_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSimulateMousePress)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SimulateMousePress();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SimulateMousePress

// Begin Class UUINavPCComponent Function SimulateMouseRelease
struct Z_Construct_UFunction_UUINavPCComponent_SimulateMouseRelease_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCComponent_SimulateMouseRelease_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCComponent, nullptr, "SimulateMouseRelease", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCComponent_SimulateMouseRelease_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCComponent_SimulateMouseRelease_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavPCComponent_SimulateMouseRelease()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCComponent_SimulateMouseRelease_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavPCComponent::execSimulateMouseRelease)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SimulateMouseRelease();
	P_NATIVE_END;
}
// End Class UUINavPCComponent Function SimulateMouseRelease

// Begin Class UUINavPCComponent
void UUINavPCComponent::StaticRegisterNativesUUINavPCComponent()
{
	UClass* Class = UUINavPCComponent::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "AddInputContext", &UUINavPCComponent::execAddInputContext },
		{ "AllowsAllMenuInput", &UUINavPCComponent::execAllowsAllMenuInput },
		{ "AllowsDirectionalInput", &UUINavPCComponent::execAllowsDirectionalInput },
		{ "AllowsReturnInput", &UUINavPCComponent::execAllowsReturnInput },
		{ "AllowsSectionInput", &UUINavPCComponent::execAllowsSectionInput },
		{ "AllowsSelectInput", &UUINavPCComponent::execAllowsSelectInput },
		{ "ForceUpdateAllInputDisplays", &UUINavPCComponent::execForceUpdateAllInputDisplays },
		{ "GetActiveSubWidget", &UUINavPCComponent::execGetActiveSubWidget },
		{ "GetActiveWidget", &UUINavPCComponent::execGetActiveWidget },
		{ "GetAxis1DFromAxis2D", &UUINavPCComponent::execGetAxis1DFromAxis2D },
		{ "GetAxis2DFromAxis1D", &UUINavPCComponent::execGetAxis2DFromAxis1D },
		{ "GetAxisFromKey", &UUINavPCComponent::execGetAxisFromKey },
		{ "GetAxisFromScaledKey", &UUINavPCComponent::execGetAxisFromScaledKey },
		{ "GetCurrentInputType", &UUINavPCComponent::execGetCurrentInputType },
		{ "GetCurrentPlatformData", &UUINavPCComponent::execGetCurrentPlatformData },
		{ "GetEnhancedInputComponent", &UUINavPCComponent::execGetEnhancedInputComponent },
		{ "GetEnhancedInputIcon", &UUINavPCComponent::execGetEnhancedInputIcon },
		{ "GetEnhancedInputKey", &UUINavPCComponent::execGetEnhancedInputKey },
		{ "GetEnhancedInputKeys", &UUINavPCComponent::execGetEnhancedInputKeys },
		{ "GetEnhancedInputText", &UUINavPCComponent::execGetEnhancedInputText },
		{ "GetInputMode", &UUINavPCComponent::execGetInputMode },
		{ "GetKeyFromAxis", &UUINavPCComponent::execGetKeyFromAxis },
		{ "GetKeyIcon", &UUINavPCComponent::execGetKeyIcon },
		{ "GetKeyText", &UUINavPCComponent::execGetKeyText },
		{ "GetOppositeAxis2DAxis", &UUINavPCComponent::execGetOppositeAxis2DAxis },
		{ "GetOppositeAxisKey", &UUINavPCComponent::execGetOppositeAxisKey },
		{ "GetPC", &UUINavPCComponent::execGetPC },
		{ "GetSoftEnhancedInputIcon", &UUINavPCComponent::execGetSoftEnhancedInputIcon },
		{ "GetSoftKeyIcon", &UUINavPCComponent::execGetSoftKeyIcon },
		{ "GetThumbstickDelta", &UUINavPCComponent::execGetThumbstickDelta },
		{ "GetUINavInputContext", &UUINavPCComponent::execGetUINavInputContext },
		{ "GoToBuiltWidget", &UUINavPCComponent::execGoToBuiltWidget },
		{ "GoToPromptWidget", &UUINavPCComponent::execGoToPromptWidget },
		{ "GoToWidget", &UUINavPCComponent::execGoToWidget },
		{ "HidingMouseCursor", &UUINavPCComponent::execHidingMouseCursor },
		{ "IsAxis", &UUINavPCComponent::execIsAxis },
		{ "IsAxis2D", &UUINavPCComponent::execIsAxis2D },
		{ "IsMovingThumbstick", &UUINavPCComponent::execIsMovingThumbstick },
		{ "IsUsingGamepad", &UUINavPCComponent::execIsUsingGamepad },
		{ "IsUsingKeyboard", &UUINavPCComponent::execIsUsingKeyboard },
		{ "IsUsingMouse", &UUINavPCComponent::execIsUsingMouse },
		{ "IsWidgetActive", &UUINavPCComponent::execIsWidgetActive },
		{ "IsWidgetChild", &UUINavPCComponent::execIsWidgetChild },
		{ "NavigateInDirection", &UUINavPCComponent::execNavigateInDirection },
		{ "NotifyNavigatedTo", &UUINavPCComponent::execNotifyNavigatedTo },
		{ "RefreshNavigationKeys", &UUINavPCComponent::execRefreshNavigationKeys },
		{ "RemoveInputContext", &UUINavPCComponent::execRemoveInputContext },
		{ "SetActiveWidget", &UUINavPCComponent::execSetActiveWidget },
		{ "SetAllowAllMenuInput", &UUINavPCComponent::execSetAllowAllMenuInput },
		{ "SetAllowDirectionalInput", &UUINavPCComponent::execSetAllowDirectionalInput },
		{ "SetAllowReturnInput", &UUINavPCComponent::execSetAllowReturnInput },
		{ "SetAllowSectionInput", &UUINavPCComponent::execSetAllowSectionInput },
		{ "SetAllowSelectInput", &UUINavPCComponent::execSetAllowSelectInput },
		{ "SetGamepadInputDataTables", &UUINavPCComponent::execSetGamepadInputDataTables },
		{ "SetKeyboardInputDataTables", &UUINavPCComponent::execSetKeyboardInputDataTables },
		{ "SetShowMouseCursor", &UUINavPCComponent::execSetShowMouseCursor },
		{ "SimulateMouseClick", &UUINavPCComponent::execSimulateMouseClick },
		{ "SimulateMousePress", &UUINavPCComponent::execSimulateMousePress },
		{ "SimulateMouseRelease", &UUINavPCComponent::execSimulateMouseRelease },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavPCComponent);
UClass* Z_Construct_UClass_UUINavPCComponent_NoRegister()
{
	return UUINavPCComponent::StaticClass();
}
struct Z_Construct_UClass_UUINavPCComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "UINavPCComponent.h" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActiveWidget_MetaData[] = {
		{ "Category", "UINavController" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActiveSubWidget_MetaData[] = {
		{ "Category", "UINavController" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PC_MetaData[] = {
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ListeningInputBox_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CachedInputContexts_MetaData[] = {
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
		{ "NativeConstTemplateArg", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AppliedInputContexts_MetaData[] = {
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentInputType_MetaData[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseAnalogDirectionalInput_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Indicates whether the left analog stick should be used for directional navigation\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Indicates whether the left analog stick should be used for directional navigation" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bChainNavigation_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Indicates whether navigation will occur periodically after the player\n\x09holds down a navigation key\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Indicates whether navigation will occur periodically after the player\nholds down a navigation key" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputHeldWaitTime_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09The amount of time the player needs to hold a key for the navigation to\n\x09start occuring periodically\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The amount of time the player needs to hold a key for the navigation to\nstart occuring periodically" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NavigationChainFrequency_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09The amount of time that passes before the navigation chains further\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The amount of time that passes before the navigation chains further" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UseThumbstickAsMouse_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Indicates whether the controller should use the left or right stick as mouse.\n\x09If the active UINavWidget has this set to a value different than None, it will override this one.\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Indicates whether the controller should use the left or right stick as mouse.\nIf the active UINavWidget has this set to a value different than None, it will override this one." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ThumbstickCursorSensitivity_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09The sensitivity of the cursor when moved with the left stick\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The sensitivity of the cursor when moved with the left stick" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ThumbstickCursorDeadzone_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09The deadzone of the thumbstick when using it as a mouse cursor.\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The deadzone of the thumbstick when using it as a mouse cursor." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ThumbstickCursorCurve_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09""A float curve that dictates how much the analog stick moves the mouse in relation to the sensitivity\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "A float curve that dictates how much the analog stick moves the mouse in relation to the sensitivity" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bScrollWithRightThumbstick_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Indicates whether you can scroll through scroll boxes using the gamepad's right thumbstick\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Indicates whether you can scroll through scroll boxes using the gamepad's right thumbstick" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RightThumbstickScrollDeadzone_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Indicates the deadzone to use when scrolling with the right thumbstick\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Indicates the deadzone to use when scrolling with the right thumbstick" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AutoHideMouse_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Indicates whether the controller should automatically hide the mouse cursor when using certain input types.\n\x09In order for this to work, you have to set your GameViewportClient to UINavGameViewportClient in the Project Settings,\n\x09or your custom ViewportClient class must inherit from UINavGameViewportClient and not override the GetCursor function.\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Indicates whether the controller should automatically hide the mouse cursor when using certain input types.\nIn order for this to work, you have to set your GameViewportClient to UINavGameViewportClient in the Project Settings,\nor your custom ViewportClient class must inherit from UINavGameViewportClient and not override the GetCursor function." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RightThumbstickScrollSensitivity_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09The sensitivity of scrolling when using the right thumbstick\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The sensitivity of scrolling when using the right thumbstick" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RebindThreshold_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09The required value for an axis to be considered for rebinding\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The required value for an axis to be considered for rebinding" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GamepadKeyIconData_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Holds the key icons for gamepad\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
		{ "RequiredAssetDataTags", "RowStructure=/Script/UINavigation.InputIconMapping" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Holds the key icons for gamepad" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_KeyboardMouseKeyIconData_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Holds the key icons for mouse and keyboard\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
		{ "RequiredAssetDataTags", "RowStructure=/Script/UINavigation.InputIconMapping" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Holds the key icons for mouse and keyboard" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GamepadKeyNameData_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Holds the key names for gamepad\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
		{ "RequiredAssetDataTags", "RowStructure=/Script/UINavigation.InputNameMapping" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Holds the key names for gamepad" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_KeyboardMouseKeyNameData_MetaData[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n\x09Holds the key names for mouse and keyboard\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
		{ "RequiredAssetDataTags", "RowStructure=/Script/UINavigation.InputNameMapping" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Holds the key names for mouse and keyboard" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UINavInputContextRef_MetaData[] = {
		{ "Category", "Settings" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputTypeChangedDelegate_MetaData[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UpdateInputIconsDelegate_MetaData[] = {
		{ "Category", "UINavController" },
		{ "ModuleRelativePath", "Public/UINavPCComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ActiveWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ActiveSubWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PC;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ListeningInputBox;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CachedInputContexts_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_CachedInputContexts;
	static const UECodeGen_Private::FBytePropertyParams NewProp_AppliedInputContexts_ValueProp;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_AppliedInputContexts_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_AppliedInputContexts;
	static const UECodeGen_Private::FBytePropertyParams NewProp_CurrentInputType_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_CurrentInputType;
	static void NewProp_bUseAnalogDirectionalInput_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseAnalogDirectionalInput;
	static void NewProp_bChainNavigation_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bChainNavigation;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InputHeldWaitTime;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_NavigationChainFrequency;
	static const UECodeGen_Private::FBytePropertyParams NewProp_UseThumbstickAsMouse_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_UseThumbstickAsMouse;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ThumbstickCursorSensitivity;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ThumbstickCursorDeadzone;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ThumbstickCursorCurve;
	static void NewProp_bScrollWithRightThumbstick_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bScrollWithRightThumbstick;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RightThumbstickScrollDeadzone;
	static const UECodeGen_Private::FBytePropertyParams NewProp_AutoHideMouse_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_AutoHideMouse;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RightThumbstickScrollSensitivity;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RebindThreshold;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_GamepadKeyIconData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_KeyboardMouseKeyIconData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_GamepadKeyNameData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_KeyboardMouseKeyNameData;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UINavInputContextRef;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_InputTypeChangedDelegate;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_UpdateInputIconsDelegate;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUINavPCComponent_AddInputContext, "AddInputContext" }, // 814425613
		{ &Z_Construct_UFunction_UUINavPCComponent_AllowsAllMenuInput, "AllowsAllMenuInput" }, // 2512117195
		{ &Z_Construct_UFunction_UUINavPCComponent_AllowsDirectionalInput, "AllowsDirectionalInput" }, // 1518889813
		{ &Z_Construct_UFunction_UUINavPCComponent_AllowsReturnInput, "AllowsReturnInput" }, // 438790373
		{ &Z_Construct_UFunction_UUINavPCComponent_AllowsSectionInput, "AllowsSectionInput" }, // 149060722
		{ &Z_Construct_UFunction_UUINavPCComponent_AllowsSelectInput, "AllowsSelectInput" }, // 1078652290
		{ &Z_Construct_UFunction_UUINavPCComponent_ForceUpdateAllInputDisplays, "ForceUpdateAllInputDisplays" }, // 3264134864
		{ &Z_Construct_UFunction_UUINavPCComponent_GetActiveSubWidget, "GetActiveSubWidget" }, // 3133100507
		{ &Z_Construct_UFunction_UUINavPCComponent_GetActiveWidget, "GetActiveWidget" }, // 4059334559
		{ &Z_Construct_UFunction_UUINavPCComponent_GetAxis1DFromAxis2D, "GetAxis1DFromAxis2D" }, // 798633243
		{ &Z_Construct_UFunction_UUINavPCComponent_GetAxis2DFromAxis1D, "GetAxis2DFromAxis1D" }, // 2398991942
		{ &Z_Construct_UFunction_UUINavPCComponent_GetAxisFromKey, "GetAxisFromKey" }, // 3811791682
		{ &Z_Construct_UFunction_UUINavPCComponent_GetAxisFromScaledKey, "GetAxisFromScaledKey" }, // 1542173448
		{ &Z_Construct_UFunction_UUINavPCComponent_GetCurrentInputType, "GetCurrentInputType" }, // 2311188617
		{ &Z_Construct_UFunction_UUINavPCComponent_GetCurrentPlatformData, "GetCurrentPlatformData" }, // 2653715259
		{ &Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputComponent, "GetEnhancedInputComponent" }, // 246037420
		{ &Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputIcon, "GetEnhancedInputIcon" }, // 529441089
		{ &Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKey, "GetEnhancedInputKey" }, // 1011038222
		{ &Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputKeys, "GetEnhancedInputKeys" }, // 1561736324
		{ &Z_Construct_UFunction_UUINavPCComponent_GetEnhancedInputText, "GetEnhancedInputText" }, // 3671125884
		{ &Z_Construct_UFunction_UUINavPCComponent_GetInputMode, "GetInputMode" }, // 919337763
		{ &Z_Construct_UFunction_UUINavPCComponent_GetKeyFromAxis, "GetKeyFromAxis" }, // 3099488551
		{ &Z_Construct_UFunction_UUINavPCComponent_GetKeyIcon, "GetKeyIcon" }, // 1456599764
		{ &Z_Construct_UFunction_UUINavPCComponent_GetKeyText, "GetKeyText" }, // 2799895130
		{ &Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxis2DAxis, "GetOppositeAxis2DAxis" }, // 1455968879
		{ &Z_Construct_UFunction_UUINavPCComponent_GetOppositeAxisKey, "GetOppositeAxisKey" }, // 3756732238
		{ &Z_Construct_UFunction_UUINavPCComponent_GetPC, "GetPC" }, // 2143375341
		{ &Z_Construct_UFunction_UUINavPCComponent_GetSoftEnhancedInputIcon, "GetSoftEnhancedInputIcon" }, // 738679648
		{ &Z_Construct_UFunction_UUINavPCComponent_GetSoftKeyIcon, "GetSoftKeyIcon" }, // 1309800344
		{ &Z_Construct_UFunction_UUINavPCComponent_GetThumbstickDelta, "GetThumbstickDelta" }, // 1197423530
		{ &Z_Construct_UFunction_UUINavPCComponent_GetUINavInputContext, "GetUINavInputContext" }, // 3899028857
		{ &Z_Construct_UFunction_UUINavPCComponent_GoToBuiltWidget, "GoToBuiltWidget" }, // 1782565052
		{ &Z_Construct_UFunction_UUINavPCComponent_GoToPromptWidget, "GoToPromptWidget" }, // 889334145
		{ &Z_Construct_UFunction_UUINavPCComponent_GoToWidget, "GoToWidget" }, // 496488879
		{ &Z_Construct_UFunction_UUINavPCComponent_HidingMouseCursor, "HidingMouseCursor" }, // 45003424
		{ &Z_Construct_UFunction_UUINavPCComponent_IsAxis, "IsAxis" }, // 1864855725
		{ &Z_Construct_UFunction_UUINavPCComponent_IsAxis2D, "IsAxis2D" }, // 3951306558
		{ &Z_Construct_UFunction_UUINavPCComponent_IsMovingThumbstick, "IsMovingThumbstick" }, // 1860203353
		{ &Z_Construct_UFunction_UUINavPCComponent_IsUsingGamepad, "IsUsingGamepad" }, // 2503667478
		{ &Z_Construct_UFunction_UUINavPCComponent_IsUsingKeyboard, "IsUsingKeyboard" }, // 3991210507
		{ &Z_Construct_UFunction_UUINavPCComponent_IsUsingMouse, "IsUsingMouse" }, // 3062438925
		{ &Z_Construct_UFunction_UUINavPCComponent_IsWidgetActive, "IsWidgetActive" }, // 669421950
		{ &Z_Construct_UFunction_UUINavPCComponent_IsWidgetChild, "IsWidgetChild" }, // 1314098240
		{ &Z_Construct_UFunction_UUINavPCComponent_NavigateInDirection, "NavigateInDirection" }, // 15334111
		{ &Z_Construct_UFunction_UUINavPCComponent_NotifyNavigatedTo, "NotifyNavigatedTo" }, // 4222806760
		{ &Z_Construct_UFunction_UUINavPCComponent_RefreshNavigationKeys, "RefreshNavigationKeys" }, // 1333533022
		{ &Z_Construct_UFunction_UUINavPCComponent_RemoveInputContext, "RemoveInputContext" }, // 2815828924
		{ &Z_Construct_UFunction_UUINavPCComponent_SetActiveWidget, "SetActiveWidget" }, // 2588933924
		{ &Z_Construct_UFunction_UUINavPCComponent_SetAllowAllMenuInput, "SetAllowAllMenuInput" }, // 3071595618
		{ &Z_Construct_UFunction_UUINavPCComponent_SetAllowDirectionalInput, "SetAllowDirectionalInput" }, // 1934918396
		{ &Z_Construct_UFunction_UUINavPCComponent_SetAllowReturnInput, "SetAllowReturnInput" }, // 2737249850
		{ &Z_Construct_UFunction_UUINavPCComponent_SetAllowSectionInput, "SetAllowSectionInput" }, // 3510164975
		{ &Z_Construct_UFunction_UUINavPCComponent_SetAllowSelectInput, "SetAllowSelectInput" }, // 1833620471
		{ &Z_Construct_UFunction_UUINavPCComponent_SetGamepadInputDataTables, "SetGamepadInputDataTables" }, // 4080958058
		{ &Z_Construct_UFunction_UUINavPCComponent_SetKeyboardInputDataTables, "SetKeyboardInputDataTables" }, // 3506411896
		{ &Z_Construct_UFunction_UUINavPCComponent_SetShowMouseCursor, "SetShowMouseCursor" }, // 2671627582
		{ &Z_Construct_UFunction_UUINavPCComponent_SimulateMouseClick, "SimulateMouseClick" }, // 1808182228
		{ &Z_Construct_UFunction_UUINavPCComponent_SimulateMousePress, "SimulateMousePress" }, // 3394001266
		{ &Z_Construct_UFunction_UUINavPCComponent_SimulateMouseRelease, "SimulateMouseRelease" }, // 43907906
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavPCComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ActiveWidget = { "ActiveWidget", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, ActiveWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActiveWidget_MetaData), NewProp_ActiveWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ActiveSubWidget = { "ActiveSubWidget", nullptr, (EPropertyFlags)0x002008000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, ActiveSubWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActiveSubWidget_MetaData), NewProp_ActiveSubWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_PC = { "PC", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, PC), Z_Construct_UClass_APlayerController_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PC_MetaData), NewProp_PC_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ListeningInputBox = { "ListeningInputBox", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, ListeningInputBox), Z_Construct_UClass_UUINavInputBox_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ListeningInputBox_MetaData), NewProp_ListeningInputBox_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_CachedInputContexts_Inner = { "CachedInputContexts", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_CachedInputContexts = { "CachedInputContexts", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, CachedInputContexts), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CachedInputContexts_MetaData), NewProp_CachedInputContexts_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_AppliedInputContexts_ValueProp = { "AppliedInputContexts", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_AppliedInputContexts_Key_KeyProp = { "AppliedInputContexts_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_AppliedInputContexts = { "AppliedInputContexts", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, AppliedInputContexts), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AppliedInputContexts_MetaData), NewProp_AppliedInputContexts_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_CurrentInputType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_CurrentInputType = { "CurrentInputType", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, CurrentInputType), Z_Construct_UEnum_UINavigation_EInputType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentInputType_MetaData), NewProp_CurrentInputType_MetaData) }; // 346009216
void Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bUseAnalogDirectionalInput_SetBit(void* Obj)
{
	((UUINavPCComponent*)Obj)->bUseAnalogDirectionalInput = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bUseAnalogDirectionalInput = { "bUseAnalogDirectionalInput", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavPCComponent), &Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bUseAnalogDirectionalInput_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseAnalogDirectionalInput_MetaData), NewProp_bUseAnalogDirectionalInput_MetaData) };
void Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bChainNavigation_SetBit(void* Obj)
{
	((UUINavPCComponent*)Obj)->bChainNavigation = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bChainNavigation = { "bChainNavigation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavPCComponent), &Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bChainNavigation_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bChainNavigation_MetaData), NewProp_bChainNavigation_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_InputHeldWaitTime = { "InputHeldWaitTime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, InputHeldWaitTime), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputHeldWaitTime_MetaData), NewProp_InputHeldWaitTime_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_NavigationChainFrequency = { "NavigationChainFrequency", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, NavigationChainFrequency), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NavigationChainFrequency_MetaData), NewProp_NavigationChainFrequency_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_UseThumbstickAsMouse_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_UseThumbstickAsMouse = { "UseThumbstickAsMouse", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, UseThumbstickAsMouse), Z_Construct_UEnum_UINavigation_EThumbstickAsMouse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UseThumbstickAsMouse_MetaData), NewProp_UseThumbstickAsMouse_MetaData) }; // 819177189
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ThumbstickCursorSensitivity = { "ThumbstickCursorSensitivity", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, ThumbstickCursorSensitivity), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ThumbstickCursorSensitivity_MetaData), NewProp_ThumbstickCursorSensitivity_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ThumbstickCursorDeadzone = { "ThumbstickCursorDeadzone", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, ThumbstickCursorDeadzone), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ThumbstickCursorDeadzone_MetaData), NewProp_ThumbstickCursorDeadzone_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ThumbstickCursorCurve = { "ThumbstickCursorCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, ThumbstickCursorCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ThumbstickCursorCurve_MetaData), NewProp_ThumbstickCursorCurve_MetaData) };
void Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bScrollWithRightThumbstick_SetBit(void* Obj)
{
	((UUINavPCComponent*)Obj)->bScrollWithRightThumbstick = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bScrollWithRightThumbstick = { "bScrollWithRightThumbstick", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavPCComponent), &Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bScrollWithRightThumbstick_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bScrollWithRightThumbstick_MetaData), NewProp_bScrollWithRightThumbstick_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_RightThumbstickScrollDeadzone = { "RightThumbstickScrollDeadzone", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, RightThumbstickScrollDeadzone), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RightThumbstickScrollDeadzone_MetaData), NewProp_RightThumbstickScrollDeadzone_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_AutoHideMouse_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_AutoHideMouse = { "AutoHideMouse", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, AutoHideMouse), Z_Construct_UEnum_UINavigation_EAutoHideMouse, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AutoHideMouse_MetaData), NewProp_AutoHideMouse_MetaData) }; // 1955732934
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_RightThumbstickScrollSensitivity = { "RightThumbstickScrollSensitivity", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, RightThumbstickScrollSensitivity), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RightThumbstickScrollSensitivity_MetaData), NewProp_RightThumbstickScrollSensitivity_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_RebindThreshold = { "RebindThreshold", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, RebindThreshold), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RebindThreshold_MetaData), NewProp_RebindThreshold_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_GamepadKeyIconData = { "GamepadKeyIconData", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, GamepadKeyIconData), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GamepadKeyIconData_MetaData), NewProp_GamepadKeyIconData_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_KeyboardMouseKeyIconData = { "KeyboardMouseKeyIconData", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, KeyboardMouseKeyIconData), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_KeyboardMouseKeyIconData_MetaData), NewProp_KeyboardMouseKeyIconData_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_GamepadKeyNameData = { "GamepadKeyNameData", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, GamepadKeyNameData), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GamepadKeyNameData_MetaData), NewProp_GamepadKeyNameData_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_KeyboardMouseKeyNameData = { "KeyboardMouseKeyNameData", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, KeyboardMouseKeyNameData), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_KeyboardMouseKeyNameData_MetaData), NewProp_KeyboardMouseKeyNameData_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_UINavInputContextRef = { "UINavInputContextRef", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, UINavInputContextRef), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UINavInputContextRef_MetaData), NewProp_UINavInputContextRef_MetaData) };
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_InputTypeChangedDelegate = { "InputTypeChangedDelegate", nullptr, (EPropertyFlags)0x0010100010080014, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, InputTypeChangedDelegate), Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputTypeChangedDelegate_MetaData), NewProp_InputTypeChangedDelegate_MetaData) }; // 2806563397
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_UpdateInputIconsDelegate = { "UpdateInputIconsDelegate", nullptr, (EPropertyFlags)0x0010100010080014, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavPCComponent, UpdateInputIconsDelegate), Z_Construct_UDelegateFunction_UINavigation_UpdateInputIconsDelegate__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UpdateInputIconsDelegate_MetaData), NewProp_UpdateInputIconsDelegate_MetaData) }; // 3170002031
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavPCComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ActiveWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ActiveSubWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_PC,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ListeningInputBox,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_CachedInputContexts_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_CachedInputContexts,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_AppliedInputContexts_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_AppliedInputContexts_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_AppliedInputContexts,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_CurrentInputType_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_CurrentInputType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bUseAnalogDirectionalInput,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bChainNavigation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_InputHeldWaitTime,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_NavigationChainFrequency,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_UseThumbstickAsMouse_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_UseThumbstickAsMouse,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ThumbstickCursorSensitivity,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ThumbstickCursorDeadzone,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_ThumbstickCursorCurve,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_bScrollWithRightThumbstick,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_RightThumbstickScrollDeadzone,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_AutoHideMouse_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_AutoHideMouse,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_RightThumbstickScrollSensitivity,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_RebindThreshold,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_GamepadKeyIconData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_KeyboardMouseKeyIconData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_GamepadKeyNameData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_KeyboardMouseKeyNameData,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_UINavInputContextRef,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_InputTypeChangedDelegate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavPCComponent_Statics::NewProp_UpdateInputIconsDelegate,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavPCComponent_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavPCComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavPCComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavPCComponent_Statics::ClassParams = {
	&UUINavPCComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UUINavPCComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavPCComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavPCComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavPCComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavPCComponent()
{
	if (!Z_Registration_Info_UClass_UUINavPCComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavPCComponent.OuterSingleton, Z_Construct_UClass_UUINavPCComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavPCComponent.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavPCComponent>()
{
	return UUINavPCComponent::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavPCComponent);
UUINavPCComponent::~UUINavPCComponent() {}
// End Class UUINavPCComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FAxis2D_Keys::StaticStruct, Z_Construct_UScriptStruct_FAxis2D_Keys_Statics::NewStructOps, TEXT("Axis2D_Keys"), &Z_Registration_Info_UScriptStruct_Axis2D_Keys, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FAxis2D_Keys), 2587198908U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavPCComponent, UUINavPCComponent::StaticClass, TEXT("UUINavPCComponent"), &Z_Registration_Info_UClass_UUINavPCComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavPCComponent), 2643172768U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_532668112(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
