// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "UINavPromptWidget.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPromptDataBase;
class UUINavComponent;
#ifdef UINAVIGATION_UINavPromptWidget_generated_h
#error "UINavPromptWidget.generated.h already included, missing '#pragma once' in UINavPromptWidget.h"
#endif
#define UINAVIGATION_UINavPromptWidget_generated_h

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execExecuteCallback); \
	DECLARE_FUNCTION(execSetCallback); \
	DECLARE_FUNCTION(execIsAcceptComponent); \
	DECLARE_FUNCTION(execProcessPromptWidgetSelected);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_14_CALLBACK_WRAPPERS
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUUINavPromptWidget(); \
	friend struct Z_Construct_UClass_UUINavPromptWidget_Statics; \
public: \
	DECLARE_CLASS(UUINavPromptWidget, UUINavWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/UINavigation"), NO_API) \
	DECLARE_SERIALIZER(UUINavPromptWidget)


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_14_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UUINavPromptWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UUINavPromptWidget(UUINavPromptWidget&&); \
	UUINavPromptWidget(const UUINavPromptWidget&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUINavPromptWidget); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUINavPromptWidget); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUINavPromptWidget) \
	NO_API virtual ~UUINavPromptWidget();


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_11_PROLOG
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_14_CALLBACK_WRAPPERS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_14_INCLASS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UINAVIGATION_API UClass* StaticClass<class UUINavPromptWidget>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPromptWidget_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
