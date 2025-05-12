// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DronePawn.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Server/UedsGameModeServer.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "uedsGameModeBase.generated.h"

#if PLATFORM_WINDOWS
  #include "Microsoft/AllowMicrosoftPlatformTypes.h"
#endif

/**
 * 
 */
UCLASS()
class UEDS_API AuedsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable)
	int ForestDensityLevel = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable)
	int ForestHillyLevel = 3;

private:
	std::unique_ptr<UedsGameModeServer> Server;

	CameraCaptureModeEnum CameraCaptureMode = CameraCaptureModeEnum::CAPTURE_ON_DEMAND;

  TMap<int32, FString> DroneModelIdMap;

void InitializeModelIdMap()
{
    DroneModelIdMap.Empty(); 

    UE_LOG(LogTemp, Log, TEXT("Initializing Drone Model ID Map..."));

    if (!DronePawnClass)
    {
        UE_LOG(LogTemp, Error, TEXT("InitializeModelIdMap: DronePawnClass is not set in GameMode Defaults! Cannot map predefined models."));
        DroneModelIdMap.Add(0, TEXT("x500")); 
        return;
    }

    ADronePawn* DroneCDO = Cast<ADronePawn>(DronePawnClass->GetDefaultObject());
    if (!DroneCDO)
    {
        UE_LOG(LogTemp, Error, TEXT("InitializeModelIdMap: Could not get Drone Pawn CDO! Cannot map predefined models."));
        return;
    }

    const TArray<FramePropellersTransform>& PredefinedFrames = DroneCDO->GetPredefinedFrameTransforms(); 
    for (int32 i = 0; i < PredefinedFrames.Num(); ++i)                                                 
    {
      const FString& FrameName = PredefinedFrames[i].FrameName;                                  
      DroneModelIdMap.Add(i, FrameName);
      const int32 CurrentId = i; 
      UE_LOG(LogTemp, Log, TEXT("  Mapped ID %d -> Model Name '%s' (Predefined)"), CurrentId, *FrameName);
    }
	
	FString ConfigFilePath = FPaths::ProjectPluginsDir() + TEXT("flight_forge/FlightForgePlugin/Content/ExternalModels/MyDroneModels.ini");
    if (FPaths::FileExists(ConfigFilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("Loading additional model mappings from %s"), *ConfigFilePath);
        FConfigFile ModelConfigFile;
        ModelConfigFile.Read(ConfigFilePath);
        const FConfigSection* ExternalModelsSection = ModelConfigFile.FindSection(TEXT("ExternalModels"));
        if (ExternalModelsSection)
        {
            for (const auto& Pair : *ExternalModelsSection)
            {
                int32 ExternalId = FCString::Atoi(*Pair.Key.ToString()); 
                FString ExternalName = Pair.Value.GetValue();        
                if (ExternalId >= 0 && !ExternalName.IsEmpty()) 
                {
                    DroneModelIdMap.Add(ExternalId, ExternalName);
                     UE_LOG(LogTemp, Log, TEXT("  Mapped ID %d -> Model Name '%s' (From Config)"), ExternalId, *ExternalName);
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Optional model config file not found: %s"), *ConfigFilePath);
    }

    UE_LOG(LogTemp, Log, TEXT("Finished initializing Drone Model ID Map. Total Mappings: %d"), DroneModelIdMap.Num());
}
  

#if PLATFORM_WINDOWS
	std::unique_ptr<FWindowsCriticalSection> FPSCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
	std::unique_ptr<FPThreadsCriticalSection> FPSCriticalSection = std::make_unique<FPThreadsCriticalSection>();
#endif
	double FPS = 0; 

#if PLATFORM_WINDOWS
	std::unique_ptr<FWindowsCriticalSection> DronePawnsCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
	std::unique_ptr<FPThreadsCriticalSection> DronePawnsCriticalSection = std::make_unique<FPThreadsCriticalSection>();
#endif
	TMap<int, std::pair<ADronePawn*, APlayerController*>> DronePawns = TMap<int, std::pair<ADronePawn*, APlayerController*>>();

	AuedsGameModeBase(const FObjectInitializer& ObjectInitializer) : AGameModeBase(ObjectInitializer)
	{
		PrimaryActorTick.bCanEverTick = true;
		
		InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<AuedsGameModeBase>>>>();
		
		Server = std::make_unique<UedsGameModeServer>(*this, 8551);
	}

	// Must be used in order to tell UE that there will be more players - drones
	// TODO maybe cleaner solution?
	virtual void PostLogin(APlayerController* NewPlayer) override
	{
		//UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::PostLogin"));
	}

	virtual void BeginPlay() override
	{

    InitializeModelIdMap();
		UE_LOG(LogTemp, Warning, TEXT("Starting game mode server"));
		Server->Run();
		// UE_LOG(LogTemp, Warning, TEXT("Starting game mode server %s"), *GEngine->GetCurrentPlayWorld()->GetName());
		// if(GEngine->GetCurrentPlayWorld()->GetName().Equals("Forest"))
		// {
		// 	
		// }else
		// {
		// SwitchWorldLevel(Serializable::GameMode::WorldLevelEnum::FOREST);
		// }
		
		//FVector L = FVector::Zero();
		// for(int i = 0; i < 20; i++)
		// {
		// 	SpawnDroneAtLocation(L);
		// 	L.X += 200;
		// }
		//SpawnDrone();
		Super::BeginPlay();
	}

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override
	{
		Server->Stop();
	}

	virtual void Tick(float DeltaSeconds) override
	{
		std::shared_ptr<FInstruction<AuedsGameModeBase>> Instruction;
		while(InstructionQueue->Dequeue(Instruction))
		{
			//UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::Tick got Instruction"));
			Instruction->Function(*this);
			Instruction->Finished = true;
		}

		FPSCriticalSection->Lock();
		FPS = 1.0f / DeltaSeconds;
		FPSCriticalSection->Unlock();
	
		Super::Tick(DeltaSeconds);
	}

	int NextDronePort = 4000;
	
	int GetAvailableDronePort()
	{
		return NextDronePort++;
	}

public:
	void GetDronePorts(std::vector<int>& Ports)
	{
		TArray<int> Keys;
		
		DronePawnsCriticalSection->Lock();
		const auto PawnsCount = DronePawns.Num();
		DronePawns.GetKeys(Keys);
		DronePawnsCriticalSection->Unlock();

		Ports.resize(PawnsCount);
		std::copy_n(Keys.GetData(), PawnsCount, Ports.begin());
	}
	
	int SpawnDrone()
	{
		//UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone"));
		
		AActor* PlayerStart = FindPlayerStart(0, FString("UAV")); 
		ADronePawn* PlayerPawn = nullptr;
		auto PlayerController = SpawnPlayerController(ENetRole::ROLE_MAX, FString());

		// Realistic spawner
		// First Find spawn point by raycast DOWNWARDS 
		// if(UWorld* World = GetWorld())
		// {
		// 	FHitResult HitResult;
		// 	FVector Start = PlayerStart->GetTransform().GetLocation();
		// 	FVector End = Start + FVector::DownVector * 100000;
		// 	FVector SpawnOffset = 100*FVector::UpVector;
		// 	if(World->LineTraceSingleByChannel(HitResult, Start, End, ECC_MAX, FCollisionQueryParams::DefaultQueryParam))
		// 	{
		// 		UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone by raycast DOWN"));
		// 		DrawDebugSphere(World, HitResult.Location, 30, 10,FColor::Red, true, -1, 0, 3);
		// 		PlayerPawn = Cast<ADronePawn>(SpawnDefaultPawnAtTransform(PlayerController, FTransform(HitResult.Location+SpawnOffset)));
		// 	}
		// 	else if(World->LineTraceSingleByChannel(HitResult, Start, Start + FVector::UpVector * 100000, ECC_MAX, FCollisionQueryParams::DefaultQueryParam))
		// 	{
		// 		
		// 		UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone by raycast UP"));
		// 		DrawDebugSphere(World, HitResult.Location, 30, 10,FColor::Red, true, -1, 0, 3);
		// 		PlayerPawn = Cast<ADronePawn>(SpawnDefaultPawnAtTransform(PlayerController, FTransform(HitResult.Location+SpawnOffset)));
		// 	}
		// }
		
		if(PlayerPawn == nullptr)
		{
			PlayerPawn = Cast<ADronePawn>(SpawnDefaultPawnAtTransform(PlayerController, PlayerStart->GetTransform()));
			UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone at PlayerStart"));
		}

		const auto DronePort = GetAvailableDronePort();
		PlayerPawn->droneServer->SetPort(DronePort);
		PlayerPawn->SetCameraCaptureMode(this->CameraCaptureMode);
		PlayerPawn->StartServer();

		DronePawnsCriticalSection->Lock();
		DronePawns.Add(DronePort, std::make_pair(PlayerPawn, PlayerController));
		DronePawnsCriticalSection->Unlock();

		return PlayerPawn->droneServer->GetPort();
	}

UFUNCTION(BlueprintCallable)
int SpawnDroneAtLocation(FVector Location, int IdMesh)
{
    UE_LOG(LogTemp, Log, TEXT("AuedsGameModeBase::SpawnDroneAtLocation received request for ID: %d at location: %s"), IdMesh, *Location.ToString());

    FString ModelNameToLoad;
    const FString* FoundNamePtr = DroneModelIdMap.Find(IdMesh);

    if (FoundNamePtr != nullptr)
    {
        ModelNameToLoad = *FoundNamePtr;
        UE_LOG(LogTemp, Log, TEXT("Mapped ID %d to Model Name '%s'."), IdMesh, *ModelNameToLoad);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnDroneAtLocation: ID %d not found in DroneModelIdMap. Using default model."), IdMesh);
        const FString* DefaultNamePtr = DroneModelIdMap.Find(0); 
        if (DefaultNamePtr) {
            ModelNameToLoad = *DefaultNamePtr;
        } else {
             ModelNameToLoad = TEXT("x500");
             UE_LOG(LogTemp, Error, TEXT("SpawnDroneAtLocation: ID 0 fallback model name not found in map! Using hardcoded 'x500'."));
        }
         UE_LOG(LogTemp, Warning, TEXT("Falling back to model name '%s'."), *ModelNameToLoad);
    }


    ADronePawn* PlayerPawn = nullptr;
    APlayerController* PlayerController = SpawnPlayerController(ENetRole::ROLE_MAX, FString()); 


    if(PlayerPawn == nullptr)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = PlayerController;
        PlayerPawn = Cast<ADronePawn>(SpawnDefaultPawnAtTransform(PlayerController, FTransform(Location)));
        UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone at defined Location: %s"), *Location.ToString());
    }

    if (!PlayerPawn) 
    {
         UE_LOG(LogTemp, Error, TEXT("Failed to spawn DronePawn!"));
         if (PlayerController) PlayerController->Destroy();
         return -1; 
    }

    const auto DronePort = GetAvailableDronePort();
    PlayerPawn->droneServer->SetPort(DronePort);

	PlayerPawn->RootMeshComponent->bRenderCustomDepth = true;	
	PlayerPawn->RootMeshComponent->CustomDepthStencilValue = DronePort - 4000 + 200;
		
    PlayerPawn->SetCameraCaptureMode(this->CameraCaptureMode);

    if (!ModelNameToLoad.IsEmpty())
    {
        PlayerPawn->SetStaticMeshByName(ModelNameToLoad);
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("ModelNameToLoad was empty after lookup/fallback! Cannot set mesh."));
    }


    PlayerPawn->StartServer();
    PlayerPawn->Simulate_UE_Physics(3.0f);

    DronePawnsCriticalSection->Lock();
    DronePawns.Add(DronePort, std::make_pair(PlayerPawn, PlayerController));
    DronePawnsCriticalSection->Unlock();

    if(!bMutualDroneVisibilityEnabled_)
    {
        PlayerPawn->SetVisibilityOtherDrones(bMutualDroneVisibilityEnabled_);
        UpdateMutualVisibility();
    }

    return PlayerPawn->droneServer->GetPort();
}

	bool RemoveDrone(int Port)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::RemoveDrone"));
		
		DronePawnsCriticalSection->Lock();
		auto DronePair = DronePawns.Find(Port);
		if(DronePair)
		{
			DronePair->second->Destroy();
			DronePair->first->Destroy();
		}
		const bool Success = DronePawns.Remove(Port) > 0;
		DronePawnsCriticalSection->Unlock();

		return Success;
	}

	CameraCaptureModeEnum GetCaptureMode()
	{
		return CameraCaptureMode;
	}

	bool SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode)
	{
		CameraCaptureMode = CaptureMode;

		for(auto& a : DronePawns)
		{
			a.Value.first->SetCameraCaptureMode(CaptureMode);
		}

		return true;
	}

	float GetFPS()
	{
		float _FPS = 0;

		FPSCriticalSection->Lock();
		_FPS = FPS;
		FPSCriticalSection->Unlock();

		return _FPS;
	}

	void GetAllDronesLocation(std::vector<FVector>& Positions)
	{
		TArray<int> DronePorts;
		
		DronePawnsCriticalSection->Lock();
		DronePawns.GetKeys(DronePorts);
		for (const auto DronePort : DronePorts)
		{
			auto DronePawn = DronePawns.Find(DronePort)->first;
			Positions.push_back(DronePawn->GetActorLocation());
		}
		DronePawnsCriticalSection->Unlock();
	}

	bool SetGraphicsSettings(const int32 Level)
	{
		bool bResult = true;
		
		UGameUserSettings *GameUserSettings = GEngine->GetGameUserSettings();
		
		if (GameUserSettings != nullptr)
		{
			GameUserSettings->SetOverallScalabilityLevel(Level);
		
			GameUserSettings->ApplySettings(false);
		}
		else
		{
			bResult = false;
		}

		return bResult;
	
	}

	//UFUNCTION(BlueprintCallable)
	bool SwitchWorldLevel(const short& WorldLevelEnum)
	{
		FName NameOfWorld;

		switch (WorldLevelEnum)
		{
		case Serializable::GameMode::WorldLevelEnum::VALLEY:
			NameOfWorld = "Valley";
			break;
		case Serializable::GameMode::WorldLevelEnum::FOREST:
			NameOfWorld = "Forest";
			break;
		case Serializable::GameMode::WorldLevelEnum::INFINITE_FOREST:
			NameOfWorld = "InfinityForest";
			break;
		case Serializable::GameMode::WorldLevelEnum::WAREHOUSE:
			NameOfWorld = "Warehouse";
			break;
		case Serializable::GameMode::WorldLevelEnum::CAVE:
			NameOfWorld = "CaveTunnel";
			break;
		case Serializable::GameMode::WorldLevelEnum::ERDING_AIRBASE:
			NameOfWorld = "ErdingAirBase";
			break;
		case Serializable::GameMode::WorldLevelEnum::TEMESVAR:
			NameOfWorld = "Temesvar_annotated";
			break;
		case 7:
			NameOfWorld = "ElectricTowers";
			break;
		case 8:
			NameOfWorld = "Race_1";
			break;
		case 9:
			NameOfWorld = "Race_2";
			break;
	    case 10:
	      NameOfWorld = "IndustialWarehouse";
	      break;
	    case 11:
	      NameOfWorld = "ServiceTunnel";
	      break;
	    case 12:
	      NameOfWorld = "DeadSpruceForestBiome_Example_Daytime";
	      break;
		default:
			NameOfWorld = "InfiniteForest";
			break;
		}
		
		UGameplayStatics::OpenLevel(this, NameOfWorld);

		return true; 
	}

	FVector GetWorldOrigin()
	{
		AActor* PlayerStart = FindPlayerStart(0, FString("UAV"));
		return PlayerStart->GetActorLocation();
	}

	bool bMutualDroneVisibilityEnabled_ = true;

	void SetMutualVisibility(bool bMutualDroneVisibilityEnabled)
	{
		bMutualDroneVisibilityEnabled_ = bMutualDroneVisibilityEnabled;
	}

	void UpdateMutualVisibility()
	{
		for (auto DroneToUpdate : DronePawns)
		{
			TArray<AActor*> DronesToBeHidden;
			for (auto DronePawn : DronePawns)
			{
				if(DroneToUpdate != DronePawn)
				{
					DronesToBeHidden.Add(DronePawn.Value.first);
				}
			}
			DroneToUpdate.Value.first->SceneCaptureComponent2DRgb->HiddenActors.Empty();
			DroneToUpdate.Value.first->SceneCaptureComponent2DRgb->HiddenActors.Append(DronesToBeHidden);

			// UE_LOG(LogTemp, Error, TEXT("hidden actors count is %d"), DroneToUpdate.Value.first->SceneCaptureComponent2DRgb->HiddenActors.Num());
		}
	}
		
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetWeather(int TypeId);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetDaytime(int hour, int minute);

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<AuedsGameModeBase>>>> InstructionQueue;
	
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category=Classes)
	TSubclassOf<ADronePawn> DronePawnClass;
};
