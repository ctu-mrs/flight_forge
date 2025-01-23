// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DronePawn.h"
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
		UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone at location: %lf, %lf, %lf [mesh %d]"), Location.X, Location.Y, Location.Z, IdMesh);
		
		ADronePawn* PlayerPawn = nullptr;
		auto PlayerController = SpawnPlayerController(ENetRole::ROLE_MAX, FString());

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
		
		if(PlayerPawn == nullptr)
		{
			PlayerPawn = Cast<ADronePawn>(SpawnDefaultPawnAtTransform(PlayerController, FTransform(Location)));
			UE_LOG(LogTemp, Warning, TEXT("AuedsGameModeBase::SpawnDrone at defined Location"));
		}
		
		const auto DronePort = GetAvailableDronePort();
		PlayerPawn->droneServer->SetPort(DronePort);
		PlayerPawn->SetCameraCaptureMode(this->CameraCaptureMode);
		PlayerPawn->StartServer();
		PlayerPawn->SetStaticMesh(IdMesh);
		PlayerPawn->Simulate_UE_Physics(3.0f);
		
		DronePawnsCriticalSection->Lock();
		DronePawns.Add(DronePort, std::make_pair(PlayerPawn, PlayerController));
		DronePawnsCriticalSection->Unlock();

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
			NameOfWorld = "EletricTowers";
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
		case 13:
			NameOfWorld = "Race_3";
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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetWeather(int TypeId);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool SetDaytime(int hour, int minute);

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<AuedsGameModeBase>>>> InstructionQueue;
	
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category=Classes)
	TSubclassOf<ADronePawn> DronePawnClass;
};
