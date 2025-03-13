// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "UINavHorizontalComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_UINavHorizontalComponent_generated_h
#error "UINavHorizontalComponent.generated.h already included, missing '#pragma once' in UINavHorizontalComponent.h"
#endif
#define UINAVIGATION_UINavHorizontalComponent_generated_h

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_9_DELEGATE \
UINAVIGATION_API void FOnValueChangedEvent_DelegateWrapper(const FMulticastScriptDelegate& OnValueChangedEvent);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnUpdated); \
	DECLARE_FUNCTION(execOnNavigateRight); \
	DECLARE_FUNCTION(execOnNavigateLeft); \
	DECLARE_FUNCTION(execNavigateRight); \
	DECLARE_FUNCTION(execNavigateLeft); \
	DECLARE_FUNCTION(execGetMaxOptionIndex); \
	DECLARE_FUNCTION(execSetOptionIndex); \
	DECLARE_FUNCTION(execUpdate);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_17_CALLBACK_WRAPPERS
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUUINavHorizontalComponent(); \
	friend struct Z_Construct_UClass_UUINavHorizontalComponent_Statics; \
public: \
	DECLARE_CLASS(UUINavHorizontalComponent, UUINavComponent, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/UINavigation"), NO_API) \
	DECLARE_SERIALIZER(UUINavHorizontalComponent)


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_17_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UUINavHorizontalComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UUINavHorizontalComponent(UUINavHorizontalComponent&&); \
	UUINavHorizontalComponent(const UUINavHorizontalComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUINavHorizontalComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUINavHorizontalComponent); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUINavHorizontalComponent) \
	NO_API virtual ~UUINavHorizontalComponent();


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_14_PROLOG
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_17_CALLBACK_WRAPPERS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_17_INCLASS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UINAVIGATION_API UClass* StaticClass<class UUINavHorizontalComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavHorizontalComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
