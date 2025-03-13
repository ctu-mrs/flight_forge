// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Data/SelectorPosition.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_SelectorPosition_generated_h
#error "SelectorPosition.generated.h already included, missing '#pragma once' in SelectorPosition.h"
#endif
#define UINAVIGATION_SelectorPosition_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_SelectorPosition_h


#define FOREACH_ENUM_ESELECTORPOSITION(op) \
	op(ESelectorPosition::Position_Center) \
	op(ESelectorPosition::Position_Top) \
	op(ESelectorPosition::Position_Bottom) \
	op(ESelectorPosition::Position_Left) \
	op(ESelectorPosition::Position_Right) \
	op(ESelectorPosition::Position_Top_Right) \
	op(ESelectorPosition::Position_Top_Left) \
	op(ESelectorPosition::Position_Bottom_Right) \
	op(ESelectorPosition::Position_Bottom_Left) 

enum class ESelectorPosition : uint8;
template<> struct TIsUEnumClass<ESelectorPosition> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<ESelectorPosition>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
