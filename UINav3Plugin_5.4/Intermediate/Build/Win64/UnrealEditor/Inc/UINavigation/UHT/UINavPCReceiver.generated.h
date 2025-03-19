// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "UINavPCReceiver.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UUINavWidget;
enum class EInputType : uint8;
enum class EUINavigation : uint8;
#ifdef UINAVIGATION_UINavPCReceiver_generated_h
#error "UINavPCReceiver.generated.h already included, missing '#pragma once' in UINavPCReceiver.h"
#endif
#define UINAVIGATION_UINavPCReceiver_generated_h

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOnPrevious); \
	DECLARE_FUNCTION(execOnNext); \
	DECLARE_FUNCTION(execOnReturn); \
	DECLARE_FUNCTION(execOnSelect); \
	DECLARE_FUNCTION(execOnNavigated); \
	DECLARE_FUNCTION(execOnActiveWidgetChanged); \
	DECLARE_FUNCTION(execOnControllerConnectionChanged); \
	DECLARE_FUNCTION(execOnThumbstickCursorInput); \
	DECLARE_FUNCTION(execOnInputChanged); \
	DECLARE_FUNCTION(execOnRootWidgetRemoved); \
	DECLARE_FUNCTION(execOnRootWidgetAdded);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_CALLBACK_WRAPPERS
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	UINAVIGATION_API UUINavPCReceiver(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UUINavPCReceiver(UUINavPCReceiver&&); \
	UUINavPCReceiver(const UUINavPCReceiver&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(UINAVIGATION_API, UUINavPCReceiver); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUINavPCReceiver); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUINavPCReceiver) \
	UINAVIGATION_API virtual ~UUINavPCReceiver();


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_GENERATED_UINTERFACE_BODY() \
private: \
	static void StaticRegisterNativesUUINavPCReceiver(); \
	friend struct Z_Construct_UClass_UUINavPCReceiver_Statics; \
public: \
	DECLARE_CLASS(UUINavPCReceiver, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), CASTCLASS_None, TEXT("/Script/UINavigation"), UINAVIGATION_API) \
	DECLARE_SERIALIZER(UUINavPCReceiver)


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_GENERATED_UINTERFACE_BODY() \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~IUINavPCReceiver() {} \
public: \
	typedef UUINavPCReceiver UClassType; \
	typedef IUINavPCReceiver ThisClass; \
	static void Execute_OnActiveWidgetChanged(UObject* O, UUINavWidget* OldActiveWidget, UUINavWidget* NewActiveWidget); \
	static void Execute_OnControllerConnectionChanged(UObject* O, bool bConnected, int32 UserId, int32 UserIndex); \
	static void Execute_OnInputChanged(UObject* O, EInputType From, EInputType To); \
	static void Execute_OnNavigated(UObject* O, EUINavigation NavigationDirection); \
	static void Execute_OnNext(UObject* O); \
	static void Execute_OnPrevious(UObject* O); \
	static void Execute_OnReturn(UObject* O); \
	static void Execute_OnRootWidgetAdded(UObject* O); \
	static void Execute_OnRootWidgetRemoved(UObject* O); \
	static void Execute_OnSelect(UObject* O); \
	static void Execute_OnThumbstickCursorInput(UObject* O, FVector2D const& ThumbstickDelta); \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_12_PROLOG
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_23_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_CALLBACK_WRAPPERS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h_15_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UINAVIGATION_API UClass* StaticClass<class UUINavPCReceiver>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavPCReceiver_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
