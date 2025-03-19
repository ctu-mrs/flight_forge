// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Data/InputMode.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_InputMode_generated_h
#error "InputMode.generated.h already included, missing '#pragma once' in InputMode.h"
#endif
#define UINAVIGATION_InputMode_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputMode_h


#define FOREACH_ENUM_EINPUTMODE(op) \
	op(EInputMode::None) \
	op(EInputMode::GameUI) \
	op(EInputMode::Game) \
	op(EInputMode::UI) 

enum class EInputMode : uint8;
template<> struct TIsUEnumClass<EInputMode> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<EInputMode>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
