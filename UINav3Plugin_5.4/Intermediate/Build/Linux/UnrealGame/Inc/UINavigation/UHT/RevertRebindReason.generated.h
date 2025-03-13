// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Data/RevertRebindReason.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_RevertRebindReason_generated_h
#error "RevertRebindReason.generated.h already included, missing '#pragma once' in RevertRebindReason.h"
#endif
#define UINAVIGATION_RevertRebindReason_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_RevertRebindReason_h


#define FOREACH_ENUM_EREVERTREBINDREASON(op) \
	op(ERevertRebindReason::None) \
	op(ERevertRebindReason::RestrictionMismatch) \
	op(ERevertRebindReason::NonWhitelistedKey) \
	op(ERevertRebindReason::BlacklistedKey) \
	op(ERevertRebindReason::UsedBySameInputGroup) \
	op(ERevertRebindReason::UsedBySameInput) \
	op(ERevertRebindReason::SwapRejected) 

enum class ERevertRebindReason : uint8;
template<> struct TIsUEnumClass<ERevertRebindReason> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<ERevertRebindReason>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
