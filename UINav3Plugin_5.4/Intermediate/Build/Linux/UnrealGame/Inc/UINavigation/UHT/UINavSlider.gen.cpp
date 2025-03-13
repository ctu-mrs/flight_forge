// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavSlider.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavSlider() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FLinearColor();
SLATECORE_API UEnum* Z_Construct_UEnum_SlateCore_ETextCommit();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavHorizontalComponent();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavSlider();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavSlider_NoRegister();
UMG_API UClass* Z_Construct_UClass_USlider_NoRegister();
UMG_API UClass* Z_Construct_UClass_USpinBox_NoRegister();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Class UUINavSlider Function GetCurrentValue
struct Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics
{
	struct UINavSlider_eventGetCurrentValue_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavSlider" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Get Current Value inserted in the specified number range (not 0 to 1)\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Get Current Value inserted in the specified number range (not 0 to 1)" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavSlider_eventGetCurrentValue_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavSlider, nullptr, "GetCurrentValue", nullptr, nullptr, Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::UINavSlider_eventGetCurrentValue_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::UINavSlider_eventGetCurrentValue_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavSlider_GetCurrentValue()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavSlider_GetCurrentValue_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavSlider::execGetCurrentValue)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=P_THIS->GetCurrentValue();
	P_NATIVE_END;
}
// End Class UUINavSlider Function GetCurrentValue

// Begin Class UUINavSlider Function GetSliderValue
struct Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics
{
	struct UINavSlider_eventGetSliderValue_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavSlider" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Get Current Slider value (0 to 1)\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Get Current Slider value (0 to 1)" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavSlider_eventGetSliderValue_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavSlider, nullptr, "GetSliderValue", nullptr, nullptr, Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::UINavSlider_eventGetSliderValue_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::UINavSlider_eventGetSliderValue_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavSlider_GetSliderValue()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavSlider_GetSliderValue_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavSlider::execGetSliderValue)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=P_THIS->GetSliderValue();
	P_NATIVE_END;
}
// End Class UUINavSlider Function GetSliderValue

// Begin Class UUINavSlider Function HandleOnSliderMouseCaptureBegin
struct Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureBegin_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureBegin_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavSlider, nullptr, "HandleOnSliderMouseCaptureBegin", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureBegin_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureBegin_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureBegin()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureBegin_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavSlider::execHandleOnSliderMouseCaptureBegin)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HandleOnSliderMouseCaptureBegin();
	P_NATIVE_END;
}
// End Class UUINavSlider Function HandleOnSliderMouseCaptureBegin

// Begin Class UUINavSlider Function HandleOnSliderMouseCaptureEnd
struct Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureEnd_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureEnd_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavSlider, nullptr, "HandleOnSliderMouseCaptureEnd", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureEnd_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureEnd_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureEnd()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureEnd_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavSlider::execHandleOnSliderMouseCaptureEnd)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HandleOnSliderMouseCaptureEnd();
	P_NATIVE_END;
}
// End Class UUINavSlider Function HandleOnSliderMouseCaptureEnd

// Begin Class UUINavSlider Function HandleOnSliderValueChanged
struct Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics
{
	struct UINavSlider_eventHandleOnSliderValueChanged_Parms
	{
		float InValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavSlider_eventHandleOnSliderValueChanged_Parms, InValue), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InValue_MetaData), NewProp_InValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::NewProp_InValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavSlider, nullptr, "HandleOnSliderValueChanged", nullptr, nullptr, Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::UINavSlider_eventHandleOnSliderValueChanged_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::UINavSlider_eventHandleOnSliderValueChanged_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavSlider::execHandleOnSliderValueChanged)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HandleOnSliderValueChanged(Z_Param_InValue);
	P_NATIVE_END;
}
// End Class UUINavSlider Function HandleOnSliderValueChanged

// Begin Class UUINavSlider Function HandleOnSpinBoxMouseCaptureBegin
struct Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxMouseCaptureBegin_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxMouseCaptureBegin_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavSlider, nullptr, "HandleOnSpinBoxMouseCaptureBegin", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxMouseCaptureBegin_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxMouseCaptureBegin_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxMouseCaptureBegin()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxMouseCaptureBegin_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavSlider::execHandleOnSpinBoxMouseCaptureBegin)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HandleOnSpinBoxMouseCaptureBegin();
	P_NATIVE_END;
}
// End Class UUINavSlider Function HandleOnSpinBoxMouseCaptureBegin

// Begin Class UUINavSlider Function HandleOnSpinBoxValueChanged
struct Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics
{
	struct UINavSlider_eventHandleOnSpinBoxValueChanged_Parms
	{
		float InValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavSlider_eventHandleOnSpinBoxValueChanged_Parms, InValue), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InValue_MetaData), NewProp_InValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::NewProp_InValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavSlider, nullptr, "HandleOnSpinBoxValueChanged", nullptr, nullptr, Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::UINavSlider_eventHandleOnSpinBoxValueChanged_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::UINavSlider_eventHandleOnSpinBoxValueChanged_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavSlider::execHandleOnSpinBoxValueChanged)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HandleOnSpinBoxValueChanged(Z_Param_InValue);
	P_NATIVE_END;
}
// End Class UUINavSlider Function HandleOnSpinBoxValueChanged

// Begin Class UUINavSlider Function HandleOnSpinBoxValueCommitted
struct Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics
{
	struct UINavSlider_eventHandleOnSpinBoxValueCommitted_Parms
	{
		float InValue;
		TEnumAsByte<ETextCommit::Type> CommitMethod;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InValue_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CommitMethod_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
	static const UECodeGen_Private::FBytePropertyParams NewProp_CommitMethod;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavSlider_eventHandleOnSpinBoxValueCommitted_Parms, InValue), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InValue_MetaData), NewProp_InValue_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::NewProp_CommitMethod = { "CommitMethod", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavSlider_eventHandleOnSpinBoxValueCommitted_Parms, CommitMethod), Z_Construct_UEnum_SlateCore_ETextCommit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CommitMethod_MetaData), NewProp_CommitMethod_MetaData) }; // 1817921380
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::NewProp_InValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::NewProp_CommitMethod,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavSlider, nullptr, "HandleOnSpinBoxValueCommitted", nullptr, nullptr, Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::UINavSlider_eventHandleOnSpinBoxValueCommitted_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::UINavSlider_eventHandleOnSpinBoxValueCommitted_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavSlider::execHandleOnSpinBoxValueCommitted)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
	P_GET_PROPERTY(FByteProperty,Z_Param_CommitMethod);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HandleOnSpinBoxValueCommitted(Z_Param_InValue,ETextCommit::Type(Z_Param_CommitMethod));
	P_NATIVE_END;
}
// End Class UUINavSlider Function HandleOnSpinBoxValueCommitted

// Begin Class UUINavSlider Function SetValueClamped
struct Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics
{
	struct UINavSlider_eventSetValueClamped_Parms
	{
		float Value;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavSlider" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Value_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Value;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavSlider_eventSetValueClamped_Parms, Value), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Value_MetaData), NewProp_Value_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::NewProp_Value,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavSlider, nullptr, "SetValueClamped", nullptr, nullptr, Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::UINavSlider_eventSetValueClamped_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::UINavSlider_eventSetValueClamped_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavSlider_SetValueClamped()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavSlider_SetValueClamped_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavSlider::execSetValueClamped)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_Value);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetValueClamped(Z_Param_Value);
	P_NATIVE_END;
}
// End Class UUINavSlider Function SetValueClamped

// Begin Class UUINavSlider
void UUINavSlider::StaticRegisterNativesUUINavSlider()
{
	UClass* Class = UUINavSlider::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetCurrentValue", &UUINavSlider::execGetCurrentValue },
		{ "GetSliderValue", &UUINavSlider::execGetSliderValue },
		{ "HandleOnSliderMouseCaptureBegin", &UUINavSlider::execHandleOnSliderMouseCaptureBegin },
		{ "HandleOnSliderMouseCaptureEnd", &UUINavSlider::execHandleOnSliderMouseCaptureEnd },
		{ "HandleOnSliderValueChanged", &UUINavSlider::execHandleOnSliderValueChanged },
		{ "HandleOnSpinBoxMouseCaptureBegin", &UUINavSlider::execHandleOnSpinBoxMouseCaptureBegin },
		{ "HandleOnSpinBoxValueChanged", &UUINavSlider::execHandleOnSpinBoxValueChanged },
		{ "HandleOnSpinBoxValueCommitted", &UUINavSlider::execHandleOnSpinBoxValueCommitted },
		{ "SetValueClamped", &UUINavSlider::execSetValueClamped },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavSlider);
UClass* Z_Construct_UClass_UUINavSlider_NoRegister()
{
	return UUINavSlider::StaticClass();
}
struct Z_Construct_UClass_UUINavSlider_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UINavSlider.h" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Slider_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavSlider" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NavSpinBox_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "UINavSlider" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
		{ "OptionalWidget", "TRUE" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MinValue_MetaData[] = {
		{ "Category", "UINavSlider" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxValue_MetaData[] = {
		{ "Category", "UINavSlider" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Interval_MetaData[] = {
		{ "Category", "UINavSlider" },
		{ "ClampMin", "0" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxDecimalDigits_MetaData[] = {
		{ "Category", "UINavSlider" },
		{ "ClampMin", "0" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MinDecimalDigits_MetaData[] = {
		{ "Category", "UINavSlider" },
		{ "ClampMin", "0" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bUseComma_MetaData[] = {
		{ "Category", "UINavSlider" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HandleHoverColor_MetaData[] = {
		{ "Category", "UINavSlider" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BarHoverColor_MetaData[] = {
		{ "Category", "UINavSlider" },
		{ "ModuleRelativePath", "Public/UINavSlider.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Slider;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NavSpinBox;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MinValue;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxValue;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Interval;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxDecimalDigits;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MinDecimalDigits;
	static void NewProp_bUseComma_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseComma;
	static const UECodeGen_Private::FStructPropertyParams NewProp_HandleHoverColor;
	static const UECodeGen_Private::FStructPropertyParams NewProp_BarHoverColor;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUINavSlider_GetCurrentValue, "GetCurrentValue" }, // 3689496152
		{ &Z_Construct_UFunction_UUINavSlider_GetSliderValue, "GetSliderValue" }, // 3113733251
		{ &Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureBegin, "HandleOnSliderMouseCaptureBegin" }, // 2596784360
		{ &Z_Construct_UFunction_UUINavSlider_HandleOnSliderMouseCaptureEnd, "HandleOnSliderMouseCaptureEnd" }, // 222368329
		{ &Z_Construct_UFunction_UUINavSlider_HandleOnSliderValueChanged, "HandleOnSliderValueChanged" }, // 2009269567
		{ &Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxMouseCaptureBegin, "HandleOnSpinBoxMouseCaptureBegin" }, // 2072869970
		{ &Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueChanged, "HandleOnSpinBoxValueChanged" }, // 4164565466
		{ &Z_Construct_UFunction_UUINavSlider_HandleOnSpinBoxValueCommitted, "HandleOnSpinBoxValueCommitted" }, // 489829844
		{ &Z_Construct_UFunction_UUINavSlider_SetValueClamped, "SetValueClamped" }, // 2337639811
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavSlider>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavSlider_Statics::NewProp_Slider = { "Slider", nullptr, (EPropertyFlags)0x002008000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSlider, Slider), Z_Construct_UClass_USlider_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Slider_MetaData), NewProp_Slider_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUINavSlider_Statics::NewProp_NavSpinBox = { "NavSpinBox", nullptr, (EPropertyFlags)0x002008000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSlider, NavSpinBox), Z_Construct_UClass_USpinBox_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NavSpinBox_MetaData), NewProp_NavSpinBox_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UUINavSlider_Statics::NewProp_MinValue = { "MinValue", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSlider, MinValue), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MinValue_MetaData), NewProp_MinValue_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UUINavSlider_Statics::NewProp_MaxValue = { "MaxValue", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSlider, MaxValue), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxValue_MetaData), NewProp_MaxValue_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UUINavSlider_Statics::NewProp_Interval = { "Interval", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSlider, Interval), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Interval_MetaData), NewProp_Interval_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UUINavSlider_Statics::NewProp_MaxDecimalDigits = { "MaxDecimalDigits", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSlider, MaxDecimalDigits), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxDecimalDigits_MetaData), NewProp_MaxDecimalDigits_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UUINavSlider_Statics::NewProp_MinDecimalDigits = { "MinDecimalDigits", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSlider, MinDecimalDigits), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MinDecimalDigits_MetaData), NewProp_MinDecimalDigits_MetaData) };
void Z_Construct_UClass_UUINavSlider_Statics::NewProp_bUseComma_SetBit(void* Obj)
{
	((UUINavSlider*)Obj)->bUseComma = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavSlider_Statics::NewProp_bUseComma = { "bUseComma", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavSlider), &Z_Construct_UClass_UUINavSlider_Statics::NewProp_bUseComma_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bUseComma_MetaData), NewProp_bUseComma_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UUINavSlider_Statics::NewProp_HandleHoverColor = { "HandleHoverColor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSlider, HandleHoverColor), Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HandleHoverColor_MetaData), NewProp_HandleHoverColor_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UUINavSlider_Statics::NewProp_BarHoverColor = { "BarHoverColor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavSlider, BarHoverColor), Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BarHoverColor_MetaData), NewProp_BarHoverColor_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavSlider_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSlider_Statics::NewProp_Slider,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSlider_Statics::NewProp_NavSpinBox,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSlider_Statics::NewProp_MinValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSlider_Statics::NewProp_MaxValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSlider_Statics::NewProp_Interval,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSlider_Statics::NewProp_MaxDecimalDigits,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSlider_Statics::NewProp_MinDecimalDigits,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSlider_Statics::NewProp_bUseComma,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSlider_Statics::NewProp_HandleHoverColor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavSlider_Statics::NewProp_BarHoverColor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSlider_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavSlider_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUINavHorizontalComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSlider_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavSlider_Statics::ClassParams = {
	&UUINavSlider::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UUINavSlider_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSlider_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavSlider_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavSlider_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavSlider()
{
	if (!Z_Registration_Info_UClass_UUINavSlider.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavSlider.OuterSingleton, Z_Construct_UClass_UUINavSlider_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavSlider.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavSlider>()
{
	return UUINavSlider::StaticClass();
}
UUINavSlider::UUINavSlider(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavSlider);
UUINavSlider::~UUINavSlider() {}
// End Class UUINavSlider

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavSlider, UUINavSlider::StaticClass, TEXT("UUINavSlider"), &Z_Registration_Info_UClass_UUINavSlider, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavSlider), 3505850604U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_3725687875(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
