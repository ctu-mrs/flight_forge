// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Data/TargetColumn.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_TargetColumn_generated_h
#error "TargetColumn.generated.h already included, missing '#pragma once' in TargetColumn.h"
#endif
#define UINAVIGATION_TargetColumn_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_TargetColumn_h


#define FOREACH_ENUM_ETARGETCOLUMN(op) \
	op(ETargetColumn::Left) \
	op(ETargetColumn::Middle) \
	op(ETargetColumn::Right) 

enum class ETargetColumn : uint8;
template<> struct TIsUEnumClass<ETargetColumn> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<ETargetColumn>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
