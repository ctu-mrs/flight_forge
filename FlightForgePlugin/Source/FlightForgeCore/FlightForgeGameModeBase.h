// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FlightForgeGameInstance.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/Engine.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameUserSettings.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "LandscapeProxy.h"
#include "PhysicsEngine/BodySetup.h"
#include "Misc/PackageName.h"
#include "UObject/ConstructorHelpers.h"

#include "RobotPawn.h"
#include "SimClock.h"
#include "Server/SimServer.h"
#include "HAL/FileManager.h"
#include "Utils/ObjectLibrary.h"
#include "Utils/SpawnConfig.h"
#include "Utils/WorldObjectSpawner.h"
#include "Utils/WireFrame.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "RenderingThread.h"
#include "Misc/App.h"
#include "HAL/IConsoleManager.h"
#include <string>

#include "FlightForgeGameModeBase.generated.h"


#if PLATFORM_WINDOWS
  #include "Microsoft/AllowMicrosoftPlatformTypes.h"
#endif


//Drone pooling for faster spawning
struct FPooledDrone
{
	APlayerController* Controller;
	ARobotPawn* Pawn;
};

UCLASS()
class FLIGHTFORGECORE_API AFlightForgeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlightForge|Forest")
	int ForestDensityLevel = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlightForge|Forest")
	int ForestHillyLevel = 3;

private:
	int DronePoolSize = 2;
	TArray<FPooledDrone> DronePool;
	
	CameraCaptureModeEnum CameraCaptureMode = CameraCaptureModeEnum::CAPTURE_ALL_FRAMES;

	std::unique_ptr<FCriticalSection> FPSCriticalSection = std::make_unique<FCriticalSection>();
	double FPS = 0;
	double WallFPS = 0;

	/* clock control //{ */

	// a step unpauses for a budget of ticks; the paused frame after the last one drains the sensors
	std::atomic<bool>   bLockstep{false};
	std::atomic<int32>  StepBudget{0};
	std::atomic<uint64> LockstepTicks{0};
	std::atomic<bool>   bStepFinishing{false};
	std::atomic<bool>   bStepDone{true};

	double FixedDtMax    = 0.0;
	double AdaptiveMinDt = 0.0;

	//}

	std::unique_ptr<FCriticalSection> RobotPawnsCriticalSection = std::make_unique<FCriticalSection>();

	TMap<int, std::pair<ARobotPawn*, APlayerController*>> RobotPawns = TMap<int, std::pair<ARobotPawn*, APlayerController*>>();

	UPROPERTY()
	TObjectPtr<UWorldObjectSpawner> ObjectSpawner;

	AFlightForgeGameModeBase(const FObjectInitializer& ObjectInitializer) : AGameModeBase(ObjectInitializer)
	{
		PrimaryActorTick.bCanEverTick = true;
		PrimaryActorTick.bTickEvenWhenPaused = true;  // lockstep parks the world paused

		// Every spawn path casts the default pawn to ARobotPawn; without this the class
		// stays ADefaultPawn and each cast yields null. Blueprint subclasses override it.
		static ConstructorHelpers::FClassFinder<APawn> RobotPawnFinder(TEXT("/FlightForgePlugin/Blueprints/BP_DronePawn"));
		if (RobotPawnFinder.Succeeded())
		{
			DefaultPawnClass = RobotPawnFinder.Class;
		}

		InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<AFlightForgeGameModeBase>>>>();
	}

	// Must be used in order to tell UE that there will be more players - drones
	// TODO maybe cleaner solution?
	virtual void PostLogin(APlayerController* NewPlayer) override
	{
		//UE_LOG(LogTemp, Warning, TEXT("AFlightForgeGameModeBase::PostLogin"));
	}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		LoadStartupLibraries();

		// scene captures keep their history between grabs, so temporal AA costs nothing; without any AA method every sensor image aliases
		if (IConsoleVariable* AntiAliasing = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod")))
		{
			if (AntiAliasing->GetInt() == 0)
			{
				AntiAliasing->Set(2, ECVF_SetByConsole);
				UE_LOG(LogTemp, Warning, TEXT("FlightForge: the project has no anti-aliasing method, enabling TAA for the sensors"));
			}
		}

		// A Blueprint that fills in RobotPawnClass wins over the class the constructor found.
		if (RobotPawnClass)
		{
			DefaultPawnClass = RobotPawnClass;
		}

		GroundOrigin = FindGroundOrigin();

		if (FSimServer* Server = GetServer())
		{
			Server->AttachGameMode(this);
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
	}

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override
	{
		SetLockstep(false);
		SetFixedTimestep(0.0, 0.0);

		// a connection may be waiting on an instruction only this thread runs
		if (FSimServer* Server = GetServer())
		{
			Server->DetachGameMode(this, [this]() { PumpInstructions(); });
		}

		Super::EndPlay(EndPlayReason);
	}

	virtual void Tick(float DeltaSeconds) override
	{
		UWorld* World = GetWorld();

		// pause only changes inside the drain below, so on entry it says whether the world ticks this frame
		const bool bPausedFrame = World && World->IsPaused();

		PumpInstructions();

		if (bLockstep.load())
		{
			if (!bPausedFrame && StepBudget.load() > 0)
			{
				LockstepTicks++;

				if (--StepBudget <= 0)
				{
					StepBudget = 0;
					SetWorldPaused(true);
					bStepFinishing = true;
				}
			}
			else if (bStepFinishing.load())
			{
				DrainSensors();
				bStepFinishing = false;
				bStepDone      = true;
			}
		}

		if (!bPausedFrame && FixedDtMax > 0.0 && AdaptiveMinDt > 0.0)
		{
			FApp::SetFixedDeltaTime(AdaptiveTimestep());
		}

		// Tick's delta is dilated, so 1/delta counts frames per SIMULATED second: under a
		// dilation of 0.25 it reads four times the rate the machine is really drawing at.
		// That number is what tells a client how finely a sensor can be sampled, so both
		// are kept: this one, and the wall-clock rate the hardware is actually managing.
		const double WallDelta = FApp::GetDeltaTime() > 0.0 ? FApp::GetDeltaTime() : DeltaSeconds;

		FPSCriticalSection->Lock();
		FPS     = 1.0f / DeltaSeconds;
		WallFPS = 1.0 / WallDelta;
		FPSCriticalSection->Unlock();
	
		Super::Tick(DeltaSeconds);
	}

	void PumpInstructions()
	{
		std::shared_ptr<FInstruction<AFlightForgeGameModeBase>> Instruction;
		while (InstructionQueue->Dequeue(Instruction))
		{
			Instruction->Run(*this);
		}
	}

	FSimServer* GetServer() const
	{
		const UFlightForgeGameInstance* Instance = Cast<UFlightForgeGameInstance>(GetGameInstance());
		return Instance ? Instance->Server.get() : nullptr;
	}

	// the "UAV" PlayerStart lowered onto the floor it stands on, or the level origin
	FVector FindGroundOrigin()
	{
		AActor* PlayerStart = FindPlayerStart(0, FString("UAV"));
		if (!PlayerStart)
		{
			UE_LOG(LogTemp, Warning, TEXT("AFlightForgeGameModeBase: no PlayerStart tagged \"UAV\", the ground frame sits at the level origin"));
			return FVector::ZeroVector;
		}

		return PlayerStart->GetActorLocation() - FVector(0, 0, FWireFrame::PlayerStartHeightCm);
	}

	FVector GroundOrigin = FVector::ZeroVector;

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
			
		NewDrone.Pawn = Cast<ARobotPawn>(SpawnDefaultPawnAtTransform(NewDrone.Controller, HideTransform));

		if (NewDrone.Pawn)
		{
			NewDrone.Pawn->SetActorHiddenInGame(true);
			NewDrone.Pawn->SetActorEnableCollision(false);
			NewDrone.Pawn->SetActorTickEnabled(false);
            
			DronePool.Add(NewDrone);
		}
	}

public:
	/* LiftOntoSurface() //{ */

	// landscape collision is one-sided, so a drone asked for below the terrain falls through
	// every trace and renders the world from underneath; the heightfield is what tells us
	FVector LiftOntoSurface(const FVector& Location)
	{
		UWorld* World = GetWorld();

		if (!World)
		{
			return Location;
		}

		constexpr double ClearanceCm = 100.0;

		for (TActorIterator<ALandscapeProxy> It(World); It; ++It)
		{
			const TOptional<float> Height = It->GetHeightAtLocation(Location);

			if (!Height.IsSet() || Location.Z >= *Height)
			{
				continue;
			}

			const FVector Lifted = FVector(Location.X, Location.Y, *Height + ClearanceCm);
			UE_LOG(LogTemp, Warning, TEXT("SpawnRobot: %s is under the terrain, spawning at %s instead"), *Location.ToCompactString(),
			       *Lifted.ToCompactString());

			return Lifted;
		}

		return Location;
	}

	/* //} */

	/* RestOnSurface() //{ */

	// the base link origin sits above the landing gear, so a pose on the floor would start the airframe inside it
	void RestOnSurface(ARobotPawn& Pawn)
	{
		UStaticMeshComponent* Body  = Pawn.RootMeshComponent;
		UWorld*               World = GetWorld();
		if (!Body || !World || !Body->IsCollisionEnabled())
		{
			return;
		}

		FBox Box = Body->Bounds.GetBox();
		if (const UBodySetup* Setup = Body->GetBodySetup(); Setup && Setup->AggGeom.GetElementCount() > 0)
		{
			Box = Setup->AggGeom.CalcAABB(Body->GetComponentTransform());
		}

		const FVector Origin = Pawn.GetActorLocation();
		if (!Box.IsValid || Box.Min.Z >= Origin.Z)
		{
			return;
		}

		constexpr double RestGapCm = 0.5;  // a sweep from a touching start reports penetration, not a hit
		const FVector    Footprint(Box.GetExtent().X, Box.GetExtent().Y, 1.0);
		const FVector    Top(Box.GetCenter().X, Box.GetCenter().Y, Box.Max.Z + Footprint.Z);

		FCollisionQueryParams Params(SCENE_QUERY_STAT(FlightForgeSpawnRest), false, &Pawn);
		FHitResult            Hit;
		const bool bHit = World->SweepSingleByChannel(Hit, Top, FVector(Top.X, Top.Y, Box.Min.Z + Footprint.Z), FQuat::Identity, Body->GetCollisionObjectType(),
		                                              FCollisionShape::MakeBox(Footprint), Params, FCollisionResponseParams(Body->GetCollisionResponseToChannels()));
		if (!bHit || Hit.bStartPenetrating)
		{
			return;
		}

		const double Lift = Hit.Location.Z - Footprint.Z - Box.Min.Z + RestGapCm;
		if (Lift > 0.0)
		{
			Pawn.SetActorLocation(Origin + FVector(0.0, 0.0, Lift));
			UE_LOG(LogTemp, Log, TEXT("FlightForge: raised '%s' by %.1f cm to rest on the surface below it"), *Pawn.Label, Lift);
		}
	}

	/* //} */

	/* robots //{ */

	ARobotPawn* SpawnRobot(const FTransform& Transform, const FString& Name, const FString& Label)
	{
		const FVector Location = LiftOntoSurface(Transform.GetLocation());

		UE_LOG(LogTemp, Log, TEXT("FlightForge: spawning %s '%s' at %s"), *Name, *Label, *Location.ToCompactString());

		FPooledDrone ReadyDrone;
		if (DronePool.Num() > 0)
		{
			ReadyDrone = DronePool.Pop();

			ReadyDrone.Pawn->SetActorLocationAndRotation(Location, Transform.GetRotation());
			ReadyDrone.Pawn->SetActorHiddenInGame(false);
			ReadyDrone.Pawn->SetActorEnableCollision(true);
			ReadyDrone.Pawn->SetActorTickEnabled(true);

			auto Instruction      = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();
			Instruction->Function = [](AFlightForgeGameModeBase& _GameMode) { _GameMode.SpawnDroneToPool(); };
			InstructionQueue->Enqueue(Instruction);
		}
		else
		{
			ReadyDrone.Controller = SpawnPlayerController(ENetRole::ROLE_MAX, FString());
			ReadyDrone.Pawn       = Cast<ARobotPawn>(SpawnDefaultPawnAtTransform(ReadyDrone.Controller, FTransform(Transform.GetRotation(), Location)));
		}

		if (ReadyDrone.Pawn == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AFlightForgeGameModeBase::SpawnRobot: DefaultPawnClass is not an ARobotPawn, nothing spawned"));
			return nullptr;
		}

		ARobotPawn* Pawn   = ReadyDrone.Pawn;
		Pawn->RobotId      = FSimServer::AllocateRobotId();
		Pawn->RobotName    = Name;
		Pawn->Label        = Label;
		Pawn->GroundOrigin = GroundOrigin;
		Pawn->uav_crashed_ = false;
		Pawn->SetCameraCaptureMode(this->CameraCaptureMode);
		Pawn->SetStaticMesh(std::string(TCHAR_TO_UTF8(*Name)));
		RestOnSurface(*Pawn);
		Pawn->WireKind = Name.Contains(TEXT("wing")) ? 2 : (Pawn->PropellerLinkNames().Num() > 0 ? 1 : 0);
		Pawn->Simulate_UE_Physics(3.0f);

		RobotPawnsCriticalSection->Lock();
		RobotPawns.Add(Pawn->RobotId, std::make_pair(Pawn, ReadyDrone.Controller));
		RobotPawnsCriticalSection->Unlock();

		if (FSimServer* Server = GetServer())
		{
			Server->RegisterRobot(Pawn);
		}

		if (!bMutualDroneVisibilityEnabled_)
		{
			Pawn->SetVisibilityOtherDrones(bMutualDroneVisibilityEnabled_);
			UpdateMutualVisibility();
		}

		return Pawn;
	}

	bool RemoveRobot(const int32 RobotId)
	{
		RobotPawnsCriticalSection->Lock();
		std::pair<ARobotPawn*, APlayerController*> Pair(nullptr, nullptr);
		if (const auto* Entry = RobotPawns.Find(RobotId))
		{
			Pair = *Entry;
		}
		RobotPawns.Remove(RobotId);
		RobotPawnsCriticalSection->Unlock();

		if (!Pair.first)
		{
			return false;
		}

		if (Pair.second)
		{
			Pair.second->Destroy();
		}
		Pair.first->Destroy();
		return true;
	}

	ARobotPawn* FindRobotPawn(const int32 RobotId)
	{
		RobotPawnsCriticalSection->Lock();
		const auto* Entry = RobotPawns.Find(RobotId);
		ARobotPawn* Pawn  = Entry ? Entry->first : nullptr;
		RobotPawnsCriticalSection->Unlock();
		return Pawn;
	}

	/* //} */

	CameraCaptureModeEnum GetCaptureMode()
	{
		return CameraCaptureMode;
	}

	bool SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode)
	{
		CameraCaptureMode = CaptureMode;

		for(auto& a : RobotPawns)
		{
			a.Value.first->SetCameraCaptureMode(CaptureMode);
		}

		return true;
	}

	/* clock control //{ */

	bool SetWorldPaused(bool bPaused)
	{
		UWorld* World = GetWorld();
		if (!World)
		{
			return false;
		}

		// the engine call reports false when already in the requested state; the state is what matters
		if (World->IsPaused() != bPaused)
		{
			UGameplayStatics::SetGamePaused(World, bPaused);
		}

		if (World->IsPaused() != bPaused)
		{
			UE_LOG(LogTemp, Warning, TEXT("FlightForge: could not %s the world (no player controller?)"), bPaused ? TEXT("pause") : TEXT("unpause"));
			return false;
		}

		return true;
	}

	// game thread
	bool SetFixedTimestep(double Dt, double MinDt)
	{
		if (Dt < 0.0 || MinDt < 0.0 || (MinDt > 0.0 && MinDt > Dt))
		{
			return false;
		}

		FixedDtMax    = Dt;
		AdaptiveMinDt = Dt > 0.0 ? MinDt : 0.0;

		FApp::SetUseFixedTimeStep(Dt > 0.0);
		FApp::SetFixedDeltaTime(Dt > 0.0 ? Dt : 1.0 / 30.0);

		FFlightForgeSimClock::FixedDt.store(FixedDtMax);
		FFlightForgeSimClock::AdaptiveMinDt.store(AdaptiveMinDt);

		UE_LOG(LogTemp, Log, TEXT("FlightForge: fixed timestep %.6f s (adaptive floor %.6f s)"), FixedDtMax, AdaptiveMinDt);
		return true;
	}

	double GetFixedTimestep() const
	{
		return FApp::UseFixedTimeStep() ? FApp::GetFixedDeltaTime() : 0.0;
	}

	// game thread
	bool SetLockstep(bool bEnabled)
	{
		if (bEnabled == bLockstep.load())
		{
			return true;
		}

		if (!SetWorldPaused(bEnabled))
		{
			return false;
		}

		bLockstep      = bEnabled;
		StepBudget     = 0;
		bStepFinishing = false;
		bStepDone      = true;
		if (bEnabled)
		{
			LockstepTicks = 0;
		}

		FFlightForgeSimClock::bLockstep.store(bEnabled);

		UE_LOG(LogTemp, Log, TEXT("FlightForge: lockstep %s"), bEnabled ? TEXT("on") : TEXT("off"));
		return true;
	}

	// completes asynchronously, poll IsStepDone()
	bool RequestStep(int32 Ticks)
	{
		if (!bLockstep.load() || Ticks < 0 || !bStepDone.load())
		{
			return false;
		}

		if (Ticks == 0)
		{
			return true;
		}

		StepBudget = Ticks;
		bStepDone  = false;

		if (!SetWorldPaused(false))
		{
			StepBudget = 0;
			bStepDone  = true;
			return false;
		}

		return true;
	}

	bool IsStepDone() const
	{
		return bStepDone.load();
	}

	uint64 GetLockstepTicks() const
	{
		return LockstepTicks.load();
	}

	double GetSimTime() const
	{
		const UWorld* World = GetWorld();
		return World ? World->GetTimeSeconds() : 0.0;
	}

	void DrainSensors()
	{
		FlushRenderingCommands();

		RobotPawnsCriticalSection->Lock();
		TArray<ARobotPawn*> Pawns;
		for (auto& Entry : RobotPawns)
		{
			Pawns.Add(Entry.Value.first);
		}
		RobotPawnsCriticalSection->Unlock();

		for (ARobotPawn* Pawn : Pawns)
		{
			if (!IsValid(Pawn))
			{
				continue;
			}

			TInlineComponentArray<UCamera*> Cameras;
			Pawn->GetComponents<UCamera>(Cameras);
			for (UCamera* Camera : Cameras)
			{
				Camera->DrainReadback();
			}
		}
	}

	double AdaptiveTimestep()
	{
		double Dt = FixedDtMax;

		RobotPawnsCriticalSection->Lock();
		for (auto& Entry : RobotPawns)
		{
			ARobotPawn* Pawn = Entry.Value.first;
			if (!IsValid(Pawn))
			{
				continue;
			}

			TInlineComponentArray<UEventCamera*> Cameras;
			Pawn->GetComponents<UEventCamera>(Cameras);
			for (const UEventCamera* Camera : Cameras)
			{
				Dt = FMath::Min(Dt, Camera->SuggestedTimestep(FixedDtMax, AdaptiveMinDt));
			}
		}
		RobotPawnsCriticalSection->Unlock();

		return Dt;
	}

	//}

	// frames per SIMULATED second; divide by the time dilation for the wall-clock rate
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
		RobotPawnsCriticalSection->Lock();
		for (const auto& Entry : RobotPawns)
		{
			Positions.push_back(Entry.Value.first->GetActorLocation());
		}
		RobotPawnsCriticalSection->Unlock();
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
	bool SwitchWorldLevel(const FString& WorldLevelStr)
	{
		// full package path: preload the map through the Entry level for a fast switch
		if (WorldLevelStr.Contains(TEXT("/")))
		{
			const FString NameOfWorld = FPackageName::GetShortName(WorldLevelStr);
			const FString Options = FString::Printf(TEXT("?game=/Script/FlightForgeCore.FlightForgeGameModeBase?NextMapName=%s?NextMapPath=%s"),
				*NameOfWorld,
				*WorldLevelStr);

			UE_LOG(LogTemp, Warning, TEXT("OpenLevel: Empty"));
			UGameplayStatics::OpenLevel(this, "/Engine/Maps/Entry", true, Options);
		}
		else
		{
			UGameplayStatics::OpenLevel(this, FName(WorldLevelStr), true, TEXT("?game=/Script/FlightForgeCore.FlightForgeGameModeBase"));
		}

		return true;
	}

	void StartAsyncLoadingTargetMap(const FString& NextMapNameStr, const FString& NextMapPathStr)
	{
		FName TargetMapName = FName(*NextMapNameStr);
		
		FLoadPackageAsyncDelegate CompletionDelegate = FLoadPackageAsyncDelegate::CreateUObject(
			this, 
			&AFlightForgeGameModeBase::OnMapPackageLoaded, 
			TargetMapName
		);
		
		LoadPackageAsync(NextMapPathStr, CompletionDelegate, 0, PKG_ContainsMap);
	}
	

	void OnMapPackageLoaded(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result, FName MapNameToOpen)
	{
		if (Result == EAsyncLoadingResult::Succeeded)
		{
			UE_LOG(LogTemp, Log, TEXT("Target Map %s is fully in RAM. Executing fast switch."), *MapNameToOpen.ToString());

			// Without the override the destination map uses its own GameModeOverride, and a
			// map that carries one leaves the server with no game mode registered: the
			// simulator comes up but answers nothing.
			UGameplayStatics::OpenLevel(this, MapNameToOpen, true, TEXT("?game=/Script/FlightForgeCore.FlightForgeGameModeBase"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load map package to RAM: %s"), *PackageName.ToString());
		}
	}

	// Unreal cm; set at BeginPlay, read by any thread
	FVector GetGroundOrigin() const
	{
		return GroundOrigin;
	}

	/* object spawning //{ */

	UWorldObjectSpawner* GetObjectSpawner()
	{
		if(!ObjectSpawner)
		{
			ObjectSpawner = NewObject<UWorldObjectSpawner>(this);
			ObjectSpawner->SetLibrary(&ObjectLibrary);
		}

		return ObjectSpawner;
	}

	TArray<int32> SpawnObjectsFromConfig(const FSpawnConfig& Config, const FSpawnBatchPtr& Batch = nullptr)
	{
		return GetObjectSpawner()->SpawnFromConfig(GetWorld(), Config, GroundOrigin, Batch);
	}

	// Used by the spawn_objects_from_yaml command, where the document arrives over the
	// socket and so has no directory of its own to resolve relative model paths against.
	TArray<int32> SpawnObjectsFromYaml(const std::string& Yaml, FString& OutError, const FSpawnBatchPtr& Batch)
	{
		FSpawnConfig Config;
		if(!USpawnConfigUtils::ParseSpawnConfig(FString(UTF8_TO_TCHAR(Yaml.c_str())), Config, OutError))
		{
			return TArray<int32>();
		}

		for(FString& ModelPath : Config.ModelPaths)
		{
			if(FPaths::IsRelative(ModelPath))
			{
				ModelPath = FPaths::Combine(FPaths::ProjectDir(), ModelPath);
			}
		}

		if(Config.ModelPaths.IsEmpty())
		{
			Config.ModelPaths.Add(FPaths::Combine(FPaths::ProjectDir(), TEXT("FlightForge")));
		}

		return SpawnObjectsFromConfig(Config, Batch);
	}

	bool RemoveSpawnedObject(const int32 ObjectId)
	{
		return GetObjectSpawner()->RemoveObject(ObjectId);
	}

	int32 RemoveAllSpawnedObjects()
	{
		return GetObjectSpawner()->RemoveAllObjects();
	}

	bool MoveSpawnedObject(const int32 ObjectId, const FTransform& BaseTransform)
	{
		return GetObjectSpawner()->MoveObject(ObjectId, BaseTransform);
	}

	TArray<FSpawnedObjectState> ListSpawnedObjects()
	{
		return GetObjectSpawner()->ListObjects();
	}

	FString ExportSceneYaml()
	{
		return GetObjectSpawner()->ExportScene(GroundOrigin);
	}

	TArray<FString> ListSpawnableAssets(const FString& PathPrefix, const FString& NameFilter, const int32 MaxResults, bool& bOutTruncated)
	{
		// The same on-disk model directory SpawnObjectsFromYaml falls back to, so the
		// catalog and the spawner agree on what a bare name means.
		TArray<FString> ModelPaths;
		ModelPaths.Add(FPaths::Combine(FPaths::ProjectDir(), TEXT("FlightForge")));

		return UAssetResolver::ListSpawnableAssets(PathPrefix, NameFilter, ModelPaths, MaxResults, bOutTruncated);
	}

	/* //} */

	/* object library //{ */

	FObjectLibrary ObjectLibrary;

	bool bProjectAssetsListed = false;

	void LoadStartupLibraries()
	{
		TArray<FString> Paths;
		IFileManager::Get().FindFiles(Paths, *FPaths::Combine(FPaths::ProjectDir(), TEXT("FlightForge"), TEXT("Libraries"), TEXT("*.yaml")), true, false);

		for (FString& Path : Paths)
		{
			Path = FPaths::Combine(FPaths::ProjectDir(), TEXT("FlightForge"), TEXT("Libraries"), Path);
		}

		FString FromCommandLine;
		if (FParse::Value(FCommandLine::Get(), TEXT("FlightForgeLibrary="), FromCommandLine, false))
		{
			TArray<FString> Listed;
			FromCommandLine.ParseIntoArray(Listed, TEXT(","));
			Paths.Append(Listed);
		}

		for (const FString& Path : Paths)
		{
			int32   Count = 0;
			FString Error;
			if (!ObjectLibrary.Load(Path, Count, Error))
			{
				UE_LOG(LogTemp, Error, TEXT("FlightForge: library %s: %s"), *Path, *Error);
			}
		}
	}

	bool LoadLibrary(const FString& Path, int32& OutCount, FString& OutError)
	{
		return ObjectLibrary.Load(Path, OutCount, OutError);
	}

	const FObjectLibrary& GetObjectLibrary()
	{
		if (!bProjectAssetsListed)
		{
			bool bTruncated = false;
			ObjectLibrary.SetProjectAssets(UAssetResolver::ListSpawnableAssets(FString(), FString(), {}, MAX_int32, bTruncated));
			bProjectAssetsListed = true;
		}

		return ObjectLibrary;
	}

	TArray<FLibraryObject> SearchLibrary(const FString& Library, const FString& Class, const FString& Filter, const int32 Offset, const int32 Limit, int32& OutTotal)
	{
		return GetObjectLibrary().Search(Library, Class, Filter, Offset, Limit, OutTotal);
	}

	/* //} */

	bool bMutualDroneVisibilityEnabled_ = true;

	void SetMutualVisibility(bool bMutualDroneVisibilityEnabled)
	{
		bMutualDroneVisibilityEnabled_ = bMutualDroneVisibilityEnabled;
	}

	void UpdateMutualVisibility()
	{
		for (auto DroneToUpdate : RobotPawns)
		{
			TArray<AActor*> DronesToBeHidden;
			for (auto RobotPawn : RobotPawns)
			{
				if(DroneToUpdate != RobotPawn)
				{
					DronesToBeHidden.Add(RobotPawn.Value.first);
				}
			}
			// DroneToUpdate.Value.first->SceneCaptureComponent2DRgb->HiddenActors.Empty();
			// DroneToUpdate.Value.first->SceneCaptureComponent2DRgb->HiddenActors.Append(DronesToBeHidden);
			DroneToUpdate.Value.first->UpdateCameraSensorsMutualVisibility(DronesToBeHidden);
			// UE_LOG(LogTemp, Error, TEXT("hidden actors count is %d"), DroneToUpdate.Value.first->SceneCaptureComponent2DRgb->HiddenActors.Num());
		}
	}
		
	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<AFlightForgeGameModeBase>>>> InstructionQueue;
	
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category=Classes)
	TSubclassOf<ARobotPawn> RobotPawnClass;
};
