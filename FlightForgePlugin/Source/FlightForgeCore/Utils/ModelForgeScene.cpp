#include "ModelForgeScene.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "UObject/UnrealType.h"

namespace
{

UClass* LoaderClass()
{
	return FindObject<UClass>(nullptr, TEXT("/Script/ModelForgeSplatScene.ModelForgeSceneLoader"));
}

}  // namespace

/* ActiveFolder() //{ */

FString ModelForgeScene::ActiveFolder(UWorld* World)
{
	UClass* Class = LoaderClass();
	if (!World || !Class)
	{
		return {};
	}

	const FStrProperty* Folder   = FindFProperty<FStrProperty>(Class, TEXT("SceneFolder"));
	UFunction*          GetState = Class->FindFunctionByName(TEXT("GetState"));
	if (!Folder || !GetState)
	{
		return {};
	}

	// EModelForgeSceneState: Empty, Loading, Ready, Failed
	constexpr uint8 Loading = 1;
	constexpr uint8 Ready   = 2;

	for (TActorIterator<AActor> It(World, Class); It; ++It)
	{
		struct
		{
			uint8 ReturnValue = 0;
		} Params;
		It->ProcessEvent(GetState, &Params);

		if (Params.ReturnValue == Loading || Params.ReturnValue == Ready)
		{
			return Folder->GetPropertyValue_InContainer(*It);
		}
	}

	return {};
}

/* //} */

/* Load() //{ */

bool ModelForgeScene::Load(UWorld* World, const FString& Folder)
{
	UClass* Class = LoaderClass();
	if (!World || !Class)
	{
		UE_LOG(LogTemp, Error, TEXT("FlightForge: cannot load the ModelForge scene %s, is the ModelForgeSplat plugin enabled?"), *Folder);
		return false;
	}

	// -MFScene= gives every world a loader of its own before BeginPlay
	AActor* Loader = nullptr;
	for (TActorIterator<AActor> It(World, Class); It; ++It)
	{
		Loader = *It;
		break;
	}

	const FStrProperty* SceneFolder = FindFProperty<FStrProperty>(Class, TEXT("SceneFolder"));
	if (Loader && SceneFolder && SceneFolder->GetPropertyValue_InContainer(Loader) == Folder)
	{
		return true;
	}

	Loader               = Loader ? Loader : World->SpawnActor<AActor>(Class, FTransform::Identity);
	UFunction* LoadScene = Loader ? Loader->FindFunction(TEXT("LoadScene")) : nullptr;
	if (!LoadScene)
	{
		return false;
	}

	struct
	{
		FString Folder;
	} Params{Folder};
	Loader->ProcessEvent(LoadScene, &Params);

	UE_LOG(LogTemp, Log, TEXT("FlightForge: loading the ModelForge scene %s"), *Folder);
	return true;
}

/* //} */

/* PlacedStart() //{ */

bool ModelForgeScene::PlacedStart(UWorld* World, FVector& OutGround)
{
	UClass* Class = LoaderClass();
	if (!World || !Class)
	{
		return false;
	}

	UFunction* IsPlaced = Class->FindFunctionByName(TEXT("IsPlayerPlaced"));
	UFunction* GetStart = Class->FindFunctionByName(TEXT("GetPlayerStart"));
	if (!IsPlaced || !GetStart)
	{
		return false;
	}

	for (TActorIterator<AActor> It(World, Class); It; ++It)
	{
		struct
		{
			bool ReturnValue = false;
		} Placed;
		It->ProcessEvent(IsPlaced, &Placed);
		if (!Placed.ReturnValue)
		{
			continue;
		}

		struct
		{
			FTransform ReturnValue;
		} Start;
		It->ProcessEvent(GetStart, &Start);
		OutGround = Start.ReturnValue.GetLocation();
		return true;
	}

	return false;
}

/* //} */
