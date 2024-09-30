// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeueds_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_ueds;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_ueds()
	{
		if (!Z_Registration_Info_UPackage__Script_ueds.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/ueds",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x550A0C4E,
				0xF19BA6FC,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_ueds.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_ueds.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_ueds(Z_Construct_UPackage__Script_ueds, TEXT("/Script/ueds"), Z_Registration_Info_UPackage__Script_ueds, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x550A0C4E, 0xF19BA6FC));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
