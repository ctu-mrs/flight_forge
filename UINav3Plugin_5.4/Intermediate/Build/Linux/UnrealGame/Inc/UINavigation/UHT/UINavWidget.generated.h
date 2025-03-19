// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "UINavWidget.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UScrollBox;
class UUINavComponent;
class UUINavPromptWidget;
class UUINavWidget;
enum class EInputType : uint8;
#ifdef UINAVIGATION_UINavWidget_generated_h
#error "UINavWidget.generated.h already included, missing '#pragma once' in UINavWidget.h"
#endif
#define UINAVIGATION_UINavWidget_generated_h

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_36_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execReturnToParent); \
	DECLARE_FUNCTION(execGoToBuiltWidget); \
	DECLARE_FUNCTION(execGoToPromptWidget); \
	DECLARE_FUNCTION(execGoToWidget); \
	DECLARE_FUNCTION(execIsSelectorValid); \
	DECLARE_FUNCTION(execGetMostOuterUINavWidget); \
	DECLARE_FUNCTION(execOnHorizCompUpdated); \
	DECLARE_FUNCTION(execOnHorizCompNavigateRight); \
	DECLARE_FUNCTION(execOnHorizCompNavigateLeft); \
	DECLARE_FUNCTION(execOnSetupCompleted); \
	DECLARE_FUNCTION(execPreSetup); \
	DECLARE_FUNCTION(execOnThumbstickCursorInput); \
	DECLARE_FUNCTION(execOnInputChanged); \
	DECLARE_FUNCTION(execOnChangedSection); \
	DECLARE_FUNCTION(execOnPrevious); \
	DECLARE_FUNCTION(execOnNext); \
	DECLARE_FUNCTION(execOnChildReturn); \
	DECLARE_FUNCTION(execOnReturn); \
	DECLARE_FUNCTION(execOnStopSelect); \
	DECLARE_FUNCTION(execOnStartSelect); \
	DECLARE_FUNCTION(execOnSelect); \
	DECLARE_FUNCTION(execOnNavigate); \
	DECLARE_FUNCTION(execIsRebindingInput); \
	DECLARE_FUNCTION(execIsSelectorVisible); \
	DECLARE_FUNCTION(execSetSelectorVisible); \
	DECLARE_FUNCTION(execSetSelectorScale); \
	DECLARE_FUNCTION(execUpdateSelectorLocation); \
	DECLARE_FUNCTION(execGetScrollBoxToFocus); \
	DECLARE_FUNCTION(execOnLostNavigation); \
	DECLARE_FUNCTION(execOnGainedNavigation); \
	DECLARE_FUNCTION(execGetInitialFocusComponent); \
	DECLARE_FUNCTION(execOnSectionButtonPressed10); \
	DECLARE_FUNCTION(execOnSectionButtonPressed9); \
	DECLARE_FUNCTION(execOnSectionButtonPressed8); \
	DECLARE_FUNCTION(execOnSectionButtonPressed7); \
	DECLARE_FUNCTION(execOnSectionButtonPressed6); \
	DECLARE_FUNCTION(execOnSectionButtonPressed5); \
	DECLARE_FUNCTION(execOnSectionButtonPressed4); \
	DECLARE_FUNCTION(execOnSectionButtonPressed3); \
	DECLARE_FUNCTION(execOnSectionButtonPressed2); \
	DECLARE_FUNCTION(execOnSectionButtonPressed1); \
	DECLARE_FUNCTION(execGoToSection); \
	DECLARE_FUNCTION(execGoToPreviousSection); \
	DECLARE_FUNCTION(execGoToNextSection); \
	DECLARE_FUNCTION(execConfigureUINavPC);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_36_CALLBACK_WRAPPERS
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_36_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUUINavWidget(); \
	friend struct Z_Construct_UClass_UUINavWidget_Statics; \
public: \
	DECLARE_CLASS(UUINavWidget, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/UINavigation"), NO_API) \
	DECLARE_SERIALIZER(UUINavWidget)


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_36_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UUINavWidget(UUINavWidget&&); \
	UUINavWidget(const UUINavWidget&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUINavWidget); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUINavWidget); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUINavWidget) \
	NO_API virtual ~UUINavWidget();


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_33_PROLOG
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_36_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_36_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_36_CALLBACK_WRAPPERS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_36_INCLASS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h_36_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UINAVIGATION_API UClass* StaticClass<class UUINavWidget>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavWidget_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
