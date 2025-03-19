// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavHorizontalComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavHorizontalComponent() {}

// Begin Cross Module References
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavComponent();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavHorizontalComponent();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavHorizontalComponent_NoRegister();
UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_OnValueChangedEvent__DelegateSignature();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Delegate FOnValueChangedEvent
struct Z_Construct_UDelegateFunction_UINavigation_OnValueChangedEvent__DelegateSignature_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_UINavigation_OnValueChangedEvent__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_UINavigation, nullptr, "OnValueChangedEvent__DelegateSignature", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_UINavigation_OnValueChangedEvent__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_UINavigation_OnValueChangedEvent__DelegateSignature_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UDelegateFunction_UINavigation_OnValueChangedEvent__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_UINavigation_OnValueChangedEvent__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnValueChangedEvent_DelegateWrapper(const FMulticastScriptDelegate& OnValueChangedEvent)
{
	OnValueChangedEvent.ProcessMulticastDelegate<UObject>(NULL);
}
// End Delegate FOnValueChangedEvent

// Begin Class UUINavHorizontalComponent Function GetMaxOptionIndex
struct Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics
{
	struct UINavHorizontalComponent_eventGetMaxOptionIndex_Parms
	{
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavComponentBox" },
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavHorizontalComponent_eventGetMaxOptionIndex_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavHorizontalComponent, nullptr, "GetMaxOptionIndex", nullptr, nullptr, Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::UINavHorizontalComponent_eventGetMaxOptionIndex_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::UINavHorizontalComponent_eventGetMaxOptionIndex_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavHorizontalComponent::execGetMaxOptionIndex)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=P_THIS->GetMaxOptionIndex();
	P_NATIVE_END;
}
// End Class UUINavHorizontalComponent Function GetMaxOptionIndex

// Begin Class UUINavHorizontalComponent Function NavigateLeft
struct Z_Construct_UFunction_UUINavHorizontalComponent_NavigateLeft_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavHorizontalComponent" },
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavHorizontalComponent_NavigateLeft_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavHorizontalComponent, nullptr, "NavigateLeft", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_NavigateLeft_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavHorizontalComponent_NavigateLeft_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavHorizontalComponent_NavigateLeft()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavHorizontalComponent_NavigateLeft_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavHorizontalComponent::execNavigateLeft)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->NavigateLeft();
	P_NATIVE_END;
}
// End Class UUINavHorizontalComponent Function NavigateLeft

// Begin Class UUINavHorizontalComponent Function NavigateRight
struct Z_Construct_UFunction_UUINavHorizontalComponent_NavigateRight_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavHorizontalComponent" },
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavHorizontalComponent_NavigateRight_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavHorizontalComponent, nullptr, "NavigateRight", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_NavigateRight_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavHorizontalComponent_NavigateRight_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavHorizontalComponent_NavigateRight()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavHorizontalComponent_NavigateRight_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavHorizontalComponent::execNavigateRight)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->NavigateRight();
	P_NATIVE_END;
}
// End Class UUINavHorizontalComponent Function NavigateRight

// Begin Class UUINavHorizontalComponent Function OnNavigateLeft
static FName NAME_UUINavHorizontalComponent_OnNavigateLeft = FName(TEXT("OnNavigateLeft"));
void UUINavHorizontalComponent::OnNavigateLeft()
{
	ProcessEvent(FindFunctionChecked(NAME_UUINavHorizontalComponent_OnNavigateLeft),NULL);
}
struct Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateLeft_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavHorizontalComponent" },
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateLeft_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavHorizontalComponent, nullptr, "OnNavigateLeft", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateLeft_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateLeft_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateLeft()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateLeft_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavHorizontalComponent::execOnNavigateLeft)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnNavigateLeft_Implementation();
	P_NATIVE_END;
}
// End Class UUINavHorizontalComponent Function OnNavigateLeft

// Begin Class UUINavHorizontalComponent Function OnNavigateRight
static FName NAME_UUINavHorizontalComponent_OnNavigateRight = FName(TEXT("OnNavigateRight"));
void UUINavHorizontalComponent::OnNavigateRight()
{
	ProcessEvent(FindFunctionChecked(NAME_UUINavHorizontalComponent_OnNavigateRight),NULL);
}
struct Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateRight_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavHorizontalComponent" },
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateRight_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavHorizontalComponent, nullptr, "OnNavigateRight", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateRight_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateRight_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateRight()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateRight_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavHorizontalComponent::execOnNavigateRight)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnNavigateRight_Implementation();
	P_NATIVE_END;
}
// End Class UUINavHorizontalComponent Function OnNavigateRight

// Begin Class UUINavHorizontalComponent Function OnUpdated
static FName NAME_UUINavHorizontalComponent_OnUpdated = FName(TEXT("OnUpdated"));
void UUINavHorizontalComponent::OnUpdated()
{
	ProcessEvent(FindFunctionChecked(NAME_UUINavHorizontalComponent_OnUpdated),NULL);
}
struct Z_Construct_UFunction_UUINavHorizontalComponent_OnUpdated_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavHorizontalComponent" },
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavHorizontalComponent_OnUpdated_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavHorizontalComponent, nullptr, "OnUpdated", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_OnUpdated_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavHorizontalComponent_OnUpdated_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavHorizontalComponent_OnUpdated()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavHorizontalComponent_OnUpdated_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavHorizontalComponent::execOnUpdated)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnUpdated_Implementation();
	P_NATIVE_END;
}
// End Class UUINavHorizontalComponent Function OnUpdated

// Begin Class UUINavHorizontalComponent Function SetOptionIndex
struct Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics
{
	struct UINavHorizontalComponent_eventSetOptionIndex_Parms
	{
		int32 NewIndex;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavComponentBox" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Changes the text displayed to match the specified option index\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Changes the text displayed to match the specified option index" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_NewIndex;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::NewProp_NewIndex = { "NewIndex", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavHorizontalComponent_eventSetOptionIndex_Parms, NewIndex), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavHorizontalComponent_eventSetOptionIndex_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavHorizontalComponent_eventSetOptionIndex_Parms), &Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::NewProp_NewIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavHorizontalComponent, nullptr, "SetOptionIndex", nullptr, nullptr, Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::UINavHorizontalComponent_eventSetOptionIndex_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::UINavHorizontalComponent_eventSetOptionIndex_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavHorizontalComponent::execSetOptionIndex)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_NewIndex);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->SetOptionIndex(Z_Param_NewIndex);
	P_NATIVE_END;
}
// End Class UUINavHorizontalComponent Function SetOptionIndex

// Begin Class UUINavHorizontalComponent Function Update
struct Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics
{
	struct UINavHorizontalComponent_eventUpdate_Parms
	{
		bool bNotify;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavHorizontalComponent" },
		{ "CPP_Default_bNotify", "true" },
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bNotify_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static void NewProp_bNotify_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bNotify;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::NewProp_bNotify_SetBit(void* Obj)
{
	((UINavHorizontalComponent_eventUpdate_Parms*)Obj)->bNotify = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::NewProp_bNotify = { "bNotify", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavHorizontalComponent_eventUpdate_Parms), &Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::NewProp_bNotify_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bNotify_MetaData), NewProp_bNotify_MetaData) };
void Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((UINavHorizontalComponent_eventUpdate_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavHorizontalComponent_eventUpdate_Parms), &Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::NewProp_bNotify,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavHorizontalComponent, nullptr, "Update", nullptr, nullptr, Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::PropPointers), sizeof(Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::UINavHorizontalComponent_eventUpdate_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::UINavHorizontalComponent_eventUpdate_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavHorizontalComponent_Update()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavHorizontalComponent_Update_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUINavHorizontalComponent::execUpdate)
{
	P_GET_UBOOL(Z_Param_bNotify);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->Update(Z_Param_bNotify);
	P_NATIVE_END;
}
// End Class UUINavHorizontalComponent Function Update

// Begin Class UUINavHorizontalComponent
void UUINavHorizontalComponent::StaticRegisterNativesUUINavHorizontalComponent()
{
	UClass* Class = UUINavHorizontalComponent::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetMaxOptionIndex", &UUINavHorizontalComponent::execGetMaxOptionIndex },
		{ "NavigateLeft", &UUINavHorizontalComponent::execNavigateLeft },
		{ "NavigateRight", &UUINavHorizontalComponent::execNavigateRight },
		{ "OnNavigateLeft", &UUINavHorizontalComponent::execOnNavigateLeft },
		{ "OnNavigateRight", &UUINavHorizontalComponent::execOnNavigateRight },
		{ "OnUpdated", &UUINavHorizontalComponent::execOnUpdated },
		{ "SetOptionIndex", &UUINavHorizontalComponent::execSetOptionIndex },
		{ "Update", &UUINavHorizontalComponent::execUpdate },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavHorizontalComponent);
UClass* Z_Construct_UClass_UUINavHorizontalComponent_NoRegister()
{
	return UUINavHorizontalComponent::StaticClass();
}
struct Z_Construct_UClass_UUINavHorizontalComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UINavHorizontalComponent.h" },
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OptionIndex_MetaData[] = {
		{ "Category", "UINavHorizontalComponent" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Indicates the option that should appear first in the slider\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Indicates the option that should appear first in the slider" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bLoopOptions_MetaData[] = {
		{ "Category", "UINavComponentBox" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//If set to true, will loop between options (won't disable buttons, even if DisableButtons is set to true)\n" },
#endif
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "If set to true, will loop between options (won't disable buttons, even if DisableButtons is set to true)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnValueChanged_MetaData[] = {
		{ "Category", "Appearance|Event" },
		{ "ModuleRelativePath", "Public/UINavHorizontalComponent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_OptionIndex;
	static void NewProp_bLoopOptions_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bLoopOptions;
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnValueChanged;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUINavHorizontalComponent_GetMaxOptionIndex, "GetMaxOptionIndex" }, // 101903236
		{ &Z_Construct_UFunction_UUINavHorizontalComponent_NavigateLeft, "NavigateLeft" }, // 523178494
		{ &Z_Construct_UFunction_UUINavHorizontalComponent_NavigateRight, "NavigateRight" }, // 1273646347
		{ &Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateLeft, "OnNavigateLeft" }, // 3551589259
		{ &Z_Construct_UFunction_UUINavHorizontalComponent_OnNavigateRight, "OnNavigateRight" }, // 509224441
		{ &Z_Construct_UFunction_UUINavHorizontalComponent_OnUpdated, "OnUpdated" }, // 2372231431
		{ &Z_Construct_UFunction_UUINavHorizontalComponent_SetOptionIndex, "SetOptionIndex" }, // 3551068915
		{ &Z_Construct_UFunction_UUINavHorizontalComponent_Update, "Update" }, // 1470256871
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUINavHorizontalComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UUINavHorizontalComponent_Statics::NewProp_OptionIndex = { "OptionIndex", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavHorizontalComponent, OptionIndex), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OptionIndex_MetaData), NewProp_OptionIndex_MetaData) };
void Z_Construct_UClass_UUINavHorizontalComponent_Statics::NewProp_bLoopOptions_SetBit(void* Obj)
{
	((UUINavHorizontalComponent*)Obj)->bLoopOptions = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UUINavHorizontalComponent_Statics::NewProp_bLoopOptions = { "bLoopOptions", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UUINavHorizontalComponent), &Z_Construct_UClass_UUINavHorizontalComponent_Statics::NewProp_bLoopOptions_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bLoopOptions_MetaData), NewProp_bLoopOptions_MetaData) };
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UUINavHorizontalComponent_Statics::NewProp_OnValueChanged = { "OnValueChanged", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUINavHorizontalComponent, OnValueChanged), Z_Construct_UDelegateFunction_UINavigation_OnValueChangedEvent__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnValueChanged_MetaData), NewProp_OnValueChanged_MetaData) }; // 3254488210
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUINavHorizontalComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavHorizontalComponent_Statics::NewProp_OptionIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavHorizontalComponent_Statics::NewProp_bLoopOptions,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUINavHorizontalComponent_Statics::NewProp_OnValueChanged,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavHorizontalComponent_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UUINavHorizontalComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUINavComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavHorizontalComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavHorizontalComponent_Statics::ClassParams = {
	&UUINavHorizontalComponent::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UUINavHorizontalComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UUINavHorizontalComponent_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavHorizontalComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavHorizontalComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavHorizontalComponent()
{
	if (!Z_Registration_Info_UClass_UUINavHorizontalComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavHorizontalComponent.OuterSingleton, Z_Construct_UClass_UUINavHorizontalComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavHorizontalComponent.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavHorizontalComponent>()
{
	return UUINavHorizontalComponent::StaticClass();
}
UUINavHorizontalComponent::UUINavHorizontalComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavHorizontalComponent);
UUINavHorizontalComponent::~UUINavHorizontalComponent() {}
// End Class UUINavHorizontalComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavHorizontalComponent, UUINavHorizontalComponent::StaticClass, TEXT("UUINavHorizontalComponent"), &Z_Registration_Info_UClass_UUINavHorizontalComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavHorizontalComponent), 684506294U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_1615126224(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
