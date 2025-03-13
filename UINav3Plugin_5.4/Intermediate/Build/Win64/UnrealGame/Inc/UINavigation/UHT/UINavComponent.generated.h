// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "UINavComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UScrollBox;
enum class EButtonStyle : uint8;
struct FLinearColor;
#ifdef UINAVIGATION_UINavComponent_generated_h
#error "UINavComponent.generated.h already included, missing '#pragma once' in UINavComponent.h"
#endif
#define UINAVIGATION_UINavComponent_generated_h

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_19_DELEGATE \
UINAVIGATION_API void FOnClickedEvent_DelegateWrapper(const FMulticastScriptDelegate& OnClickedEvent);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_20_DELEGATE \
UINAVIGATION_API void FOnPressedEvent_DelegateWrapper(const FMulticastScriptDelegate& OnPressedEvent);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_21_DELEGATE \
UINAVIGATION_API void FOnReleasedEvent_DelegateWrapper(const FMulticastScriptDelegate& OnReleasedEvent);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_22_DELEGATE \
UINAVIGATION_API void FOnNavigatedToEvent_DelegateWrapper(const FMulticastScriptDelegate& OnNavigatedToEvent);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_23_DELEGATE \
UINAVIGATION_API void FOnNavigatedFromEvent_DelegateWrapper(const FMulticastScriptDelegate& OnNavigatedFromEvent);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_40_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetParentScrollBox); \
	DECLARE_FUNCTION(execIsBeingNavigated); \
	DECLARE_FUNCTION(execCanBeNavigated); \
	DECLARE_FUNCTION(execSwitchTextColorToNavigated); \
	DECLARE_FUNCTION(execSwitchTextColorToDefault); \
	DECLARE_FUNCTION(execSwitchTextColorTo); \
	DECLARE_FUNCTION(execSwitchButtonStyle); \
	DECLARE_FUNCTION(execSetText); \
	DECLARE_FUNCTION(execOnButtonUnhovered); \
	DECLARE_FUNCTION(execOnButtonHovered); \
	DECLARE_FUNCTION(execOnButtonReleased); \
	DECLARE_FUNCTION(execOnButtonPressed); \
	DECLARE_FUNCTION(execOnButtonClicked); \
	DECLARE_FUNCTION(execOnNavigatedFrom); \
	DECLARE_FUNCTION(execOnNavigatedTo);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_40_CALLBACK_WRAPPERS
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_40_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUUINavComponent(); \
	friend struct Z_Construct_UClass_UUINavComponent_Statics; \
public: \
	DECLARE_CLASS(UUINavComponent, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/UINavigation"), NO_API) \
	DECLARE_SERIALIZER(UUINavComponent)


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_40_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UUINavComponent(UUINavComponent&&); \
	UUINavComponent(const UUINavComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUINavComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUINavComponent); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUINavComponent) \
	NO_API virtual ~UUINavComponent();


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_37_PROLOG
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_40_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_40_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_40_CALLBACK_WRAPPERS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_40_INCLASS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h_40_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UINAVIGATION_API UClass* StaticClass<class UUINavComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavComponent_h


#define FOREACH_ENUM_EBUTTONSTYLE(op) \
	op(EButtonStyle::None) \
	op(EButtonStyle::Normal) \
	op(EButtonStyle::Hovered) \
	op(EButtonStyle::Pressed) 

enum class EButtonStyle : uint8;
template<> struct TIsUEnumClass<EButtonStyle> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<EButtonStyle>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
