// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "UINavBlueprintFunctionLibrary.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class APlayerController;
class UInputAction;
class UPanelWidget;
class UPromptDataBinary;
class USoundClass;
class UUINavSettings;
class UUserWidget;
class UWidget;
enum class EInputRestriction : uint8;
struct FKey;
#ifdef UINAVIGATION_UINavBlueprintFunctionLibrary_generated_h
#error "UINavBlueprintFunctionLibrary.generated.h already included, missing '#pragma once' in UINavBlueprintFunctionLibrary.h"
#endif
#define UINAVIGATION_UINavBlueprintFunctionLibrary_generated_h

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execIsKeyInCategory); \
	DECLARE_FUNCTION(execIsVRKey); \
	DECLARE_FUNCTION(execGetUINavSettings); \
	DECLARE_FUNCTION(execGetFirstWidgetInUserWidget); \
	DECLARE_FUNCTION(execGetIndexInUniformGridWidget); \
	DECLARE_FUNCTION(execGetParentPanelWidget); \
	DECLARE_FUNCTION(execGetIndexInPanelWidget); \
	DECLARE_FUNCTION(execFindWidgetOfClassesInWidget); \
	DECLARE_FUNCTION(execGetUniformGridChild); \
	DECLARE_FUNCTION(execGetPanelWidgetChild); \
	DECLARE_FUNCTION(execCreateBinaryPromptData); \
	DECLARE_FUNCTION(execIsUINavInputAction); \
	DECLARE_FUNCTION(execIsGamepadConnected); \
	DECLARE_FUNCTION(execGetRawTextFromRichText); \
	DECLARE_FUNCTION(execApplyStyleRowToText); \
	DECLARE_FUNCTION(execRespectsRestriction); \
	DECLARE_FUNCTION(execResetInputSettings); \
	DECLARE_FUNCTION(execGetPostProcessSettings); \
	DECLARE_FUNCTION(execSetPostProcessSettings); \
	DECLARE_FUNCTION(execGetSoundClassVolume); \
	DECLARE_FUNCTION(execSetSoundClassVolume);


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_22_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUUINavBlueprintFunctionLibrary(); \
	friend struct Z_Construct_UClass_UUINavBlueprintFunctionLibrary_Statics; \
public: \
	DECLARE_CLASS(UUINavBlueprintFunctionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/UINavigation"), NO_API) \
	DECLARE_SERIALIZER(UUINavBlueprintFunctionLibrary)


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_22_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UUINavBlueprintFunctionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UUINavBlueprintFunctionLibrary(UUINavBlueprintFunctionLibrary&&); \
	UUINavBlueprintFunctionLibrary(const UUINavBlueprintFunctionLibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUINavBlueprintFunctionLibrary); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUINavBlueprintFunctionLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUINavBlueprintFunctionLibrary) \
	NO_API virtual ~UUINavBlueprintFunctionLibrary();


#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_19_PROLOG
#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_22_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_22_INCLASS_NO_PURE_DECLS \
	FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h_22_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UINAVIGATION_API UClass* StaticClass<class UUINavBlueprintFunctionLibrary>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_UINavBlueprintFunctionLibrary_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
