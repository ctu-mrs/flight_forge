// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "UINavSlider.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_UINavSlider_generated_h
#error "UINavSlider.generated.h already included, missing '#pragma once' in UINavSlider.h"
#endif
#define UINAVIGATION_UINavSlider_generated_h

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetSliderValue); \
	DECLARE_FUNCTION(execSetValueClamped); \
	DECLARE_FUNCTION(execGetCurrentValue); \
	DECLARE_FUNCTION(execHandleOnSpinBoxValueCommitted); \
	DECLARE_FUNCTION(execHandleOnSpinBoxValueChanged); \
	DECLARE_FUNCTION(execHandleOnSpinBoxMouseCaptureBegin); \
	DECLARE_FUNCTION(execHandleOnSliderMouseCaptureEnd); \
	DECLARE_FUNCTION(execHandleOnSliderMouseCaptureBegin); \
	DECLARE_FUNCTION(execHandleOnSliderValueChanged);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUUINavSlider(); \
	friend struct Z_Construct_UClass_UUINavSlider_Statics; \
public: \
	DECLARE_CLASS(UUINavSlider, UUINavHorizontalComponent, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/UINavigation"), NO_API) \
	DECLARE_SERIALIZER(UUINavSlider)


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_16_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UUINavSlider(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UUINavSlider(UUINavSlider&&); \
	UUINavSlider(const UUINavSlider&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUINavSlider); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUINavSlider); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUINavSlider) \
	NO_API virtual ~UUINavSlider();


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_13_PROLOG
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_16_INCLASS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UINAVIGATION_API UClass* StaticClass<class UUINavSlider>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavSlider_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
