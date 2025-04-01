// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Data/InputDisplayType.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_InputDisplayType_generated_h
#error "InputDisplayType.generated.h already included, missing '#pragma once' in InputDisplayType.h"
#endif
#define UINAVIGATION_InputDisplayType_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_InputDisplayType_h


#define FOREACH_ENUM_EINPUTDISPLAYTYPE(op) \
	op(EInputDisplayType::Icon) \
	op(EInputDisplayType::Text) \
	op(EInputDisplayType::Both) 

enum class EInputDisplayType : uint8;
template<> struct TIsUEnumClass<EInputDisplayType> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<EInputDisplayType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
