#pragma once

#include "CoreMinimal.h"

/* FWorldCatalog //{ */

struct FWorldEntry
{
	FString Name;
	FString Package;
	FString DisplayName;
};

class FLIGHTFORGECORE_API FWorldCatalog
{
public:
	// the maps under /Game this build can open, sorted by package; built once, any thread
	static const TArray<FWorldEntry>& Get();

	// a package path, a list_worlds name, or a short name unique ignoring case
	static bool Resolve(const FString& Level, FString& OutPackage, FString& OutError);

	static TArray<FWorldEntry> FromPackages(TArray<FString> Packages);
	static bool                Resolve(const TArray<FWorldEntry>& Worlds, const FString& Level, FString& OutPackage, FString& OutError);
};

/* //} */
