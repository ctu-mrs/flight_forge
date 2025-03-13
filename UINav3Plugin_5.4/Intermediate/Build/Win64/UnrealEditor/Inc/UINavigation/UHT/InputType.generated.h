// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Data/InputType.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_InputType_generated_h
#error "InputType.generated.h already included, missing '#pragma once' in InputType.h"
#endif
#define UINAVIGATION_InputType_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputType_h


#define FOREACH_ENUM_EINPUTTYPE(op) \
	op(EInputType::None) \
	op(EInputType::Keyboard) \
	op(EInputType::Mouse) \
	op(EInputType::Gamepad) 

enum class EInputType : uint8;
template<> struct TIsUEnumClass<EInputType> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<EInputType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
