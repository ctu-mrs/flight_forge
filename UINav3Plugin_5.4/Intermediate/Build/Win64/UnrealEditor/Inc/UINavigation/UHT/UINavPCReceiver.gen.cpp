// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UINavigation/Public/UINavPCReceiver.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavPCReceiver() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
SLATECORE_API UEnum* Z_Construct_UEnum_SlateCore_EUINavigation();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPCReceiver();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavPCReceiver_NoRegister();
UINAVIGATION_API UClass* Z_Construct_UClass_UUINavWidget_NoRegister();
UINAVIGATION_API UEnum* Z_Construct_UEnum_UINavigation_EInputType();
UPackage* Z_Construct_UPackage__Script_UINavigation();
// End Cross Module References

// Begin Interface UUINavPCReceiver Function OnActiveWidgetChanged
struct UINavPCReceiver_eventOnActiveWidgetChanged_Parms
{
	UUINavWidget* OldActiveWidget;
	UUINavWidget* NewActiveWidget;
};
void IUINavPCReceiver::OnActiveWidgetChanged(UUINavWidget* OldActiveWidget, UUINavWidget* NewActiveWidget)
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnActiveWidgetChanged instead.");
}
static FName NAME_UUINavPCReceiver_OnActiveWidgetChanged = FName(TEXT("OnActiveWidgetChanged"));
void IUINavPCReceiver::Execute_OnActiveWidgetChanged(UObject* O, UUINavWidget* OldActiveWidget, UUINavWidget* NewActiveWidget)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UINavPCReceiver_eventOnActiveWidgetChanged_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnActiveWidgetChanged);
	if (Func)
	{
		Parms.OldActiveWidget=OldActiveWidget;
		Parms.NewActiveWidget=NewActiveWidget;
		O->ProcessEvent(Func, &Parms);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnActiveWidgetChanged_Implementation(OldActiveWidget,NewActiveWidget);
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the active widget changes\n\x09*\n\x09*\x09@param OldActiveWidget The previously active widget\n\x09*\x09@param NewActiveWidget The new active widget\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the active widget changes\n\n@param OldActiveWidget The previously active widget\n@param NewActiveWidget The new active widget" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OldActiveWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NewActiveWidget_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OldActiveWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_NewActiveWidget;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::NewProp_OldActiveWidget = { "OldActiveWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCReceiver_eventOnActiveWidgetChanged_Parms, OldActiveWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OldActiveWidget_MetaData), NewProp_OldActiveWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::NewProp_NewActiveWidget = { "NewActiveWidget", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCReceiver_eventOnActiveWidgetChanged_Parms, NewActiveWidget), Z_Construct_UClass_UUINavWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NewActiveWidget_MetaData), NewProp_NewActiveWidget_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::NewProp_OldActiveWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::NewProp_NewActiveWidget,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnActiveWidgetChanged", nullptr, nullptr, Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::PropPointers), sizeof(UINavPCReceiver_eventOnActiveWidgetChanged_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavPCReceiver_eventOnActiveWidgetChanged_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnActiveWidgetChanged)
{
	P_GET_OBJECT(UUINavWidget,Z_Param_OldActiveWidget);
	P_GET_OBJECT(UUINavWidget,Z_Param_NewActiveWidget);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnActiveWidgetChanged_Implementation(Z_Param_OldActiveWidget,Z_Param_NewActiveWidget);
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnActiveWidgetChanged

// Begin Interface UUINavPCReceiver Function OnControllerConnectionChanged
struct UINavPCReceiver_eventOnControllerConnectionChanged_Parms
{
	bool bConnected;
	int32 UserId;
	int32 UserIndex;
};
void IUINavPCReceiver::OnControllerConnectionChanged(bool bConnected, int32 UserId, int32 UserIndex)
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnControllerConnectionChanged instead.");
}
static FName NAME_UUINavPCReceiver_OnControllerConnectionChanged = FName(TEXT("OnControllerConnectionChanged"));
void IUINavPCReceiver::Execute_OnControllerConnectionChanged(UObject* O, bool bConnected, int32 UserId, int32 UserIndex)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UINavPCReceiver_eventOnControllerConnectionChanged_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnControllerConnectionChanged);
	if (Func)
	{
		Parms.bConnected=bConnected;
		Parms.UserId=UserId;
		Parms.UserIndex=UserIndex;
		O->ProcessEvent(Func, &Parms);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnControllerConnectionChanged_Implementation(bConnected,UserId,UserIndex);
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when a controller is connected and disconnected\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when a controller is connected and disconnected" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bConnected_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bConnected;
	static const UECodeGen_Private::FIntPropertyParams NewProp_UserId;
	static const UECodeGen_Private::FIntPropertyParams NewProp_UserIndex;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::NewProp_bConnected_SetBit(void* Obj)
{
	((UINavPCReceiver_eventOnControllerConnectionChanged_Parms*)Obj)->bConnected = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::NewProp_bConnected = { "bConnected", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UINavPCReceiver_eventOnControllerConnectionChanged_Parms), &Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::NewProp_bConnected_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::NewProp_UserId = { "UserId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCReceiver_eventOnControllerConnectionChanged_Parms, UserId), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::NewProp_UserIndex = { "UserIndex", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCReceiver_eventOnControllerConnectionChanged_Parms, UserIndex), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::NewProp_bConnected,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::NewProp_UserId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::NewProp_UserIndex,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnControllerConnectionChanged", nullptr, nullptr, Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::PropPointers), sizeof(UINavPCReceiver_eventOnControllerConnectionChanged_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavPCReceiver_eventOnControllerConnectionChanged_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnControllerConnectionChanged)
{
	P_GET_UBOOL(Z_Param_bConnected);
	P_GET_PROPERTY(FIntProperty,Z_Param_UserId);
	P_GET_PROPERTY(FIntProperty,Z_Param_UserIndex);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnControllerConnectionChanged_Implementation(Z_Param_bConnected,Z_Param_UserId,Z_Param_UserIndex);
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnControllerConnectionChanged

// Begin Interface UUINavPCReceiver Function OnInputChanged
struct UINavPCReceiver_eventOnInputChanged_Parms
{
	EInputType From;
	EInputType To;
};
void IUINavPCReceiver::OnInputChanged(EInputType From, EInputType To)
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnInputChanged instead.");
}
static FName NAME_UUINavPCReceiver_OnInputChanged = FName(TEXT("OnInputChanged"));
void IUINavPCReceiver::Execute_OnInputChanged(UObject* O, EInputType From, EInputType To)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UINavPCReceiver_eventOnInputChanged_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnInputChanged);
	if (Func)
	{
		Parms.From=From;
		Parms.To=To;
		O->ProcessEvent(Func, &Parms);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnInputChanged_Implementation(From,To);
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the input type changes\n\x09*\n\x09*\x09@param From The input type being used before\n\x09*\x09@param To The input type being used now\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the input type changes\n\n@param From The input type being used before\n@param To The input type being used now" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_From_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_From;
	static const UECodeGen_Private::FBytePropertyParams NewProp_To_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_To;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::NewProp_From_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::NewProp_From = { "From", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCReceiver_eventOnInputChanged_Parms, From), Z_Construct_UEnum_UINavigation_EInputType, METADATA_PARAMS(0, nullptr) }; // 346009216
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::NewProp_To_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::NewProp_To = { "To", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCReceiver_eventOnInputChanged_Parms, To), Z_Construct_UEnum_UINavigation_EInputType, METADATA_PARAMS(0, nullptr) }; // 346009216
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::NewProp_From_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::NewProp_From,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::NewProp_To_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::NewProp_To,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnInputChanged", nullptr, nullptr, Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::PropPointers), sizeof(UINavPCReceiver_eventOnInputChanged_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavPCReceiver_eventOnInputChanged_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnInputChanged)
{
	P_GET_ENUM(EInputType,Z_Param_From);
	P_GET_ENUM(EInputType,Z_Param_To);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnInputChanged_Implementation(EInputType(Z_Param_From),EInputType(Z_Param_To));
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnInputChanged

// Begin Interface UUINavPCReceiver Function OnNavigated
struct UINavPCReceiver_eventOnNavigated_Parms
{
	EUINavigation NavigationDirection;
};
void IUINavPCReceiver::OnNavigated(EUINavigation NavigationDirection)
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnNavigated instead.");
}
static FName NAME_UUINavPCReceiver_OnNavigated = FName(TEXT("OnNavigated"));
void IUINavPCReceiver::Execute_OnNavigated(UObject* O, EUINavigation NavigationDirection)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UINavPCReceiver_eventOnNavigated_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnNavigated);
	if (Func)
	{
		Parms.NavigationDirection=NavigationDirection;
		O->ProcessEvent(Func, &Parms);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnNavigated_Implementation(NavigationDirection);
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the player navigates in a certain direction\n\x09*\n\x09*\x09@param NavigationDirection The direction of navigation\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the player navigates in a certain direction\n\n@param NavigationDirection The direction of navigation" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_NavigationDirection_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_NavigationDirection;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::NewProp_NavigationDirection_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::NewProp_NavigationDirection = { "NavigationDirection", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCReceiver_eventOnNavigated_Parms, NavigationDirection), Z_Construct_UEnum_SlateCore_EUINavigation, METADATA_PARAMS(0, nullptr) }; // 2993106650
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::NewProp_NavigationDirection_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::NewProp_NavigationDirection,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnNavigated", nullptr, nullptr, Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::PropPointers), sizeof(UINavPCReceiver_eventOnNavigated_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavPCReceiver_eventOnNavigated_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnNavigated()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnNavigated_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnNavigated)
{
	P_GET_ENUM(EUINavigation,Z_Param_NavigationDirection);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnNavigated_Implementation(EUINavigation(Z_Param_NavigationDirection));
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnNavigated

// Begin Interface UUINavPCReceiver Function OnNext
void IUINavPCReceiver::OnNext()
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnNext instead.");
}
static FName NAME_UUINavPCReceiver_OnNext = FName(TEXT("OnNext"));
void IUINavPCReceiver::Execute_OnNext(UObject* O)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnNext);
	if (Func)
	{
		O->ProcessEvent(Func, NULL);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnNext_Implementation();
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnNext_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the player navigates to the next section\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the player navigates to the next section" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnNext_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnNext", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnNext_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnNext_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnNext()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnNext_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnNext)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnNext_Implementation();
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnNext

// Begin Interface UUINavPCReceiver Function OnPrevious
void IUINavPCReceiver::OnPrevious()
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnPrevious instead.");
}
static FName NAME_UUINavPCReceiver_OnPrevious = FName(TEXT("OnPrevious"));
void IUINavPCReceiver::Execute_OnPrevious(UObject* O)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnPrevious);
	if (Func)
	{
		O->ProcessEvent(Func, NULL);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnPrevious_Implementation();
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnPrevious_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the player navigates to the previous section\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the player navigates to the previous section" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnPrevious_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnPrevious", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnPrevious_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnPrevious_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnPrevious()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnPrevious_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnPrevious)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnPrevious_Implementation();
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnPrevious

// Begin Interface UUINavPCReceiver Function OnReturn
void IUINavPCReceiver::OnReturn()
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnReturn instead.");
}
static FName NAME_UUINavPCReceiver_OnReturn = FName(TEXT("OnReturn"));
void IUINavPCReceiver::Execute_OnReturn(UObject* O)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnReturn);
	if (Func)
	{
		O->ProcessEvent(Func, NULL);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnReturn_Implementation();
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnReturn_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the player returns to the previous widget\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the player returns to the previous widget" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnReturn_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnReturn", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnReturn_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnReturn_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnReturn()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnReturn_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnReturn)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnReturn_Implementation();
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnReturn

// Begin Interface UUINavPCReceiver Function OnRootWidgetAdded
void IUINavPCReceiver::OnRootWidgetAdded()
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnRootWidgetAdded instead.");
}
static FName NAME_UUINavPCReceiver_OnRootWidgetAdded = FName(TEXT("OnRootWidgetAdded"));
void IUINavPCReceiver::Execute_OnRootWidgetAdded(UObject* O)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnRootWidgetAdded);
	if (Func)
	{
		O->ProcessEvent(Func, NULL);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnRootWidgetAdded_Implementation();
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetAdded_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the root UINavWidget is added to the viewport\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the root UINavWidget is added to the viewport" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetAdded_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnRootWidgetAdded", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetAdded_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetAdded_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetAdded()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetAdded_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnRootWidgetAdded)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnRootWidgetAdded_Implementation();
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnRootWidgetAdded

// Begin Interface UUINavPCReceiver Function OnRootWidgetRemoved
void IUINavPCReceiver::OnRootWidgetRemoved()
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnRootWidgetRemoved instead.");
}
static FName NAME_UUINavPCReceiver_OnRootWidgetRemoved = FName(TEXT("OnRootWidgetRemoved"));
void IUINavPCReceiver::Execute_OnRootWidgetRemoved(UObject* O)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnRootWidgetRemoved);
	if (Func)
	{
		O->ProcessEvent(Func, NULL);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnRootWidgetRemoved_Implementation();
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetRemoved_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the root UINavWidget is removed from the viewport\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the root UINavWidget is removed from the viewport" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetRemoved_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnRootWidgetRemoved", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetRemoved_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetRemoved_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetRemoved()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetRemoved_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnRootWidgetRemoved)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnRootWidgetRemoved_Implementation();
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnRootWidgetRemoved

// Begin Interface UUINavPCReceiver Function OnSelect
void IUINavPCReceiver::OnSelect()
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnSelect instead.");
}
static FName NAME_UUINavPCReceiver_OnSelect = FName(TEXT("OnSelect"));
void IUINavPCReceiver::Execute_OnSelect(UObject* O)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnSelect);
	if (Func)
	{
		O->ProcessEvent(Func, NULL);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnSelect_Implementation();
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnSelect_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the player selects the current option\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called when the player selects the current option" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnSelect_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnSelect", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnSelect_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnSelect_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnSelect()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnSelect_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnSelect)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnSelect_Implementation();
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnSelect

// Begin Interface UUINavPCReceiver Function OnThumbstickCursorInput
struct UINavPCReceiver_eventOnThumbstickCursorInput_Parms
{
	FVector2D ThumbstickDelta;
};
void IUINavPCReceiver::OnThumbstickCursorInput(FVector2D const& ThumbstickDelta)
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_OnThumbstickCursorInput instead.");
}
static FName NAME_UUINavPCReceiver_OnThumbstickCursorInput = FName(TEXT("OnThumbstickCursorInput"));
void IUINavPCReceiver::Execute_OnThumbstickCursorInput(UObject* O, FVector2D const& ThumbstickDelta)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UUINavPCReceiver::StaticClass()));
	UINavPCReceiver_eventOnThumbstickCursorInput_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UUINavPCReceiver_OnThumbstickCursorInput);
	if (Func)
	{
		Parms.ThumbstickDelta=ThumbstickDelta;
		O->ProcessEvent(Func, &Parms);
	}
	else if (auto I = (IUINavPCReceiver*)(O->GetNativeInterfaceAddress(UUINavPCReceiver::StaticClass())))
	{
		I->OnThumbstickCursorInput_Implementation(ThumbstickDelta);
	}
}
struct Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UINavController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09*\x09""Called when the gamepad's thumbstick moves\n\x09*/" },
#endif
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
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
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics::NewProp_ThumbstickDelta = { "ThumbstickDelta", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UINavPCReceiver_eventOnThumbstickCursorInput_Parms, ThumbstickDelta), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ThumbstickDelta_MetaData), NewProp_ThumbstickDelta_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics::NewProp_ThumbstickDelta,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUINavPCReceiver, nullptr, "OnThumbstickCursorInput", nullptr, nullptr, Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics::PropPointers), sizeof(UINavPCReceiver_eventOnThumbstickCursorInput_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0CC20C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics::Function_MetaDataParams) };
static_assert(sizeof(UINavPCReceiver_eventOnThumbstickCursorInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IUINavPCReceiver::execOnThumbstickCursorInput)
{
	P_GET_STRUCT_REF(FVector2D,Z_Param_Out_ThumbstickDelta);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnThumbstickCursorInput_Implementation(Z_Param_Out_ThumbstickDelta);
	P_NATIVE_END;
}
// End Interface UUINavPCReceiver Function OnThumbstickCursorInput

// Begin Interface UUINavPCReceiver
void UUINavPCReceiver::StaticRegisterNativesUUINavPCReceiver()
{
	UClass* Class = UUINavPCReceiver::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "OnActiveWidgetChanged", &IUINavPCReceiver::execOnActiveWidgetChanged },
		{ "OnControllerConnectionChanged", &IUINavPCReceiver::execOnControllerConnectionChanged },
		{ "OnInputChanged", &IUINavPCReceiver::execOnInputChanged },
		{ "OnNavigated", &IUINavPCReceiver::execOnNavigated },
		{ "OnNext", &IUINavPCReceiver::execOnNext },
		{ "OnPrevious", &IUINavPCReceiver::execOnPrevious },
		{ "OnReturn", &IUINavPCReceiver::execOnReturn },
		{ "OnRootWidgetAdded", &IUINavPCReceiver::execOnRootWidgetAdded },
		{ "OnRootWidgetRemoved", &IUINavPCReceiver::execOnRootWidgetRemoved },
		{ "OnSelect", &IUINavPCReceiver::execOnSelect },
		{ "OnThumbstickCursorInput", &IUINavPCReceiver::execOnThumbstickCursorInput },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUINavPCReceiver);
UClass* Z_Construct_UClass_UUINavPCReceiver_NoRegister()
{
	return UUINavPCReceiver::StaticClass();
}
struct Z_Construct_UClass_UUINavPCReceiver_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/UINavPCReceiver.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnActiveWidgetChanged, "OnActiveWidgetChanged" }, // 1215980767
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnControllerConnectionChanged, "OnControllerConnectionChanged" }, // 708461380
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnInputChanged, "OnInputChanged" }, // 488034218
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnNavigated, "OnNavigated" }, // 258029540
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnNext, "OnNext" }, // 2539721729
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnPrevious, "OnPrevious" }, // 1392661333
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnReturn, "OnReturn" }, // 24692999
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetAdded, "OnRootWidgetAdded" }, // 1051398624
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnRootWidgetRemoved, "OnRootWidgetRemoved" }, // 1949113299
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnSelect, "OnSelect" }, // 2041694786
		{ &Z_Construct_UFunction_UUINavPCReceiver_OnThumbstickCursorInput, "OnThumbstickCursorInput" }, // 3896661665
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IUINavPCReceiver>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UUINavPCReceiver_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInterface,
	(UObject* (*)())Z_Construct_UPackage__Script_UINavigation,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavPCReceiver_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUINavPCReceiver_Statics::ClassParams = {
	&UUINavPCReceiver::StaticClass,
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
	0x000840A1u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUINavPCReceiver_Statics::Class_MetaDataParams), Z_Construct_UClass_UUINavPCReceiver_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUINavPCReceiver()
{
	if (!Z_Registration_Info_UClass_UUINavPCReceiver.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUINavPCReceiver.OuterSingleton, Z_Construct_UClass_UUINavPCReceiver_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUINavPCReceiver.OuterSingleton;
}
template<> UINAVIGATION_API UClass* StaticClass<UUINavPCReceiver>()
{
	return UUINavPCReceiver::StaticClass();
}
UUINavPCReceiver::UUINavPCReceiver(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUINavPCReceiver);
UUINavPCReceiver::~UUINavPCReceiver() {}
// End Interface UUINavPCReceiver

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUINavPCReceiver, UUINavPCReceiver::StaticClass, TEXT("UUINavPCReceiver"), &Z_Registration_Info_UClass_UUINavPCReceiver, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUINavPCReceiver), 1279494643U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_375850245(TEXT("/Script/UINavigation"),
	Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
