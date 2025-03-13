// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ComponentActions/UINavComponentAction.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UUINavComponent;
#ifdef UINAVIGATION_UINavComponentAction_generated_h
#error "UINavComponentAction.generated.h already included, missing '#pragma once' in UINavComponentAction.h"
#endif
#define UINAVIGATION_UINavComponentAction_generated_h

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_14_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FComponentActions_Statics; \
	UINAVIGATION_API static class UScriptStruct* StaticStruct();


template<> UINAVIGATION_API UScriptStruct* StaticStruct<struct FComponentActions>();

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_36_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execExecuteAction);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_36_CALLBACK_WRAPPERS
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_36_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUUINavComponentAction(); \
	friend struct Z_Construct_UClass_UUINavComponentAction_Statics; \
public: \
	DECLARE_CLASS(UUINavComponentAction, UObject, COMPILED_IN_FLAGS(CLASS_Abstract), CASTCLASS_None, TEXT("/Script/UINavigation"), NO_API) \
	DECLARE_SERIALIZER(UUINavComponentAction)


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_36_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UUINavComponentAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UUINavComponentAction(UUINavComponentAction&&); \
	UUINavComponentAction(const UUINavComponentAction&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUINavComponentAction); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUINavComponentAction); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUINavComponentAction) \
	NO_API virtual ~UUINavComponentAction();


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_33_PROLOG
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_36_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_36_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_36_CALLBACK_WRAPPERS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_36_INCLASS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h_36_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UINAVIGATION_API UClass* StaticClass<class UUINavComponentAction>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_ComponentActions_UINavComponentAction_h


#define FOREACH_ENUM_ECOMPONENTACTION(op) \
	op(EComponentAction::OnClicked) \
	op(EComponentAction::OnPressed) \
	op(EComponentAction::OnReleased) \
	op(EComponentAction::OnNavigatedTo) \
	op(EComponentAction::OnNavigatedFrom) 

enum class EComponentAction : uint8;
template<> struct TIsUEnumClass<EComponentAction> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<EComponentAction>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
