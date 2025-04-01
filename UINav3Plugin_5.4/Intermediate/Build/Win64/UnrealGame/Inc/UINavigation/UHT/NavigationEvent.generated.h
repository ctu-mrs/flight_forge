// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Data/NavigationEvent.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ReflectedTypeAccessors.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UINAVIGATION_NavigationEvent_generated_h
#error "NavigationEvent.generated.h already included, missing '#pragma once' in NavigationEvent.h"
#endif
#define UINAVIGATION_NavigationEvent_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_janhr_Documents_Unreal_Projects_mrs_flight_forge_Plugins_flight_forge_UINav3Plugin_5_4_Source_UINavigation_Public_Data_NavigationEvent_h


#define FOREACH_ENUM_ENAVIGATIONEVENT(op) \
	op(ENavigationEvent::OnSelect) \
	op(ENavigationEvent::OnNavigate) 

enum class ENavigationEvent : uint8;
template<> struct TIsUEnumClass<ENavigationEvent> { enum { Value = true }; };
template<> UINAVIGATION_API UEnum* StaticEnum<ENavigationEvent>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
