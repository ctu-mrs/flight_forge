// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "UINavPCComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class APlayerController;
class UDataTable;
class UEnhancedInputComponent;
class UInputAction;
class UInputMappingContext;
class UTexture2D;
class UUINavPromptWidget;
class UUINavWidget;
enum class EAxisType : uint8;
enum class EInputAxis : uint8;
enum class EInputMode : uint8;
enum class EInputRestriction : uint8;
enum class EInputType : uint8;
enum class EUINavigation : uint8;
struct FKey;
struct FPlatformConfigData;
#ifdef UINAVIGATION_UINavPCComponent_generated_h
#error "UINavPCComponent.generated.h already included, missing '#pragma once' in UINavPCComponent.h"
#endif
#define UINAVIGATION_UINavPCComponent_generated_h

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_37_DELEGATE \
UINAVIGATION_API void FInputTypeChangedDelegate_DelegateWrapper(const FMulticastScriptDelegate& InputTypeChangedDelegate, EInputType InputType);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_38_DELEGATE \
UINAVIGATION_API void FUpdateInputIconsDelegate_DelegateWrapper(const FMulticastScriptDelegate& UpdateInputIconsDelegate);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_43_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FAxis2D_Keys_Statics; \
	UINAVIGATION_API static class UScriptStruct* StaticStruct();


template<> UINAVIGATION_API UScriptStruct* StaticStruct<struct FAxis2D_Keys>();

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_65_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execIsMovingThumbstick); \
	DECLARE_FUNCTION(execGetThumbstickDelta); \
	DECLARE_FUNCTION(execIsWidgetChild); \
	DECLARE_FUNCTION(execIsWidgetActive); \
	DECLARE_FUNCTION(execGetActiveSubWidget); \
	DECLARE_FUNCTION(execGetActiveWidget); \
	DECLARE_FUNCTION(execIsUsingGamepad); \
	DECLARE_FUNCTION(execIsUsingKeyboard); \
	DECLARE_FUNCTION(execIsUsingMouse); \
	DECLARE_FUNCTION(execGetCurrentPlatformData); \
	DECLARE_FUNCTION(execGetCurrentInputType); \
	DECLARE_FUNCTION(execGetPC); \
	DECLARE_FUNCTION(execNavigateInDirection); \
	DECLARE_FUNCTION(execGoToBuiltWidget); \
	DECLARE_FUNCTION(execGoToPromptWidget); \
	DECLARE_FUNCTION(execGoToWidget); \
	DECLARE_FUNCTION(execNotifyNavigatedTo); \
	DECLARE_FUNCTION(execSetActiveWidget); \
	DECLARE_FUNCTION(execGetUINavInputContext); \
	DECLARE_FUNCTION(execGetEnhancedInputComponent); \
	DECLARE_FUNCTION(execGetEnhancedInputKeys); \
	DECLARE_FUNCTION(execGetKeyText); \
	DECLARE_FUNCTION(execGetEnhancedInputText); \
	DECLARE_FUNCTION(execGetSoftEnhancedInputIcon); \
	DECLARE_FUNCTION(execGetEnhancedInputIcon); \
	DECLARE_FUNCTION(execGetSoftKeyIcon); \
	DECLARE_FUNCTION(execGetKeyIcon); \
	DECLARE_FUNCTION(execGetEnhancedInputKey); \
	DECLARE_FUNCTION(execIsAxis); \
	DECLARE_FUNCTION(execIsAxis2D); \
	DECLARE_FUNCTION(execGetOppositeAxis2DAxis); \
	DECLARE_FUNCTION(execGetOppositeAxisKey); \
	DECLARE_FUNCTION(execGetAxis2DFromAxis1D); \
	DECLARE_FUNCTION(execGetAxis1DFromAxis2D); \
	DECLARE_FUNCTION(execGetAxisFromKey); \
	DECLARE_FUNCTION(execGetAxisFromScaledKey); \
	DECLARE_FUNCTION(execGetKeyFromAxis); \
	DECLARE_FUNCTION(execGetInputMode); \
	DECLARE_FUNCTION(execSimulateMouseClick); \
	DECLARE_FUNCTION(execSimulateMouseRelease); \
	DECLARE_FUNCTION(execSimulateMousePress); \
	DECLARE_FUNCTION(execRemoveInputContext); \
	DECLARE_FUNCTION(execAddInputContext); \
	DECLARE_FUNCTION(execForceUpdateAllInputDisplays); \
	DECLARE_FUNCTION(execSetKeyboardInputDataTables); \
	DECLARE_FUNCTION(execSetGamepadInputDataTables); \
	DECLARE_FUNCTION(execSetAllowSectionInput); \
	DECLARE_FUNCTION(execSetAllowReturnInput); \
	DECLARE_FUNCTION(execSetAllowSelectInput); \
	DECLARE_FUNCTION(execSetAllowDirectionalInput); \
	DECLARE_FUNCTION(execSetAllowAllMenuInput); \
	DECLARE_FUNCTION(execRefreshNavigationKeys); \
	DECLARE_FUNCTION(execHidingMouseCursor); \
	DECLARE_FUNCTION(execSetShowMouseCursor); \
	DECLARE_FUNCTION(execAllowsSectionInput); \
	DECLARE_FUNCTION(execAllowsReturnInput); \
	DECLARE_FUNCTION(execAllowsSelectInput); \
	DECLARE_FUNCTION(execAllowsDirectionalInput); \
	DECLARE_FUNCTION(execAllowsAllMenuInput);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_65_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUUINavPCComponent(); \
	friend struct Z_Construct_UClass_UUINavPCComponent_Statics; \
public: \
	DECLARE_CLASS(UUINavPCComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/UINavigation"), NO_API) \
	DECLARE_SERIALIZER(UUINavPCComponent)


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_65_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UUINavPCComponent(UUINavPCComponent&&); \
	UUINavPCComponent(const UUINavPCComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUINavPCComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUINavPCComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UUINavPCComponent) \
	NO_API virtual ~UUINavPCComponent();


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_62_PROLOG
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_65_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_65_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_65_INCLASS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h_65_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UINAVIGATION_API UClass* StaticClass<class UUINavPCComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
