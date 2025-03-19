// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Data/AxisType.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_AxisType_generated_h
#error "AxisType.generated.h already included, missing '#pragma once' in AxisType.h"
#endif
#define UINAVIGATION_AxisType_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_AxisType_h


#define FOREACH_ENUM_EAXISTYPE(op) \
	op(EAxisType::None) \
	op(EAxisType::Positive) \
	op(EAxisType::Negative) 

enum class EAxisType : uint8;
template<> struct TIsUEnumClass<EAxisType> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<EAxisType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
