// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DronePawn.h"
#include "uedsGameInstance.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Server/UedsGameModeServer.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "uedsGameModeBase.generated.h"

#if PLATFORM_WINDOWS
  #include "Microsoft/AllowMicrosoftPlatformTypes.h"
#endif


//Drone pooling for faster spawning
struct FPooledDrone
{
	APlayerController* Controller;
	ADronePawn* Pawn;
};

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
	int DronePoolSize = 2;
	TArray<FPooledDrone> DronePool;
	
	CameraCaptureModeEnum CameraCaptureMode = CameraCaptureModeEnum::CAPTURE_ALL_FRAMES;

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
	}

	// Must be used in order to tell UE that there will be more players - drones
	// TODO maybe cleaner solution?
	virtual void PostLogin(APlayerController* NewPlayer) override
	{
		//UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::PostLogin"));
	}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		UuedsGameInstance* MyGameInstance = Cast<UuedsGameInstance>(GetGameInstance());
		if (MyGameInstance && MyGameInstance->Server)
		{
			MyGameInstance->Server->SetCurrentGameMode(this);
			UE_LOG(LogTemp, Log, TEXT("GameMode connected to server!"));
		}
		
		FString NextMapNameStr = UGameplayStatics::ParseOption(OptionsString, TEXT("NextMapName"));
		FString NextMapPathStr = UGameplayStatics::ParseOption(OptionsString, TEXT("NextMapPath"));
		if (!NextMapNameStr.IsEmpty() && !NextMapPathStr.IsEmpty())
		{
			StartAsyncLoadingTargetMap(NextMapNameStr, NextMapPathStr);
        
			UE_LOG(LogTemp, Warning, TEXT("Loading screen, switching to: %s"), *NextMapPathStr);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Starting game mode server (Normal Map)"));
			InitializeDronePool();
		}
		
		// Server->Run();
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
		// Super::BeginPlay();
	}

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override
	{
		UuedsGameInstance* MyGameInstance = Cast<UuedsGameInstance>(GetGameInstance());
		if (MyGameInstance && MyGameInstance->Server)
		{
			MyGameInstance->Server->SetCurrentGameMode(nullptr);
		}

		Super::EndPlay(EndPlayReason);
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

	void InitializeDronePool()
	{
		for (int i = 0; i < DronePoolSize; i++)
		{
			SpawnDroneToPool();
		}
	}

	void SpawnDroneToPool()
	{
		FTransform HideTransform(FRotator::ZeroRotator, FVector(0, 0, -100000));
		FPooledDrone NewDrone;
			
		NewDrone.Controller = SpawnPlayerController(ENetRole::ROLE_MAX, FString());
			
		NewDrone.Pawn = Cast<ADronePawn>(SpawnDefaultPawnAtTransform(NewDrone.Controller, HideTransform));

		if (NewDrone.Pawn)
		{
			NewDrone.Pawn->SetActorHiddenInGame(true);
			NewDrone.Pawn->SetActorEnableCollision(false);
			NewDrone.Pawn->SetActorTickEnabled(false);
            
			DronePool.Add(NewDrone);
		}
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
		
		UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone"));

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
		UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDroneAtLocation at location: %lf, %lf, %lf [mesh %d]"), Location.X, Location.Y, Location.Z, IdMesh);
		
		// ADronePawn* PlayerPawn = nullptr;
		// auto PlayerController = SpawnPlayerController(ENetRole::ROLE_MAX, FString());

		// Realistic spawner
		// First Find spawn point by raycast DOWNWARDS
		
		// if(UWorld* World = GetWorld())
		// {
		// 	FHitResult HitResult;
		// 	FVector Start = Location;
		// 	FVector End = Start + FVector::DownVector * 100000;
		// 	FVector SpawnOffset = 300*FVector::UpVector;
		// 	if(World->LineTraceSingleByChannel(HitResult, Start, End, ECC_MAX, FCollisionQueryParams::DefaultQueryParam))
		// 	{
		// 		UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone by raycast DOWN"));
		// 		DrawDebugSphere(World, HitResult.Location, 10, 10,FColor::Red, true, -1, 0, 3);
		// 		PlayerPawn = Cast<ADronePawn>(SpawnDefaultPawnAtTransform(PlayerController, FTransform(HitResult.Location+SpawnOffset)));
		// 	}
		// 	else if(World->LineTraceSingleByChannel(HitResult, Start, Start + FVector::UpVector * 100000, ECC_MAX, FCollisionQueryParams::DefaultQueryParam))
		// 	{
		// 		
		// 		UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone by raycast UP"));
		// 		DrawDebugSphere(World, HitResult.Location, 10, 10,FColor::Red, true, -1, 0, 3);
		// 		PlayerPawn = Cast<ADronePawn>(SpawnDefaultPawnAtTransform(PlayerController, FTransform(HitResult.Location+SpawnOffset)));
		// 	}
		// }

		FPooledDrone ReadyDrone;
		if (DronePool.Num() > 0)
		{
			ReadyDrone = DronePool.Pop();
			
			ReadyDrone.Pawn->SetActorLocation(Location);
			
			ReadyDrone.Pawn->SetActorHiddenInGame(false);
			ReadyDrone.Pawn->SetActorEnableCollision(true);
			ReadyDrone.Pawn->SetActorTickEnabled(true);
        
			UE_LOG(LogTemp, Log, TEXT("Drone taken from a pool"));

			//new drone to pool
			auto Instruction = std::make_shared<FInstruction<AuedsGameModeBase>>();
			Instruction->Function = [](AuedsGameModeBase& _GameMode)
			{
				_GameMode.SpawnDroneToPool();
			};
			InstructionQueue->Enqueue(Instruction);
			
		}
		else
		{
			ReadyDrone.Controller = SpawnPlayerController(ENetRole::ROLE_MAX, FString());
			ReadyDrone.Pawn = Cast<ADronePawn>(SpawnDefaultPawnAtTransform(ReadyDrone.Controller, FTransform(Location)));
			UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone at defined Location"));
		}
		
		const auto DronePort = GetAvailableDronePort();
		ReadyDrone.Pawn->droneServer->SetPort(DronePort);
		ReadyDrone.Pawn->SetCameraCaptureMode(this->CameraCaptureMode);
		ReadyDrone.Pawn->StartServer();
		ReadyDrone.Pawn->SetStaticMesh(IdMesh);
		ReadyDrone.Pawn->Simulate_UE_Physics(3.0f);
		
		DronePawnsCriticalSection->Lock();
		DronePawns.Add(DronePort, std::make_pair(ReadyDrone.Pawn, ReadyDrone.Controller));
		DronePawnsCriticalSection->Unlock();

		if(!bMutualDroneVisibilityEnabled_)
		{
			ReadyDrone.Pawn->SetVisibilityOtherDrones(bMutualDroneVisibilityEnabled_);
			UpdateMutualVisibility();
		}

		return ReadyDrone.Pawn->droneServer->GetPort();
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
		FString PackagePath;
		
		switch (WorldLevelEnum)
		{
		case Serializable::GameMode::WorldLevelEnum::VALLEY:
			NameOfWorld = "Valley";
			PackagePath = "/Game/Worlds/Valley/Maps/Valley";
			break;
		case Serializable::GameMode::WorldLevelEnum::FOREST:
			NameOfWorld = "Forest";
			PackagePath = "/Game/Worlds/Forest/Maps/Forest";
			break;
		case Serializable::GameMode::WorldLevelEnum::INFINITE_FOREST:
			NameOfWorld = "InfinityForest";
			PackagePath = "/Game/Worlds/Forest/Maps/InfinityForest";
			break;
		case Serializable::GameMode::WorldLevelEnum::WAREHOUSE:
			NameOfWorld = "Warehouse";
			PackagePath = "/Game/Worlds/Warehouse/Maps/Warehouse";
			break;
		case Serializable::GameMode::WorldLevelEnum::CAVE:
			NameOfWorld = "CaveTunnel";
			PackagePath = "/Game/Worlds/Cave/Maps/CaveTunnel";
			break;
		case Serializable::GameMode::WorldLevelEnum::ERDING_AIRBASE:
			NameOfWorld = "ErdingAirBase";
			PackagePath = "/Game/Worlds/ErdingAirBase/Maps/ErdingAirBase";
			break;
		case Serializable::GameMode::WorldLevelEnum::TEMESVAR:
			NameOfWorld = "Temesvar_annotated";
			PackagePath = "/Game/Worlds/Temesvar/Maps/Temesvar_annotated";
			break;
		case 7:
			NameOfWorld = "ElectricTowers";
			PackagePath = "/Game/Worlds/ElectricTowers/Maps/ElectricTowers";
			break;
		case 8:
			NameOfWorld = "Race_1";
			PackagePath = "/Game/Worlds/Warehouse/Maps/Race_1";
			break;
		case 9:
			NameOfWorld = "Race_2";
			PackagePath = "/Game/Worlds/Warehouse/Maps/Race_2";
			break;
	    case 10:
			NameOfWorld = "IndustialWarehouse";
			PackagePath = "/Game/Worlds/IndustialWarehouse/Maps/IndustialWarehouse";
	      break;
	    case 11:
			NameOfWorld = "ServiceTunnel";
			PackagePath = "/Game/Worlds/ServiceTunnel/Maps/ServiceTunnel";
	      break;
	    case 12:
			NameOfWorld = "DeadSpruceForestBiome_Example_Daytime";
			PackagePath = "/Game/MWDeadSpruceForest/Maps/DeadSpruceForestBiome_Example_Daytime";
	      break;
		default:
			// NameOfWorld = "InfiniteForest"; could not find level named InfiniteForest
			NameOfWorld = "InfinityForest";
			PackagePath = "/Game/Worlds/Forest/Maps/InfinityForest";
			break;
		}
		
		FString Options = FString::Printf(TEXT("?game=/Script/ueds.uedsGameModeBase?NextMapName=%s?NextMapPath=%s"), 
		*NameOfWorld.ToString(), 
		*PackagePath);

		//opens empty level, might switch to loading screen
		UE_LOG(LogTemp, Warning, TEXT("OpenLevel: Empty"));
		UGameplayStatics::OpenLevel(this, "/Engine/Maps/Entry", true, Options);
		
		return true; 
	}

	void StartAsyncLoadingTargetMap(const FString& NextMapNameStr, const FString& NextMapPathStr)
	{
		FName TargetMapName = FName(*NextMapNameStr);
		
		FLoadPackageAsyncDelegate CompletionDelegate = FLoadPackageAsyncDelegate::CreateUObject(
			this, 
			&AuedsGameModeBase::OnMapPackageLoaded, 
			TargetMapName
		);
		
		LoadPackageAsync(NextMapPathStr, CompletionDelegate, 0, PKG_ContainsMap);
	}
	

	void OnMapPackageLoaded(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result, FName MapNameToOpen)
	{
		if (Result == EAsyncLoadingResult::Succeeded)
		{
			UE_LOG(LogTemp, Log, TEXT("Target Map %s is fully in RAM. Executing fast switch."), *MapNameToOpen.ToString());
			
			UGameplayStatics::OpenLevel(this, MapNameToOpen);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load map package to RAM: %s"), *PackageName.ToString());
		}
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
			// DroneToUpdate.Value.first->SceneCaptureComponent2DRgb->HiddenActors.Empty();
			// DroneToUpdate.Value.first->SceneCaptureComponent2DRgb->HiddenActors.Append(DronesToBeHidden);
			DroneToUpdate.Value.first->UpdateCameraSensorsMutualVisibility(DronesToBeHidden);
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
