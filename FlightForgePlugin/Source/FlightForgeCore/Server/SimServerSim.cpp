#include "SimServerInternal.h"

#include "GameFramework/WorldSettings.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "FlightForgeCore/Segmentation/SegmentationTagger.h"
#include "FlightForgeCore/Utils/DistanceFieldQuery.h"
#include "FlightForgeCore/Utils/SkyControl.h"
#include "FlightForgeCore/Utils/SurfaceHeightQuery.h"
#include "FlightForgeCore/Utils/WorldCatalog.h"
#include "FlightForgeCore/Utils/WorldMeshExtractor.h"
#include "FlightForgeCore/RenderSettings/ViewportMatch.h"
#include "FlightForgeCore/Sensors/SensorWire.h"

static constexpr uint16 ApiMajor = 1;
static constexpr uint16 ApiMinor = 8;

/* connection //{ */

void FSimServer::Ping(const ffw::Request& Request, FWireReply& Reply)
{
}

// the compiled schema ships beside the plugin's generated headers
static const TArray<uint8>& CompiledSchema()
{
	static const TArray<uint8> Schema = [] {
		TArray<uint8> Bytes;
		if (const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("FlightForgePlugin")))
		{
			const FString Path =
			    FPaths::Combine(Plugin->GetBaseDir(), TEXT("Source"), TEXT("MessageSerialization"), TEXT("Public"), TEXT("Generated"), TEXT("flightforge.bfbs"));
			if (!FFileHelper::LoadFileToArray(Bytes, *Path))
			{
				UE_LOG(LogTemp, Warning, TEXT("FlightForge: %s is missing, get_info carries no schema"), *Path);
			}
		}
		return Bytes;
	}();
	return Schema;
}

void FSimServer::GetInfo(const ffw::Request& Request, FWireReply& Reply)
{
	const TArray<uint8>& Schema = CompiledSchema();
	const auto           Bytes  = Reply.CopyVector(Schema.GetData(), Schema.Num());
	Reply.Set(ffw::CreateGetInfoResponse(Reply.Builder, ApiMajor, ApiMinor, Bytes));
}

/* //} */

/* time //{ */

void FSimServer::GetTime(const ffw::Request& Request, FWireReply& Reply)
{
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	double SimTime  = 0.0;
	double Dilation = 1.0;
	if (const UWorld* World = GameMode->GetWorld())
	{
		SimTime  = World->GetTimeSeconds();
		Dilation = World->GetWorldSettings() ? World->GetWorldSettings()->GetEffectiveTimeDilation() : 1.0;
	}

	Reply.Set(ffw::CreateGetTimeResponse(Reply.Builder, FPlatformTime::Seconds(), SimTime, Dilation));
}

void FSimServer::GetFps(const ffw::Request& Request, FWireReply& Reply)
{
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (GameMode)
	{
		Reply.Set(ffw::CreateGetFpsResponse(Reply.Builder, GameMode->GetFPS()));
	}
}

void FSimServer::SetTimeDilation(const ffw::Request& Request, FWireReply& Reply)
{
	const double Dilation = Request.body_as_SetTimeDilationRequest()->dilation();
	if (!(Dilation > 0.0))
	{
		Reply.Fail(FString::Printf(TEXT("time dilation %g must be positive"), Dilation));
		return;
	}

	std::lock_guard<std::mutex> Lock(WorldMutex);
	const FGameModeLease        GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	double Applied = 1.0;
	RunOnGameThread(*GameMode.GameMode, [Dilation, &Applied](AFlightForgeGameModeBase& Mode) {
		if (UWorld* World = Mode.GetWorld())
		{
			UGameplayStatics::SetGlobalTimeDilation(World, static_cast<float>(Dilation));
			// the engine clamps to the world's limits; the client is told what it got
			Applied = World->GetWorldSettings() ? World->GetWorldSettings()->GetEffectiveTimeDilation() : 1.0;
		}
	});

	Reply.Set(ffw::CreateSetTimeDilationResponse(Reply.Builder, Applied));
}

void FSimServer::SetFixedTimestep(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::SetFixedTimestepRequest* Ask = Request.body_as_SetFixedTimestepRequest();

	std::lock_guard<std::mutex> Lock(WorldMutex);
	const FGameModeLease        GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	bool   bOk     = false;
	double Applied = 0.0;
	RunOnGameThread(*GameMode.GameMode, [Ask, &bOk, &Applied](AFlightForgeGameModeBase& Mode) {
		bOk     = Mode.SetFixedTimestep(Ask->dt(), Ask->adaptive_min_dt());
		Applied = Mode.GetFixedTimestep();
	});

	if (!bOk)
	{
		Reply.Fail(FString::Printf(TEXT("fixed timestep %g s with adaptive floor %g s is not valid"), Ask->dt(), Ask->adaptive_min_dt()));
		return;
	}
	Reply.Set(ffw::CreateSetFixedTimestepResponse(Reply.Builder, Applied));
}

void FSimServer::SetLockstep(const ffw::Request& Request, FWireReply& Reply)
{
	const bool bEnabled = Request.body_as_SetLockstepRequest()->enabled();

	std::lock_guard<std::mutex> Lock(WorldMutex);
	const FGameModeLease        GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	bool bOk = false;
	RunOnGameThread(*GameMode.GameMode, [bEnabled, &bOk](AFlightForgeGameModeBase& Mode) { bOk = Mode.SetLockstep(bEnabled); });

	if (!bOk)
	{
		Reply.Fail(TEXT("the world could not be paused (no player controller?)"));
	}
}

void FSimServer::Step(const ffw::Request& Request, FWireReply& Reply)
{
	const uint32 Ticks = Request.body_as_StepRequest()->ticks();

	std::lock_guard<std::mutex> Lock(WorldMutex);
	const FGameModeLease        GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	bool bOk = false;
	RunOnGameThread(*GameMode.GameMode, [Ticks, &bOk](AFlightForgeGameModeBase& Mode) { bOk = Mode.RequestStep(static_cast<int32>(FMath::Min<uint32>(Ticks, MAX_int32))); });

	if (!bOk)
	{
		Reply.Fail(TEXT("step needs lockstep on and no step in progress"));
		return;
	}

	// a step not done within a minute means the world went away underneath it
	const double Deadline = FPlatformTime::Seconds() + 60.0;
	while (!GameMode->IsStepDone() && FPlatformTime::Seconds() < Deadline && bRunning)
	{
		FPlatformProcess::SleepNoStats(0.0002f);
	}

	if (!GameMode->IsStepDone())
	{
		Reply.Fail(TEXT("the step did not finish within 60 s"));
		return;
	}

	Reply.Set(ffw::CreateStepResponse(Reply.Builder, GameMode->GetSimTime(), GameMode->GetLockstepTicks()));
}

/* //} */

/* world //{ */

void FSimServer::SwitchWorld(const ffw::Request& Request, FWireReply& Reply)
{
	FString Package;
	FString Error;
	if (!FWorldCatalog::Resolve(WireString(Request.body_as_SwitchWorldRequest()->level()), Package, Error))
	{
		Reply.Fail(Error);
		return;
	}

	std::lock_guard<std::mutex> Lock(WorldMutex);

	uint64        SinceAttach = 0;
	TArray<int32> Removed;
	{
		const FGameModeLease GameMode = LeaseGameMode(Reply);
		if (!GameMode)
		{
			return;
		}

		{
			std::lock_guard<std::mutex> StateLock(GameModeMutex);
			SinceAttach   = AttachCount;
			SwitchTarget  = Package;
			SwitchFailure = FString();
		}
		{
			std::lock_guard<std::mutex> RobotsLock(RobotsMutex);
			Robots.GetKeys(Removed);
		}

		PostToGameThread(*GameMode.GameMode, [Package](AFlightForgeGameModeBase& Mode) { Mode.SwitchWorldLevel(Package); });
	}

	const bool bLoaded = WaitForWorld(Package, SinceAttach, Error);
	{
		std::lock_guard<std::mutex> StateLock(GameModeMutex);
		SwitchTarget = FString();
	}
	{
		std::lock_guard<std::mutex> RobotsLock(RobotsMutex);
		Removed.RemoveAll([this](const int32 Id) { return Robots.Contains(Id); });
		for (const int32 Id : Removed)
		{
			RobotsLostToSwitch.Add(Id, Package);
		}
	}

	if (!bLoaded)
	{
		Reply.Fail(Error);
		return;
	}

	Removed.Sort();
	Reply.Set(ffw::CreateSwitchWorldResponse(Reply.Builder, Reply.String(Package), Reply.Builder.CreateVector(Removed.GetData(), Removed.Num())));
}

// the old game mode detaches, the Entry map's may come and go, and the target's must tick once
bool FSimServer::WaitForWorld(const FString& Package, const uint64 SinceAttach, FString& OutError)
{
	constexpr double TimeoutSeconds = 1800.0;
	const double     Deadline       = FPlatformTime::Seconds() + TimeoutSeconds;

	while (bRunning && FPlatformTime::Seconds() < Deadline)
	{
		FString Attached;
		{
			std::lock_guard<std::mutex> Lock(GameModeMutex);
			if (!SwitchFailure.IsEmpty())
			{
				OutError = FString::Printf(TEXT("switch_world to %s failed: %s"), *Package, *SwitchFailure);
				return false;
			}
			if (AttachedGameMode && AttachCount != SinceAttach)
			{
				Attached = AttachedWorld;
			}
		}

		if (!Attached.IsEmpty() && Attached != TEXT("/Engine/Maps/Entry"))
		{
			if (!Attached.Equals(Package, ESearchCase::IgnoreCase))
			{
				OutError = FString::Printf(TEXT("switch_world to %s loaded %s instead"), *Package, *Attached);
				return false;
			}

			FWireReply           Scratch;
			const FGameModeLease GameMode = LeaseGameMode(Scratch);
			bool                 bTicking = false;
			if (GameMode)
			{
				RunOnGameThread(*GameMode.GameMode, [&bTicking](AFlightForgeGameModeBase& Mode) { bTicking = Mode.GetWorld()->HasBegunPlay(); });
			}
			if (bTicking)
			{
				return true;
			}
		}

		FPlatformProcess::SleepNoStats(0.01f);
	}

	OutError = bRunning ? FString::Printf(TEXT("switch_world to %s did not finish within %.0f s"), *Package, TimeoutSeconds) : TEXT("the simulator is shutting down");
	return false;
}

void FSimServer::ListWorlds(const ffw::Request& Request, FWireReply& Reply)
{
	const TArray<FWorldEntry>& Worlds = FWorldCatalog::Get();

	std::vector<flatbuffers::Offset<ffw::WorldInfo>> Infos;
	Infos.reserve(Worlds.Num());
	for (const FWorldEntry& World : Worlds)
	{
		Infos.push_back(ffw::CreateWorldInfo(Reply.Builder, Reply.String(World.Name), Reply.String(World.Package), Reply.String(World.DisplayName)));
	}

	FString Current;
	FString Loading;
	{
		std::lock_guard<std::mutex> Lock(GameModeMutex);
		Loading = SwitchTarget;
		if (Loading.IsEmpty() && AttachedWorld != TEXT("/Engine/Maps/Entry"))
		{
			Current = AttachedWorld;
		}
	}

	Reply.Set(ffw::CreateListWorldsResponse(Reply.Builder, Reply.Builder.CreateVector(Infos), Reply.String(Current), Reply.String(Loading)));
}

void FSimServer::SetWeather(const ffw::Request& Request, FWireReply& Reply)
{
	const int32          Weather  = static_cast<int32>(Request.body_as_SetWeatherRequest()->weather());
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	bool    bOk = false;
	FString Error;
	RunOnGameThread(*GameMode.GameMode, [Weather, &bOk, &Error](AFlightForgeGameModeBase& Mode) {
		bOk = Mode.GetWorld()->GetSubsystem<USkyControl>()->SetWeather(Weather, Error);
	});
	if (!bOk)
	{
		Reply.Fail(Error.IsEmpty() ? TEXT("set_weather did not run on the game thread") : Error);
	}
}

void FSimServer::SetDaytime(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::SetDaytimeRequest*     Ask      = Request.body_as_SetDaytimeRequest();
	const int32                       Hour     = Ask->hour();
	const int32                       Minute   = Ask->minute();
	const flatbuffers::Optional<bool> Animate  = Ask->animate();
	const FGameModeLease              GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	bool    bOk = false;
	FString Error;
	RunOnGameThread(*GameMode.GameMode, [Hour, Minute, Animate, &bOk, &Error](AFlightForgeGameModeBase& Mode) {
		USkyControl* Sky = Mode.GetWorld()->GetSubsystem<USkyControl>();
		bOk              = (!Animate.has_value() || Sky->SetDaytimeAnimated(*Animate, Error)) && Sky->SetDaytime(Hour, Minute, Error);
	});
	if (!bOk)
	{
		Reply.Fail(Error.IsEmpty() ? TEXT("set_daytime did not run on the game thread") : Error);
	}
}

void FSimServer::GetDaytime(const ffw::Request& Request, FWireReply& Reply)
{
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	bool    bOk       = false;
	double  Hours     = 0.0;
	bool    bAnimated = false;
	FString Sky;
	FString Error;
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) {
		bOk = Mode.GetWorld()->GetSubsystem<USkyControl>()->GetDaytime(Hours, bAnimated, Sky, Error);
	});
	if (!bOk)
	{
		Reply.Fail(Error.IsEmpty() ? TEXT("get_daytime did not run on the game thread") : Error);
		return;
	}
	Reply.Set(ffw::CreateGetDaytimeResponse(Reply.Builder, Hours, bAnimated, Reply.String(Sky)));
}

void FSimServer::GetViewportImage(const ffw::Request& Request, FWireReply& Reply)
{ //{
	const ffw::GetViewportImageRequest* Ask      = Request.body_as_GetViewportImageRequest();
	const FGameModeLease                GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	USceneCaptureComponent2D* Capture = nullptr;
	TOptional<FRobotLease>    Robot;
	if (Request.robot_id().has_value() && !Ask->release())
	{
		Robot.Emplace(LeaseRobot(Request, Reply));
		if (!*Robot)
		{
			return;
		}
		const int32    SensorId = Request.sensor_id().has_value() ? *Request.sensor_id() : -1;
		const UCamera* Camera   = Cast<UCamera>((*Robot)->FindSensorById(SensorId));
		Capture                 = Camera ? Camera->ViewCapture(Ask->view()) : nullptr;
		if (!Capture)
		{
			Reply.Fail(FString::Printf(TEXT("robot %d has no camera %d with view %d"), Robot->Id, SensorId, Ask->view()));
			return;
		}
	}

	bool    bOk = true;
	FString Error;
	auto    Shot = std::make_shared<FViewportShot>();
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) {
		UViewportMatch* Match = Mode.GetWorld()->GetSubsystem<UViewportMatch>();
		if (Ask->release())
		{
			Match->Release();
			return;
		}
		bOk = (!Capture || Match->Match(Capture, Error)) && Match->RequestShot(Shot, Error);
	});
	if (!bOk)
	{
		Reply.Fail(Error);
		return;
	}
	if (Ask->release())
	{
		return;
	}

	const double Deadline = FPlatformTime::Seconds() + 10.0;
	while (!Shot->bDone.load() && FPlatformTime::Seconds() < Deadline)
	{
		FPlatformProcess::Sleep(0.002f);
	}
	if (!Shot->bDone.load())
	{
		Reply.Fail(TEXT("the viewport drew no frame at the camera's size within 10 s"));
		return;
	}

	FCameraFrame Frame;
	const std::unique_ptr<ffw::ImageEncodingT> Encoding(Ask->encoding() ? Ask->encoding()->UnPack() : nullptr);
	UCamera::CompressFrame(Shot->Pixels, Shot->Size, FSensorWire::Encoding(Encoding.get()), false, Frame);
	const auto Image = FSensorWire::Image(Reply, Frame, FSensorWire::ColourFormat(Frame.Codec));
	Reply.Set(ffw::CreateViewportImageResponse(Reply.Builder, Shot->Stamp, Shot->Fov, Image));
} //}

void FSimServer::SetGraphicsSettings(const ffw::Request& Request, FWireReply& Reply)
{
	const int32          Quality  = static_cast<int32>(Request.body_as_SetGraphicsSettingsRequest()->quality());
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	bool bOk = false;
	RunOnGameThread(*GameMode.GameMode, [Quality, &bOk](AFlightForgeGameModeBase& Mode) { bOk = Mode.SetGraphicsSettings(Quality); });
	if (!bOk)
	{
		Reply.Fail(TEXT("no game user settings to apply the quality to"));
	}
}

void FSimServer::SetForestDensity(const ffw::Request& Request, FWireReply& Reply)
{
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (GameMode)
	{
		GameMode->ForestDensityLevel = Request.body_as_SetForestDensityRequest()->level();
	}
}

void FSimServer::SetForestHillyLevel(const ffw::Request& Request, FWireReply& Reply)
{
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (GameMode)
	{
		GameMode->ForestHillyLevel = Request.body_as_SetForestHillyLevelRequest()->level();
	}
}

void FSimServer::SetMutualVisibility(const ffw::Request& Request, FWireReply& Reply)
{
	const bool           bEnabled = Request.body_as_SetMutualVisibilityRequest()->enabled();
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (GameMode)
	{
		RunOnGameThread(*GameMode.GameMode, [bEnabled](AFlightForgeGameModeBase& Mode) { Mode.SetMutualVisibility(bEnabled); });
	}
}

void FSimServer::GetCaptureMode(const ffw::Request& Request, FWireReply& Reply)
{
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (GameMode)
	{
		Reply.Set(ffw::CreateGetCaptureModeResponse(Reply.Builder, static_cast<ffw::CaptureMode>(GameMode->GetCaptureMode())));
	}
}

void FSimServer::SetCaptureMode(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::CaptureMode Mode = Request.body_as_SetCaptureModeRequest()->mode();
	if (Mode > ffw::CaptureMode::MAX)
	{
		Reply.Fail(TEXT("unknown capture mode"));
		return;
	}

	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (GameMode)
	{
		RunOnGameThread(*GameMode.GameMode, [Mode](AFlightForgeGameModeBase& GM) { GM.SetCameraCaptureMode(static_cast<CameraCaptureModeEnum>(Mode)); });
	}
}

void FSimServer::GetWorldOrigin(const ffw::Request& Request, FWireReply& Reply)
{
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (GameMode)
	{
		const ffw::Vec3 Origin = FWireFrame::ToVec3(GameMode->GetGroundOrigin());
		Reply.Set(ffw::CreateGetWorldOriginResponse(Reply.Builder, &Origin));
	}
}

void FSimServer::GetClassMap(const ffw::Request& Request, FWireReply& Reply)
{
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	std::vector<flatbuffers::Offset<ffw::ClassEntry>> Classes;
	uint64                                            Hash = 0;

	const UWorld*              World  = GameMode->GetWorld();
	const USegmentationTagger* Tagger = World ? World->GetSubsystem<USegmentationTagger>() : nullptr;
	if (Tagger)
	{
		TArray<int32> Ids;
		Tagger->Classes().GetKeys(Ids);
		Ids.Sort();
		for (const int32 Id : Ids)
		{
			const FSegClass& Class = Tagger->Classes()[Id];
			Classes.push_back(ffw::CreateClassEntry(Reply.Builder, Class.Id, Class.ParentId, Reply.String(Class.Path)));
		}
		Hash = Tagger->ClassMapHash();
	}

	Reply.Set(ffw::CreateGetClassMapResponse(Reply.Builder, Reply.Builder.CreateVector(Classes), Hash));
}

/* //} */

/* scene //{ */

void FSimServer::SpawnObjectsFromYaml(const ffw::Request& Request, FWireReply& Reply)
{
	const flatbuffers::String* Yaml = Request.body_as_SpawnObjectsFromYamlRequest()->yaml();
	const std::string          Document(Yaml ? Yaml->str() : std::string());

	std::lock_guard<std::mutex> Lock(WorldMutex);
	const FGameModeLease        GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	TArray<int32>        SpawnedIds;
	FString              Error;
	const FSpawnBatchPtr Batch = MakeShared<FSpawnBatch, ESPMode::ThreadSafe>();

	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) { SpawnedIds = Mode.SpawnObjectsFromYaml(Document, Error, Batch); });

	if (!Error.IsEmpty())
	{
		Reply.Fail(Error);
		return;
	}

	bool bPending = false;
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) { bPending = Batch->Failed.IsEmpty() && Batch->Pending.load() > 0; });

	// models import off the game thread, so the reply waits here and the sim keeps ticking
	if (bPending)
	{
		const double Deadline = FPlatformTime::Seconds() + 300.0;
		FGenericPlatformProcess::ConditionalSleep([&Batch, Deadline]() { return Batch->Pending.load() <= 0 || FPlatformTime::Seconds() > Deadline; }, 0.01f);
	}

	// all or nothing: a scene with a hole in it is worse than no scene
	TArray<FString> Failed;
	const bool      bTimedOut = Batch->Pending.load() > 0;
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) {
		Failed = Batch->Failed;
		if (Failed.IsEmpty() && !bTimedOut)
		{
			return;
		}
		for (const int32 Id : SpawnedIds)
		{
			Mode.RemoveSpawnedObject(Id);
		}
		SpawnedIds.Reset();
	});

	if (!Failed.IsEmpty())
	{
		Reply.Fail(FString::Printf(TEXT("spawned nothing: could not place %s"), *FString::Join(Failed, TEXT(", "))));
		return;
	}
	if (bTimedOut)
	{
		Reply.Fail(TEXT("spawned nothing: the models did not import within 300 s"));
		return;
	}

	Reply.Set(ffw::CreateSpawnObjectsFromYamlResponse(Reply.Builder, Reply.CopyVector(SpawnedIds.GetData(), SpawnedIds.Num())));
}

void FSimServer::RemoveSpawnedObject(const ffw::Request& Request, FWireReply& Reply)
{
	const int32 ObjectId = Request.body_as_RemoveSpawnedObjectRequest()->object_id();

	std::lock_guard<std::mutex> Lock(WorldMutex);
	const FGameModeLease        GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	int32 Removed = 0;
	RunOnGameThread(*GameMode.GameMode, [ObjectId, &Removed](AFlightForgeGameModeBase& Mode) {
		Removed = ObjectId < 0 ? Mode.RemoveAllSpawnedObjects() : (Mode.RemoveSpawnedObject(ObjectId) ? 1 : 0);
	});

	// clearing an empty scene is the requested state; one missing id is not
	if (ObjectId >= 0 && Removed == 0)
	{
		Reply.Fail(FString::Printf(TEXT("no spawned object %d"), ObjectId));
		return;
	}

	Reply.Set(ffw::CreateRemoveSpawnedObjectResponse(Reply.Builder, Removed));
}

void FSimServer::MoveSpawnedObject(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::MoveSpawnedObjectRequest* Ask = Request.body_as_MoveSpawnedObjectRequest();

	std::lock_guard<std::mutex> Lock(WorldMutex);
	const FGameModeLease        GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	FTransform Base = FWireFrame::FromGroundPose(Ask->pose(), GameMode->GetGroundOrigin());
	Base.SetScale3D(Ask->scale() ? FWireFrame::FromVec3(*Ask->scale()) : FVector::OneVector);

	bool bMoved = false;
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) { bMoved = Mode.MoveSpawnedObject(Ask->object_id(), Base); });

	if (!bMoved)
	{
		Reply.Fail(FString::Printf(TEXT("no spawned object %d"), Ask->object_id()));
	}
}

void FSimServer::ListSpawnedObjects(const ffw::Request& Request, FWireReply& Reply)
{
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	TArray<FSpawnedObjectState> States;
	RunOnGameThread(*GameMode.GameMode, [&States](AFlightForgeGameModeBase& Mode) { States = Mode.ListSpawnedObjects(); });

	std::vector<flatbuffers::Offset<ffw::SpawnedObject>> Objects;
	for (const FSpawnedObjectState& State : States)
	{
		const ffw::Pose Pose  = FWireFrame::ToGroundPose(State.Transform, GameMode->GetGroundOrigin());
		const ffw::Vec3 Scale = FWireFrame::ToVec3(State.Transform.GetScale3D());
		Objects.push_back(ffw::CreateSpawnedObject(Reply.Builder, State.Id, Reply.String(State.Asset), State.Stencil, &Pose, &Scale));
	}

	Reply.Set(ffw::CreateListSpawnedObjectsResponse(Reply.Builder, Reply.Builder.CreateVector(Objects)));
}

void FSimServer::ExportScene(const ffw::Request& Request, FWireReply& Reply)
{
	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	FString Yaml;
	RunOnGameThread(*GameMode.GameMode, [&Yaml](AFlightForgeGameModeBase& Mode) { Yaml = Mode.ExportSceneYaml(); });

	Reply.Set(ffw::CreateExportSceneResponse(Reply.Builder, Reply.String(Yaml)));
}

void FSimServer::ListAssets(const ffw::Request& Request, FWireReply& Reply)
{
	constexpr int32 MaxResults = 5000;

	const ffw::ListAssetsRequest* Ask      = Request.body_as_ListAssetsRequest();
	const FString                 Prefix   = WireString(Ask->path_prefix());
	const FString                 Filter   = WireString(Ask->name_filter());
	const FGameModeLease          GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	TArray<FString> Names;
	bool            bTruncated = false;
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) { Names = Mode.ListSpawnableAssets(Prefix, Filter, MaxResults, bTruncated); });

	std::vector<flatbuffers::Offset<flatbuffers::String>> Assets;
	for (const FString& Name : Names)
	{
		Assets.push_back(Reply.String(Name));
	}

	Reply.Set(ffw::CreateListAssetsResponse(Reply.Builder, Reply.Builder.CreateVector(Assets), bTruncated));
}

void FSimServer::LoadLibrary(const ffw::Request& Request, FWireReply& Reply)
{
	const FString Path = WireString(Request.body_as_LoadLibraryRequest()->path());

	std::lock_guard<std::mutex> Lock(WorldMutex);
	const FGameModeLease        GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	bool    bLoaded = false;
	int32   Count   = 0;
	FString Error;
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) { bLoaded = Mode.LoadLibrary(Path, Count, Error); });

	if (!bLoaded)
	{
		Reply.Fail(Error.IsEmpty() ? FString::Printf(TEXT("could not load the library at %s"), *Path) : Error);
		return;
	}

	Reply.Set(ffw::CreateLoadLibraryResponse(Reply.Builder, Count));
}

void FSimServer::ListLibrary(const ffw::Request& Request, FWireReply& Reply)
{
	constexpr uint32 MaxResults = 2000;

	const ffw::ListLibraryRequest* Ask      = Request.body_as_ListLibraryRequest();
	const FString                  Library  = WireString(Ask->library());
	const FString                  Class    = WireString(Ask->object_class());
	const FString                  Filter   = WireString(Ask->filter());
	const int32                    Offset   = static_cast<int32>(FMath::Min<uint32>(Ask->offset(), MAX_int32));
	const int32                    Limit    = static_cast<int32>(FMath::Min(Ask->limit(), MaxResults));
	const FGameModeLease           GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	TArray<FLibraryObject> Found;
	int32                  Total = 0;
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) { Found = Mode.SearchLibrary(Library, Class, Filter, Offset, Limit, Total); });

	std::vector<flatbuffers::Offset<ffw::LibraryObject>> Objects;
	for (const FLibraryObject& Object : Found)
	{
		Objects.push_back(ffw::CreateLibraryObject(Reply.Builder, Reply.String(Object.Library), Reply.String(Object.Id), Reply.String(Object.SpawnName),
		                                           Reply.String(Object.Class), Reply.String(Object.Description), Reply.String(Object.License),
		                                           Reply.String(Object.Thumbnail), Object.Scale, Object.HeightM, Object.bRuntime));
	}

	Reply.Set(ffw::CreateListLibraryResponse(Reply.Builder, Reply.Builder.CreateVector(Objects), static_cast<uint32>(FMath::Max(Total, 0))));
}

void FSimServer::GetWorldMesh(const ffw::Request& Request, FWireReply& Reply)
{
	constexpr int32 DefaultMaxTriangles = 2000000;

	const ffw::GetWorldMeshRequest* Ask = Request.body_as_GetWorldMeshRequest();

	FBox Bounds(EForceInit::ForceInit);
	if (Ask->center() && Ask->extent() && Ask->extent()->x() > 0.0 && Ask->extent()->y() > 0.0 && Ask->extent()->z() > 0.0)
	{
		const FVector Center = FWireFrame::FromVec3(*Ask->center());
		const FVector Extent = FWireFrame::FromVec3(*Ask->extent());
		Bounds += Center - Extent;
		Bounds += Center + Extent;
	}

	const int32 MaxTriangles = Ask->max_triangles() > 0 ? static_cast<int32>(FMath::Min<uint32>(Ask->max_triangles(), MAX_int32)) : DefaultMaxTriangles;

	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	FWorldMeshResult Mesh;
	bool             bOk = false;
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) {
		bOk = FWorldMeshExtractor::Extract(Mode.GetWorld(), Mode.GetGroundOrigin(), Bounds, MaxTriangles, Mesh);
	});

	if (!bOk)
	{
		Reply.Fail(TEXT("the world mesh could not be extracted"));
		return;
	}

	const auto Vertices = Reply.CopyStructs(reinterpret_cast<const ffw::Vec3f*>(Mesh.Vertices.GetData()), Mesh.Vertices.Num() / 3);
	const auto Indices  = Reply.CopyVector(Mesh.Indices.GetData(), Mesh.Indices.Num());
	Reply.Set(ffw::CreateGetWorldMeshResponse(Reply.Builder, Vertices, Indices, Mesh.bTruncated, Mesh.SourceCount));
}

void FSimServer::GetDistanceBatch(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::GetDistanceBatchRequest* Ask = Request.body_as_GetDistanceBatchRequest();

	TArray<float> Points;
	if (Ask->points())
	{
		Points.SetNumUninitialized(Ask->points()->size() * 3);
		FMemory::Memcpy(Points.GetData(), Ask->points()->Data(), Points.Num() * sizeof(float));
	}

	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	TArray<float> Distances;
	bool          bOk = false;
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) {
		bOk = FDistanceFieldQuery::Batch(Mode.GetWorld(), Mode.GetGroundOrigin(), Points, Ask->max_radius(), Ask->tolerance(), Ask->terrain_only(), Distances);
	});

	if (!bOk)
	{
		Reply.Fail(TEXT("get_distance_batch needs 1 to 2000000 points and a positive max_radius"));
		return;
	}

	Reply.Set(ffw::CreateGetDistanceBatchResponse(Reply.Builder, Reply.CopyVector(Distances.GetData(), Distances.Num())));
}

void FSimServer::GetSurfaceHeight(const ffw::Request& Request, FWireReply& Reply)
{
	constexpr uint32 MaxPoints = 100000;

	const ffw::GetSurfaceHeightRequest* Ask = Request.body_as_GetSurfaceHeightRequest();

	TArray<FVector> Points;
	if (Ask->points())
	{
		if (Ask->points()->size() > MaxPoints)
		{
			Reply.Fail(FString::Printf(TEXT("get_surface_height takes at most %u points, got %u"), MaxPoints, Ask->points()->size()));
			return;
		}
		Points.Reserve(Ask->points()->size());
		for (const ffw::Vec3* Point : *Ask->points())
		{
			Points.Add(FWireFrame::FromVec3(*Point));
		}
	}

	const auto* RobotIds = Ask->robot_ids();
	if (RobotIds && RobotIds->size() != static_cast<uint32>(Points.Num()))
	{
		Reply.Fail(FString::Printf(TEXT("get_surface_height takes one robot id per point, got %u ids for %d points"), RobotIds->size(), Points.Num()));
		return;
	}

	const FGameModeLease GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	std::vector<FRobotLease> Leases;
	TArray<const AActor*>    Own;
	if (RobotIds)
	{
		Own.Init(nullptr, Points.Num());
		TMap<int32, const AActor*> Leased;
		for (int32 i = 0; i < Points.Num(); ++i)
		{
			const int32 Id = RobotIds->Get(i);
			if (Id < 0)
			{
				continue;
			}
			if (const AActor** Known = Leased.Find(Id))
			{
				Own[i] = *Known;
				continue;
			}
			FRobotLease Robot = LeaseRobot(Id, Reply);
			if (!Robot)
			{
				return;
			}
			Own[i] = Leased.Add(Id, Robot.Pawn);
			Leases.push_back(MoveTemp(Robot));
		}
	}

	TArray<double> Heights;
	if (Points.Num() > 0)
	{
		const bool                   bIgnoreRobots = Ask->ignore_robots();
		const TArray<const AActor*>* PerPoint      = RobotIds ? &Own : nullptr;
		RunOnGameThread(*GameMode.GameMode, [&Points, &Heights, bIgnoreRobots, PerPoint](AFlightForgeGameModeBase& Mode) {
			FSurfaceHeightQuery::Batch(Mode.GetWorld(), Mode.GetGroundOrigin(), Points, bIgnoreRobots, Heights, PerPoint);
		});
		if (Heights.Num() != Points.Num())
		{
			Reply.Fail(TEXT("get_surface_height did not run on the game thread"));
			return;
		}
	}

	Reply.Set(ffw::CreateGetSurfaceHeightResponse(Reply.Builder, Reply.CopyVector(Heights.GetData(), Heights.Num())));
}

/* //} */
