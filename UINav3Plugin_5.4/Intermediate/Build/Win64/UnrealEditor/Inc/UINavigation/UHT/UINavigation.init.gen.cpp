// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUINavigation_init() {}
	UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature();
	UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_OnClickedEvent__DelegateSignature();
	UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_OnNavigatedFromEvent__DelegateSignature();
	UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_OnNavigatedToEvent__DelegateSignature();
	UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_OnPressedEvent__DelegateSignature();
	UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_OnReleasedEvent__DelegateSignature();
	UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_OnValueChangedEvent__DelegateSignature();
	UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature();
	UINAVIGATION_API UFunction* Z_Construct_UDelegateFunction_UINavigation_UpdateInputIconsDelegate__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_UINavigation;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_UINavigation()
	{
		if (!Z_Registration_Info_UPackage__Script_UINavigation.OuterSingleton)
		{
			static UObject* (*const SingletonFuncArray[])() = {
				(UObject* (*)())Z_Construct_UDelegateFunction_UINavigation_InputTypeChangedDelegate__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_UINavigation_OnClickedEvent__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_UINavigation_OnNavigatedFromEvent__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_UINavigation_OnNavigatedToEvent__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_UINavigation_OnPressedEvent__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_UINavigation_OnReleasedEvent__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_UINavigation_OnValueChangedEvent__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_UINavigation_PromptWidgetDecided__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_UINavigation_UpdateInputIconsDelegate__DelegateSignature,
			};
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/UINavigation",
				SingletonFuncArray,
				UE_ARRAY_COUNT(SingletonFuncArray),
				PKG_CompiledIn | 0x00000000,
				0x91F4067B,
				0x3CE44621,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_UINavigation.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_UINavigation.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_UINavigation(Z_Construct_UPackage__Script_UINavigation, TEXT("/Script/UINavigation"), Z_Registration_Info_UPackage__Script_UINavigation, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x91F4067B, 0x3CE44621));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
