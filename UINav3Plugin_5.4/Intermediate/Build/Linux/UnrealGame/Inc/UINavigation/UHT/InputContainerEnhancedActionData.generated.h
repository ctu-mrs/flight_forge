// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Data/InputContainerEnhancedActionData.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_InputContainerEnhancedActionData_generated_h
#error "InputContainerEnhancedActionData.generated.h already included, missing '#pragma once' in InputContainerEnhancedActionData.h"
#endif
#define UINAVIGATION_InputContainerEnhancedActionData_generated_h

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputContainerEnhancedActionData_h_19_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FInputContainerEnhancedActionData_Statics; \
	UINAVIGATION_API static class UScriptStruct* StaticStruct();


template<> UINAVIGATION_API UScriptStruct* StaticStruct<struct FInputContainerEnhancedActionData>();

#define FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputContainerEnhancedActionData_h_42_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FInputContainerEnhancedActionDataArray_Statics; \
	UINAVIGATION_API static class UScriptStruct* StaticStruct();


template<> UINAVIGATION_API UScriptStruct* StaticStruct<struct FInputContainerEnhancedActionDataArray>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputContainerEnhancedActionData_h


#define FOREACH_ENUM_EINPUTAXIS(op) \
	op(EInputAxis::X) \
	op(EInputAxis::Y) \
	op(EInputAxis::Z) 

enum class EInputAxis : uint8;
template<> struct TIsUEnumClass<EInputAxis> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<EInputAxis>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
