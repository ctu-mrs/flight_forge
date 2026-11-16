#include "WorldCatalog.h"

#include "AssetRegistry/IAssetRegistry.h"
#include "Engine/World.h"
#include "HAL/FileManager.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"

#include <mutex>

/* FindMapPackages() //{ */

namespace
{

TArray<FString> FindMapPackages()
{
	TArray<FString> Packages;

	if (FPlatformProperties::RequiresCookedData())
	{
		FARFilter Filter;
		Filter.ClassPaths.Add(UWorld::StaticClass()->GetClassPathName());
		Filter.PackagePaths.Add(TEXT("/Game"));
		Filter.bRecursivePaths = true;

		TArray<FAssetData> Assets;
		IAssetRegistry::GetChecked().GetAssets(Filter, Assets);

		// the cooked registry also lists maps of paks that are not mounted
		for (const FAssetData& Asset : Assets)
		{
			const FString Package = Asset.PackageName.ToString();
			if (FPackageName::DoesPackageExist(Package))
			{
				Packages.AddUnique(Package);
			}
		}
		return Packages;
	}

	TArray<FString> Files;
	IFileManager::Get().FindFilesRecursive(Files, *FPaths::ProjectContentDir(), *(TEXT("*") + FPackageName::GetMapPackageExtension()), true, false);
	for (const FString& File : Files)
	{
		FString Package;
		if (!File.Contains(TEXT("__External")) && FPackageName::TryConvertFilenameToLongPackageName(File, Package))
		{
			Packages.AddUnique(Package);
		}
	}
	return Packages;
}

}  // namespace

/* //} */

/* FromPackages() //{ */

TArray<FWorldEntry> FWorldCatalog::FromPackages(TArray<FString> Packages)
{
	Packages.Sort([](const FString& A, const FString& B) { return A.Compare(B, ESearchCase::IgnoreCase) < 0; });

	TArray<FWorldEntry> Worlds;
	for (const FString& Package : Packages)
	{
		const FString Short  = FPackageName::GetShortName(Package);
		const bool    Shared = Packages.ContainsByPredicate(
		            [&](const FString& Other) { return Other != Package && FPackageName::GetShortName(Other).Equals(Short, ESearchCase::CaseSensitive); });

		Worlds.Add(FWorldEntry{Shared ? Package : Short, Package, FName::NameToDisplayString(Short, false)});
	}
	return Worlds;
}

/* //} */

/* Get() //{ */

const TArray<FWorldEntry>& FWorldCatalog::Get()
{
	static std::once_flag      Once;
	static TArray<FWorldEntry> Worlds;
	std::call_once(Once, [] {
		Worlds = FromPackages(FindMapPackages());
		UE_LOG(LogTemp, Log, TEXT("FlightForge: %d maps can be opened by switch_world"), Worlds.Num());
	});
	return Worlds;
}

/* //} */

/* Resolve() //{ */

bool FWorldCatalog::Resolve(const FString& Level, FString& OutPackage, FString& OutError)
{
	return Resolve(Get(), Level, OutPackage, OutError);
}

bool FWorldCatalog::Resolve(const TArray<FWorldEntry>& Worlds, const FString& Level, FString& OutPackage, FString& OutError)
{
	FString Wanted = Level.TrimStartAndEnd();
	if (Wanted.IsEmpty())
	{
		OutError = TEXT("switch_world needs a level");
		return false;
	}
	if (Wanted.StartsWith(TEXT("/")))
	{
		Wanted = FPackageName::ObjectPathToPackageName(Wanted);
		for (const FWorldEntry& World : Worlds)
		{
			if (World.Package.Equals(Wanted, ESearchCase::IgnoreCase))
			{
				OutPackage = World.Package;
				return true;
			}
		}
		OutError = FString::Printf(TEXT("no map %s in this build; list_worlds names the %d there are"), *Wanted, Worlds.Num());
		return false;
	}

	for (const ESearchCase::Type Case : {ESearchCase::CaseSensitive, ESearchCase::IgnoreCase})
	{
		TArray<FString> Matches;
		for (const FWorldEntry& World : Worlds)
		{
			if (FPackageName::GetShortName(World.Package).Equals(Wanted, Case))
			{
				Matches.Add(World.Package);
			}
		}

		if (Matches.Num() == 1)
		{
			OutPackage = Matches[0];
			return true;
		}
		if (Matches.Num() > 1)
		{
			OutError = FString::Printf(TEXT("world \"%s\" is ambiguous, pass one of %s"), *Wanted, *FString::Join(Matches, TEXT(", ")));
			return false;
		}
	}

	OutError = FString::Printf(TEXT("unknown world \"%s\"; list_worlds names the %d maps in this build"), *Wanted, Worlds.Num());
	return false;
}

/* //} */
