#include "Lidar.h"

#include "Async/ParallelFor.h"
#include "CollisionQueryParams.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/HitResult.h"
#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "FlightForgeCore/DronePawn.h"
#include "Serialize.h"

ULidar::ULidar()
{
    SensorType = SensorType::LIDAR;
    Name = TEXT("Lidar");
    
	PrimaryComponentTick.bCanEverTick = false;

	LidarConfig.ShowBeams = DEFAULT_LIDAR_SHOW_BEAMS;

	LidarConfig.BeamHorRays  = DEFAULT_LIDAR_BEAM_HOR;
	LidarConfig.BeamVertRays = DEFAULT_LIDAR_BEAM_VER;
	LidarConfig.Frequency    = 10;

	LidarConfig.BeamLength  = DEFAULT_LIDAR_BEAM_LENGTH;
	LidarConfig.Offset      = FVector(0, 0, 0);
	LidarConfig.Orientation = FRotator(0, 0, 0);
	LidarConfig.FOVVertUp   = 52.0;
	LidarConfig.FOVVertDown = 7.0;
	LidarConfig.FOVHorLeft  = 180.0;
	LidarConfig.FOVHorRight = 180.0;
	LidarConfig.vertRayDiff = (double)(LidarConfig.FOVVertUp + LidarConfig.FOVVertDown) / (double)(LidarConfig.BeamVertRays - 1.0);
	LidarConfig.horRayDif   = (double)(LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight) / (double)LidarConfig.BeamHorRays;

	LidarConfig.Livox       = false;
	LidarConfig.LivoxSensor = TEXT("mid360");

	LidarHitsCriticalSection    = std::make_unique<FCriticalSection>();
	LidarSegHitsCriticalSection = std::make_unique<FCriticalSection>();
	LidarIntHitsCriticalSection = std::make_unique<FCriticalSection>();

	LidarHits     = std::make_unique<std::vector<std::tuple<double, double, double, double>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
	LidarSegHits  = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
	LidarIntHits  = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
	LidarHitStart = std::make_unique<FVector>();
}

/* BeginPlay() //{ */

void ULidar::BeginPlay()
{
	Super::BeginPlay();
}

//}

/* TickComponent() //{ */

void ULidar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//}

/* GetLidarConfig() //{ */

FLidarConfig ULidar::GetLidarConfig() {
    return LidarConfig;
}

//}

/* SetLidarConfig() //{ */

bool ULidar::SetLidarConfig(const FLidarConfig& Config) {

    LidarHitsCriticalSection->Lock();
    LidarSegHitsCriticalSection->Lock();

    const bool    LivoxWas       = LidarConfig.Livox;
    const FString LivoxSensorWas = LidarConfig.LivoxSensor;

    LidarConfig.Enable      = Config.Enable;
    LidarConfig.Livox       = Config.Livox;
    LidarConfig.LivoxSensor = Config.LivoxSensor;
    LidarConfig.ShowBeams   = Config.ShowBeams;
    LidarConfig.BeamLength  = Config.BeamLength;

    ResetLivoxCursorsIfChanged(LivoxWas, LivoxSensorWas);

    if (Config.BeamHorRays > 0 && Config.BeamVertRays > 0) {
        LidarConfig.BeamHorRays  = Config.BeamHorRays;
        LidarConfig.BeamVertRays = Config.BeamVertRays;
        UE_LOG(LogTemp, Warning, TEXT("Setting LiDAR with width = %d, height %d"), LidarConfig.BeamHorRays, LidarConfig.BeamVertRays);
    } else {
        UE_LOG(LogTemp, Error, TEXT("Invalid dimensions for Lidar. BeamHorRays and BeamVertRays should be greater than 0."));
    }

    LidarConfig.Frequency = Config.Frequency;

    LidarConfig.Offset.X = Config.Offset.X;
    LidarConfig.Offset.Y = Config.Offset.Y;
    LidarConfig.Offset.Z = Config.Offset.Z;

    LidarConfig.Orientation.Pitch = Config.Orientation.Pitch;
    LidarConfig.Orientation.Yaw   = -Config.Orientation.Yaw;
    LidarConfig.Orientation.Roll  = -Config.Orientation.Roll;

    LidarConfig.FOVVertUp   = Config.FOVVertUp;
    LidarConfig.FOVVertDown = Config.FOVVertDown;
    LidarConfig.FOVHorLeft  = Config.FOVHorLeft;
    LidarConfig.FOVHorRight = Config.FOVHorRight;
    LidarConfig.vertRayDiff = (double)(LidarConfig.FOVVertUp + LidarConfig.FOVVertDown) / (double)(LidarConfig.BeamVertRays - 1.0);
    LidarConfig.horRayDif   = (double)(LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight) / (double)(LidarConfig.BeamHorRays);

    LidarHits->resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
    LidarSegHits->resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
    LidarHitsCriticalSection->Unlock();
    LidarSegHitsCriticalSection->Unlock();

    return true;
}

//}


/* resetLivoxCursorsIfChanged() //{ */

// Switching sensor mid-flight would otherwise resume at a pattern index that
// belongs to the previous sensor's table.
void ULidar::ResetLivoxCursorsIfChanged(bool LivoxWas, const FString& LivoxSensorWas) {

    if (LidarConfig.Livox == LivoxWas && LidarConfig.LivoxSensor == LivoxSensorWas) {
        return;
    }

    LivoxCursor    = FLivoxCursor();
    LivoxSegCursor = FLivoxCursor();
    LivoxIntCursor = FLivoxCursor();

    if (LidarConfig.Livox) {
        // Fail loudly at configuration time rather than returning empty clouds.
        if (const FLivoxPattern* Pattern = GetLivoxPattern()) {
            if (Pattern->MaxRangeCm > 0.0 && LidarConfig.BeamLength <= DEFAULT_LIDAR_BEAM_LENGTH) {
                UE_LOG(LogTemp, Log, TEXT("Livox: '%s' reaches %.0f m, raising the beam from the %.0f m default"), *LidarConfig.LivoxSensor,
                       Pattern->MaxRangeCm / 100.0, LidarConfig.BeamLength / 100.0);
                LidarConfig.BeamLength = Pattern->MaxRangeCm;
            }

            UE_LOG(LogTemp, Log, TEXT("Livox: '%s' active, %.0f kHz -> %d points per message at %.1f Hz"), *LidarConfig.LivoxSensor,
                   Pattern->PointRate / 1000.0, FMath::RoundToInt(Pattern->PointRate / FMath::Max(LidarConfig.Frequency, 1.0)), LidarConfig.Frequency);
        } else {
            UE_LOG(LogTemp, Error, TEXT("Livox: no scan pattern named '%s', lidar will return nothing"), *LidarConfig.LivoxSensor);
        }
    }
}

//}

/* materialIntensity() //{ */

int ULidar::MaterialIntensity(const FString& PhysMatName) {

    if (PhysMatName.Equals("PM_Grass", ESearchCase::IgnoreCase)) {
        return 1;
    } else if (PhysMatName.Equals("PM_Road", ESearchCase::IgnoreCase)) {
        return 2;
    } else if (PhysMatName.Equals("PM_Tree", ESearchCase::IgnoreCase)) {
        return 3;
    } else if (PhysMatName.Equals("PM_Building", ESearchCase::IgnoreCase)) {
        return 4;
    } else if (PhysMatName.Equals("PM_Fence", ESearchCase::IgnoreCase)) {
        return 5;
    } else if (PhysMatName.Equals("PM_DirtRoad", ESearchCase::IgnoreCase)) {
        return 6;
    }

    return 0;
}

//}

/* getLivoxPattern() //{ */

const FLivoxPattern* ULidar::GetLivoxPattern() const {
    return FLivoxPatternLibrary::Get(LidarConfig.LivoxSensor);
}

//}

/* castLivoxRays() //{ */

// A stalled client would otherwise ask for an unbounded slice on its next poll;
// one second of Avia is already 240k points.
#define LIVOX_MAX_INTEGRATION_TIME 1.0

// The real bound: a slice spans at most this many frames, so a paused client resumes with
// a normal message rather than a lump. Two leaves room for one late poll.
#define LIVOX_MAX_INTEGRATION_FRAMES 2.0

// Rays are traced in chunks this long, each from the drone pose interpolated to
// the middle of the chunk. 1 ms is ~240 rays on Avia, fine enough that the
// residual within a chunk is far below the range noise of the real sensor.
#define LIVOX_SUB_BATCH_TIME 0.001

int32 ULidar::CastLivoxRays(FLivoxCursor& Cursor, TFunctionRef<void(int32)> Prepare,
                            TFunctionRef<void(int32, double, const FVector&, const FHitResult&, bool)> Store, bool bNeedPhysicalMaterial) {

    const FLivoxPattern* Pattern = GetLivoxPattern();

    if (!Pattern || Pattern->Num() == 0) {
        return 0;
    }

    auto World = Owner->GetWorld();

    if (!World) {
        return 0;
    }

    const double Now = World->GetTimeSeconds();

    // First slice after enabling or reconfiguring: hand out one nominal period so
    // the client gets a sane message instead of an empty or enormous one.
    if (Cursor.LastTime < 0.0) {
        const double NominalPeriod = 1.0 / FMath::Max(LidarConfig.Frequency, 1.0);
        Cursor.LastTime            = Now - NominalPeriod;

        // Two units on a rig are independent oscillators; starting every cursor at zero
        // walks them in lockstep. From the id, not random, so runs stay reproducible.
        const uint32 Scramble = static_cast<uint32>(GetSensorID()) * 2654435761u;
        Cursor.PatternPos     = static_cast<double>(Scramble % static_cast<uint32>(FMath::Max(1, Pattern->Num())));
    }

    double Elapsed = Now - Cursor.LastTime;

    // World time restarts on a level switch.
    if (Elapsed < 0.0) {
        Elapsed = 1.0 / FMath::Max(LidarConfig.Frequency, 1.0);
    }

    // Real hardware drops what nobody reads rather than banking it: a lump that large
    // smears the scan and takes longer to serve than the span it covers.
    const double MaxIntegration = FMath::Min(LIVOX_MAX_INTEGRATION_TIME, LIVOX_MAX_INTEGRATION_FRAMES / FMath::Max(LidarConfig.Frequency, 1.0));

    if (Elapsed > MaxIntegration) {
        UE_LOG(LogTemp, Warning, TEXT("Livox: %.2f s since the last request, integrating the most recent %.2f s and dropping %.2f s of scan"), Elapsed,
               MaxIntegration, Elapsed - MaxIntegration);

        // Skip the pattern over the dropped span too, so playback tracks wall clock.
        Cursor.PatternPos = FMath::Fmod(Cursor.PatternPos + (Elapsed - MaxIntegration) * Pattern->PointRate, (double)Pattern->Num());
        Elapsed           = MaxIntegration;
    }

    const int32 NumRays = FMath::Max(1, FMath::RoundToInt(Elapsed * Pattern->PointRate));

    Prepare(NumRays);

    const FTransform DroneTransform = Owner->GetActorTransform();

    const FQuat      LidarQuat     = DroneTransform.GetRotation() * LidarConfig.Orientation.Quaternion();
    const FVector    LidarLocation = DroneTransform.TransformPosition(LidarConfig.Offset);
    const FTransform RefTransform(LidarQuat, LidarLocation);

    // The slice covers [LastTime, Now]; the pose at each end is known, so poses
    // inside it are interpolated. Output is expressed in the pose at Now, which is
    // what the response stamp refers to.
    const FTransform& FromPose = Cursor.bPoseValid ? Cursor.LastPose : RefTransform;

    const int32  PatternStart = (int32)FMath::Fmod(Cursor.PatternPos, (double)Pattern->Num());
    const int32  RaysPerBatch = FMath::Max(1, FMath::RoundToInt(LIVOX_SUB_BATCH_TIME * Pattern->PointRate));
    const int32  NumBatches   = FMath::DivideAndRoundUp(NumRays, RaysPerBatch);
    const double MinRangeCm   = Pattern->MinRangeCm;

    // a Livox cannot see past its own spec, whatever the config asks for
    const double BeamLength = Pattern->MaxRangeCm > 0.0 ? FMath::Min(LidarConfig.BeamLength, Pattern->MaxRangeCm) : LidarConfig.BeamLength;

    ParallelFor(NumBatches, [&](int32 BatchIndex) {
        const double Alpha = NumBatches > 1 ? ((double)BatchIndex + 0.5) / (double)NumBatches : 1.0;

        const FTransform BatchPose(FQuat::Slerp(FromPose.GetRotation(), RefTransform.GetRotation(), Alpha),
                                   FMath::Lerp(FromPose.GetLocation(), RefTransform.GetLocation(), Alpha));

        const FVector BatchOrigin = BatchPose.GetLocation();

        const int32 First = BatchIndex * RaysPerBatch;
        const int32 Last  = FMath::Min(First + RaysPerBatch, NumRays);

        // Without this every beam hits the airframe first, and that hit is inside
        // MinRangeCm, so the ray is dropped and reported as no return at all.
        FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
        CollisionParams.AddIgnoredActor(Owner);

        // resolving a material per hit is wasted unless the seg/intensity modes read it
        CollisionParams.bReturnPhysicalMaterial = bNeedPhysicalMaterial;

        if (LidarConfig.ShowBeams) {
            CollisionParams.TraceTag = FName(TEXT("LivoxTraceTag"));
        }

        // Serial on purpose: the batches are already parallel, and nesting a ParallelFor
        // here cost 4x - the inner tasks contend over work units of ~200 rays.
        for (int32 i = First; i < Last; ++i) {

            const FVector3f& Dir3f = Pattern->Directions[(PatternStart + i) % Pattern->Num()];
            const FVector    PatternDir(Dir3f.X, Dir3f.Y, Dir3f.Z);

            const FVector RayWorld = BatchPose.TransformVector(PatternDir) * BeamLength;

            FVector OutDir      = FVector::ZeroVector;
            double  OutDistance = -1.0;
            bool    bHasReturn  = false;

            FHitResult HitResult;

            if (World->LineTraceSingleByChannel(HitResult, BatchOrigin, BatchOrigin + RayWorld, ECC_Visibility, CollisionParams) && HitResult.bBlockingHit) {

                AActor* HitActor = HitResult.GetActor();

                const bool bHiddenDrone = !Owner->GetVisibilityOtherDrones() && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != Owner;

                if (!bHiddenDrone && HitResult.Distance >= MinRangeCm) {
                    // Re-expressing the world hit in the reference frame is what bakes the
                    // intra-scan motion distortion into the geometry, while keeping the
                    // wire format (distance + direction) unchanged.
                    const FVector Local = RefTransform.InverseTransformPosition(HitResult.ImpactPoint);

                    OutDistance = Local.Size();
                    OutDir      = Local.GetSafeNormal() * BeamLength;
                    bHasReturn  = true;
                }
            }

            // only a no-return needs this; on a hit the direction comes from the impact
            if (!bHasReturn) {
                OutDir = RefTransform.InverseTransformVector(RayWorld);
            }

            Store(i, OutDistance, OutDir, HitResult, bHasReturn);
        }
    });

    Cursor.PatternPos = FMath::Fmod(Cursor.PatternPos + NumRays, (double)Pattern->Num());
    Cursor.LastTime   = Now;
    Cursor.LastPose   = RefTransform;
    Cursor.bPoseValid = true;

    LidarHitStart.reset(new FVector(LidarLocation));

    return NumRays;
}

//}

static TAutoConsoleVariable<int32> CVarLidarProfile(TEXT("FlightForge.Lidar.Profile"), 0,
                                                    TEXT("Log the cast/copy/serialize split of each lidar request."), ECVF_Default);

/* GetLidarHits() //{ */

void ULidar::GetLidarHits(std::vector<Serializable::Drone::GetLidarData::LidarData>& OutLidarData, FVector& OutStart, double& OutStamp) {


    LidarHitsCriticalSection->Lock();

    auto stamp = FPlatformTime::Seconds();

    const bool   bProfile = CVarLidarProfile.GetValueOnAnyThread() != 0;
    const double CastFrom = FPlatformTime::Seconds();

    UpdateLidar(true);

    const double CastTo = FPlatformTime::Seconds();

    OutLidarData.resize(LidarHits->size());

    for (size_t i = 0; i < LidarHits->size(); i++) {
        OutLidarData[i].distance   = std::get<0>((*LidarHits)[i]);
        OutLidarData[i].directionX = std::get<1>((*LidarHits)[i]);
        OutLidarData[i].directionY = std::get<2>((*LidarHits)[i]);
        OutLidarData[i].directionZ = std::get<3>((*LidarHits)[i]);
    }

    if (bProfile) {
        const double CopyTo = FPlatformTime::Seconds();
        UE_LOG(LogTemp, Warning, TEXT("Lidar profile: %d points, cast %.1f ms, copy %.1f ms"), static_cast<int32>(LidarHits->size()),
               1000.0 * (CastTo - CastFrom), 1000.0 * (CopyTo - CastTo));
    }

    OutStart.X = LidarConfig.Offset.X;
    OutStart.Y = LidarConfig.Offset.Y;
    OutStart.Z = LidarConfig.Offset.Z;

    OutStamp = stamp;

    LidarHitsCriticalSection->Unlock();
}

//}

/* UpdateLidar() //{ */

void ULidar::UpdateLidar(bool isExternallyLocked) {
    if (!isExternallyLocked) {
        LidarHitsCriticalSection->Lock();
    }

    auto World = Owner->GetWorld();
    const auto droneTransform = Owner->GetActorTransform();

    FRotator correctedLidarOrientation = LidarConfig.Orientation;
    if(!LidarConfig.Livox){
    correctedLidarOrientation.Roll = -correctedLidarOrientation.Roll;
    }

    FQuat lidarQuat = droneTransform.GetRotation() * correctedLidarOrientation.Quaternion();
    FVector lidarLocation = droneTransform.TransformPosition(LidarConfig.Offset);
    FTransform lidarGlobalTransform(lidarQuat, lidarLocation);


    if (LidarConfig.Livox) {

        LidarHits = std::make_unique<std::vector<std::tuple<double, double, double, double>>>();

        CastLivoxRays(
            LivoxCursor, [&](int32 NumRays) { LidarHits->resize(NumRays); },
            [&](int32 i, double Distance, const FVector& Dir, const FHitResult& Hit, bool bHasReturn) {
                (*LidarHits)[i] = std::make_tuple(Distance, Dir.X, Dir.Y, Dir.Z);
            });

        if (!isExternallyLocked) {
            LidarHitsCriticalSection->Unlock();
        }

        return;
    }

    LidarHits = std::make_unique<std::vector<std::tuple<double, double, double, double>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
    
    FVector forwardVec = lidarQuat.RotateVector(FVector::ForwardVector);
    FVector rightVector   = lidarQuat.RotateVector(FVector::RightVector);
    FVector upVec        = lidarQuat.RotateVector(FVector::UpVector);

    double totalHorFov = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
    double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

    ParallelFor(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays, [&](int32 rayIndex) {
        const int32 row = rayIndex / LidarConfig.BeamVertRays;
        const int32 col = rayIndex % LidarConfig.BeamVertRays;

        double  horAngle       = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
        FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
        FVector rotatedRight   = rightVector.RotateAngleAxis(horAngle, upVec);

        {
            FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
            CollisionParams.AddIgnoredActor(Owner);
            if (LidarConfig.ShowBeams) {
                const FName TraceTag(FString::Printf(TEXT("LidarTraceTag_%d"), 0));
                CollisionParams.TraceTag = TraceTag;
            }

            double vertAngle = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));

            FVector raycastAngle_world = rotatedForward.RotateAngleAxis(-vertAngle, rotatedRight);
            raycastAngle_world *= LidarConfig.BeamLength;

            FHitResult HitResult;
            int i = row * LidarConfig.BeamVertRays + col;

            if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + raycastAngle_world, ECC_Visibility, CollisionParams))
            {
                FVector ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle_world);

                 if (HitResult.bBlockingHit) {
                    AActor* HitActor = HitResult.GetActor();
                    if (!Owner->GetVisibilityOtherDrones() && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != Owner)
                    {
                       (*LidarHits)[i] = std::make_tuple(HitResult.Distance, ray_in_lidar_coord.X, ray_in_lidar_coord.Y, ray_in_lidar_coord.Z);
                    } else {
                        (*LidarHits)[i] = std::make_tuple(HitResult.Distance, ray_in_lidar_coord.X, ray_in_lidar_coord.Y, ray_in_lidar_coord.Z);
                    }
                } else {
                    (*LidarHits)[i] = std::make_tuple(-1.0, ray_in_lidar_coord.X, ray_in_lidar_coord.Y, ray_in_lidar_coord.Z);
                }
            }
            else {
                FVector ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle_world);
                (*LidarHits)[i] = std::make_tuple(-1.0, ray_in_lidar_coord.X, ray_in_lidar_coord.Y, ray_in_lidar_coord.Z); 
            }
        }
    });
    LidarHitStart.reset(new FVector(lidarLocation));
    

    if (!isExternallyLocked) {
        LidarHitsCriticalSection->Unlock();
    }
}

//}

/* GetSegLidarHits() //{ */

void ULidar::GetSegLidarHits(std::vector<Serializable::Drone::GetLidarSegData::LidarSegData>& OutLidarSegData, FVector& OutStart, double& OutStamp) {

    LidarSegHitsCriticalSection->Lock();

    auto stamp = FPlatformTime::Seconds();

    UpdateSegLidar(true);

    OutLidarSegData.resize(LidarSegHits->size());

    for (size_t i = 0; i < LidarSegHits->size(); i++) {
        OutLidarSegData[i].distance     = std::get<0>((*LidarSegHits)[i]);
        OutLidarSegData[i].directionX   = std::get<1>((*LidarSegHits)[i]);
        OutLidarSegData[i].directionY   = std::get<2>((*LidarSegHits)[i]);
        OutLidarSegData[i].directionZ   = std::get<3>((*LidarSegHits)[i]);
        OutLidarSegData[i].segmentation = std::get<4>((*LidarSegHits)[i]);
    }

    OutStart.X = LidarConfig.Offset.X;
    OutStart.Y = LidarConfig.Offset.Y;
    OutStart.Z = LidarConfig.Offset.Z;

    OutStamp = stamp;

    LidarSegHitsCriticalSection->Unlock();
}

//}

/* UpdateSegLidar() //{ */

void ULidar::UpdateSegLidar(bool isExternallyLocked) {

  if (!isExternallyLocked) {
    LidarSegHitsCriticalSection->Lock();
  }

  auto       World          = Owner->GetWorld();
  const auto droneTransform = Owner->GetActorTransform();

    // --- Invert Roll to match ROS conventions ---
    FRotator correctedLidarOrientation = LidarConfig.Orientation;
    if(!LidarConfig.Livox){
        correctedLidarOrientation.Roll = -correctedLidarOrientation.Roll;
    }

    // Compose the lidar's global transform
    FQuat lidarQuat = droneTransform.GetRotation() * correctedLidarOrientation.Quaternion();
    FVector lidarLocation = droneTransform.TransformPosition(LidarConfig.Offset);
    FTransform lidarGlobalTransform(lidarQuat, lidarLocation);


    if (LidarConfig.Livox) {

        LidarSegHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>();

        CastLivoxRays(
            LivoxSegCursor, [&](int32 NumRays) { LidarSegHits->resize(NumRays); },
            [&](int32 i, double Distance, const FVector& Dir, const FHitResult& Hit, bool bHasReturn) {
                int segmentation = -1;

                if (bHasReturn && Hit.GetComponent()) {
                    if (Hit.GetComponent()->GetName().Contains("Landscape")) {
                        segmentation = 1;
                    } else if (auto component = Cast<UStaticMeshComponent>(Hit.GetComponent())) {
                        segmentation = component->CustomDepthStencilValue;
                    }
                }

                (*LidarSegHits)[i] = std::make_tuple(Distance, Dir.X, Dir.Y, Dir.Z, segmentation);
            });

        if (!isExternallyLocked) {
            LidarSegHitsCriticalSection->Unlock();
        }

        return;
    }

    LidarSegHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
    
    // Derive the lidar's world-space axes (these are now consistent)
    FVector forwardVec = lidarQuat.RotateVector(FVector::ForwardVector);
    FVector rightVector   = lidarQuat.RotateVector(FVector::RightVector);
    FVector upVec        = lidarQuat.RotateVector(FVector::UpVector);


    // Calculate total horizontal FOV and vertical FOV
    double totalHorFov  = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
    double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

    ParallelFor(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays, [&](int32 rayIndex) {
        const int32 row = rayIndex / LidarConfig.BeamVertRays;
        const int32 col = rayIndex % LidarConfig.BeamVertRays;

        double  horAngle       = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
        FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
        FVector rotatedRight   = rightVector.RotateAngleAxis(horAngle, upVec);

        {
        FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
        CollisionParams.AddIgnoredActor(Owner);
        // Calculate the vertical angle for the current ray
        double vertAngle = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));

        // Calculate the direction of the ray
        FVector raycastAngle = rotatedForward.RotateAngleAxis(-vertAngle, rotatedRight); // Note the -vertAngle
        raycastAngle *= LidarConfig.BeamLength;

        FHitResult HitResult;
        int i = row * LidarConfig.BeamVertRays + col;

        if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + raycastAngle, ECollisionChannel::ECC_Visibility, CollisionParams)) {

            if (HitResult.bBlockingHit) {

            AActor* HitActor = HitResult.GetActor();
            if (!Owner->GetVisibilityOtherDrones() && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != Owner)
            {
                // skip
                std::get<0>((*LidarSegHits)[i]) = -1;
            } else {
                std::get<0>((*LidarSegHits)[i]) = HitResult.IsValidBlockingHit() ? HitResult.Distance : LidarConfig.BeamLength;
            }

            // Transform ray into Lidar coordinates
            const auto ray_in_lidar_coord   = lidarGlobalTransform.InverseTransformVector(raycastAngle);

            std::get<1>((*LidarSegHits)[i]) = ray_in_lidar_coord.X;
            std::get<2>((*LidarSegHits)[i]) = ray_in_lidar_coord.Y;
            std::get<3>((*LidarSegHits)[i]) = ray_in_lidar_coord.Z;

            auto component = Cast<UStaticMeshComponent>(HitResult.GetComponent());

            if (HitResult.GetComponent()->GetName().Contains("Landscape")) {
                std::get<4>((*LidarSegHits)[i]) = 1;
            } else {
                std::get<4>((*LidarSegHits)[i]) = component->CustomDepthStencilValue;
            }

            } else {

            std::get<0>((*LidarSegHits)[i]) = -1;
                // Transform ray into Lidar coordinates
            const auto ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle);

            std::get<1>((*LidarSegHits)[i]) = ray_in_lidar_coord.X;
            std::get<2>((*LidarSegHits)[i]) = ray_in_lidar_coord.Y;
            std::get<3>((*LidarSegHits)[i]) = ray_in_lidar_coord.Z;
            std::get<4>((*LidarSegHits)[i]) = -1;
            }
        }
            //added for consistency, copied from updatelidar
            else {
                std::get<0>((*LidarSegHits)[i]) = -1;

                // Transform the ray direction into the lidar's local coordinate frame.
                FVector ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle);
                std::get<1>((*LidarSegHits)[i]) = ray_in_lidar_coord.X;
                std::get<2>((*LidarSegHits)[i]) = ray_in_lidar_coord.Y;
                std::get<3>((*LidarSegHits)[i]) = ray_in_lidar_coord.Z;
                std::get<4>((*LidarSegHits)[i]) = -1;  // Important: Set segmentation to -1 for no hit
            }
        }
    });

  LidarHitStart.reset(new FVector(lidarLocation));

  if (!isExternallyLocked) {
    LidarSegHitsCriticalSection->Unlock();
  }
}

//}

/* GetIntLidarHits() //{ */

void ULidar::GetIntLidarHits(std::vector<Serializable::Drone::GetLidarIntData::LidarIntData>& OutLidarIntData, FVector& OutStart, double& OutStamp) {

    LidarIntHitsCriticalSection->Lock();

    auto stamp = FPlatformTime::Seconds();

    UpdateIntLidar(true);

    OutLidarIntData.resize(LidarIntHits->size());

    for (size_t i = 0; i < LidarIntHits->size(); i++) {
        OutLidarIntData[i].distance   = std::get<0>((*LidarIntHits)[i]);
        OutLidarIntData[i].directionX = std::get<1>((*LidarIntHits)[i]);
        OutLidarIntData[i].directionY = std::get<2>((*LidarIntHits)[i]);
        OutLidarIntData[i].directionZ = std::get<3>((*LidarIntHits)[i]);
        OutLidarIntData[i].intensity  = std::get<4>((*LidarIntHits)[i]);
    }

    OutStart.X = LidarConfig.Offset.X;
    OutStart.Y = LidarConfig.Offset.Y;
    OutStart.Z = LidarConfig.Offset.Z;

    OutStamp = stamp;

    LidarIntHitsCriticalSection->Unlock();
}

//}

/* UpdateIntLidar() //{ */

void ULidar::UpdateIntLidar(bool isExternallyLocked) {

  if (!isExternallyLocked) {
    LidarIntHitsCriticalSection->Lock();
  }

  auto       World          = Owner->GetWorld();
  const auto droneTransform = Owner->GetActorTransform();

     // --- Invert Roll to match ROS conventions ---
    FRotator correctedLidarOrientation = LidarConfig.Orientation;
     if(!LidarConfig.Livox){
        correctedLidarOrientation.Roll = -correctedLidarOrientation.Roll;
    }
    // Compose the lidar's global transform: first apply the drone's rotation, then the corrected lidar orientation,
    // and finally translate by the lidar offset.
    FQuat lidarQuat = droneTransform.GetRotation() * correctedLidarOrientation.Quaternion();
    FVector lidarLocation = droneTransform.TransformPosition(LidarConfig.Offset);
    FTransform lidarGlobalTransform(lidarQuat, lidarLocation);

  if (LidarConfig.Livox) {

    LidarIntHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>();

    CastLivoxRays(
        LivoxIntCursor, [&](int32 NumRays) { LidarIntHits->resize(NumRays); },
        [&](int32 i, double Distance, const FVector& Dir, const FHitResult& Hit, bool bHasReturn) {
            int intensity = -1;

            if (bHasReturn) {
                if (UPhysicalMaterial* PhysMat = Hit.PhysMaterial.Get()) {
                    intensity = MaterialIntensity(PhysMat->GetName());
                }
            }

            (*LidarIntHits)[i] = std::make_tuple(Distance, Dir.X, Dir.Y, Dir.Z, intensity);
        },
        /* bNeedPhysicalMaterial */ true);

    if (!isExternallyLocked) {
      LidarIntHitsCriticalSection->Unlock();
    }

    return;
  }

  LidarIntHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

    // Derive the lidar's world-space axes
    FVector forwardVec = lidarQuat.RotateVector(FVector::ForwardVector);
    FVector rightVector   = lidarQuat.RotateVector(FVector::RightVector);
    FVector upVec        = lidarQuat.RotateVector(FVector::UpVector);



    // Calculate total horizontal FOV and vertical FOV
    double totalHorFov  = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
    double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

    ParallelFor(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays, [&](int32 rayIndex) {
        const int32 row = rayIndex / LidarConfig.BeamVertRays;
        const int32 col = rayIndex % LidarConfig.BeamVertRays;

        double  horAngle       = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
        FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
        FVector rotatedRight   = rightVector.RotateAngleAxis(horAngle, upVec);

        {
        FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
        CollisionParams.AddIgnoredActor(Owner);
        CollisionParams.bReturnPhysicalMaterial = true;

        // Calculate the vertical angle for the current ray
        double vertAngle = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));

        // Calculate the direction of the ray
        FVector raycastAngle = rotatedForward.RotateAngleAxis(-vertAngle, rotatedRight); // Note the -vertAngle
        raycastAngle *= LidarConfig.BeamLength;


        FHitResult HitResult;
            int i = row * LidarConfig.BeamVertRays + col;

        if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + raycastAngle, ECollisionChannel::ECC_Visibility, CollisionParams)) {
            if (HitResult.bBlockingHit) {

            AActor* HitActor = HitResult.GetActor();
            if (!Owner->GetVisibilityOtherDrones() && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != Owner)
            {
                // skip
                std::get<0>((*LidarHits)[i]) = -1;
            } else {
                std::get<0>((*LidarIntHits)[i]) = HitResult.IsValidBlockingHit() ? HitResult.Distance : LidarConfig.BeamLength;
            }
            
            // Transform ray into Lidar coordinates
            const auto ray_in_lidar_coord   = lidarGlobalTransform.InverseTransformVector(raycastAngle);

            std::get<1>((*LidarIntHits)[i]) = ray_in_lidar_coord.X;
            std::get<2>((*LidarIntHits)[i]) = ray_in_lidar_coord.Y;
            std::get<3>((*LidarIntHits)[i]) = ray_in_lidar_coord.Z;

            UPhysicalMaterial* PhysMat   = HitResult.PhysMaterial.Get();
            double             intensity = PhysMat ? MaterialIntensity(PhysMat->GetName()) : -1;

            std::get<4>((*LidarIntHits)[i]) = intensity;
            } else {

            std::get<0>((*LidarIntHits)[i]) = -1;
                // Transform ray into Lidar coordinates
            const auto ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle);

            std::get<1>((*LidarIntHits)[i]) = ray_in_lidar_coord.X;
            std::get<2>((*LidarIntHits)[i]) = ray_in_lidar_coord.Y;
            std::get<3>((*LidarIntHits)[i]) = ray_in_lidar_coord.Z;
            std::get<4>((*LidarIntHits)[i]) = -1;
            }
        }
            //added for consistency, copied from updatelidar
            else {
                std::get<0>((*LidarIntHits)[i]) = -1;

                // Transform the ray direction into the lidar's local coordinate frame.
                FVector ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle);
                std::get<1>((*LidarIntHits)[i]) = ray_in_lidar_coord.X;
                std::get<2>((*LidarIntHits)[i]) = ray_in_lidar_coord.Y;
                std::get<3>((*LidarIntHits)[i]) = ray_in_lidar_coord.Z;
                std::get<4>((*LidarIntHits)[i]) = -1;  // Set intensity -1 for no hit.
            }
        }
    });

  LidarHitStart.reset(new FVector(lidarLocation));

  if (!isExternallyLocked) {
    LidarIntHitsCriticalSection->Unlock();
  }
}

//}

/* SetLidarModeWithInt() //{ */

void ULidar::SetLidarModeWithInt(int type)
{
    if (type < 0 || type >= static_cast<int>(FLidarMode::MAX_MODE_TYPE))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid mode type"));
        return;
    }
    
    FLidarMode NewLidarMode = static_cast<FLidarMode>(type);
    LidarMode = NewLidarMode;
}

//}

/* GetConfig() //{ */

void ULidar::GetConfig(std::stringstream& OutputStream)
{
    Serializable::Drone::GetLidarConfig::Response Response(true);
    Response.config = Serializable::Drone::LidarConfig{};

    Response.config.Enable        = LidarConfig.Enable;
    Response.config.ShowBeams     = LidarConfig.ShowBeams;
    Response.config.BeamLength    = LidarConfig.BeamLength;
    Response.config.BeamHorRays   = LidarConfig.BeamHorRays;
    Response.config.BeamVertRays  = LidarConfig.BeamVertRays;
    Response.config.Frequency     = LidarConfig.Frequency;
    Response.config.OffsetX       = LidarConfig.Offset.X;
    Response.config.OffsetY       = LidarConfig.Offset.Y;
    Response.config.OffsetZ       = LidarConfig.Offset.Z;
    Response.config.OrientationPitch = LidarConfig.Orientation.Pitch;
    Response.config.OrientationYaw   = LidarConfig.Orientation.Yaw;
    Response.config.OrientationRoll  = LidarConfig.Orientation.Roll;
    Response.config.FOVHorLeft       = LidarConfig.FOVHorLeft;
    Response.config.FOVHorRight      = LidarConfig.FOVHorRight;
    Response.config.FOVVertUp        = LidarConfig.FOVVertUp;
    Response.config.FOVVertDown      = LidarConfig.FOVVertDown;
    Response.config.Livox            = LidarConfig.Livox;
    Response.config.LivoxSensor      = TCHAR_TO_UTF8(*LidarConfig.LivoxSensor);

    Serialization::DeserializeResponse(Response, OutputStream);

    UE_LOG(LogTemp, Log, TEXT("Got lidar config"));
}

//}

/* SetConfig() //{ */

void ULidar::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
{
    Serializable::Drone::SetLidarConfig::Request Request;
    Serialization::SerializeRequest(Request, *InputStream);

    bool Status;
    
    LidarHitsCriticalSection->Lock();
    LidarSegHitsCriticalSection->Lock();

    const bool    LivoxWas       = LidarConfig.Livox;
    const FString LivoxSensorWas = LidarConfig.LivoxSensor;

    LidarConfig.Enable      = Request.config.Enable;
    LidarConfig.Livox       = Request.config.Livox;
    LidarConfig.LivoxSensor = FString(UTF8_TO_TCHAR(Request.config.LivoxSensor.c_str()));
    LidarConfig.ShowBeams   = Request.config.ShowBeams;
    LidarConfig.BeamLength  = Request.config.BeamLength;

    ResetLivoxCursorsIfChanged(LivoxWas, LivoxSensorWas);

    if (Request.config.BeamHorRays > 0 && Request.config.BeamVertRays > 0) {
        LidarConfig.BeamHorRays  = Request.config.BeamHorRays;
        LidarConfig.BeamVertRays = Request.config.BeamVertRays;
        UE_LOG(LogTemp, Warning, TEXT("Setting LiDAR with width = %d, height %d"), LidarConfig.BeamHorRays, LidarConfig.BeamVertRays);
    } else {
        UE_LOG(LogTemp, Error, TEXT("Invalid dimensions for Lidar. BeamHorRays and BeamVertRays should be greater than 0."));
    }

    LidarConfig.Frequency = Request.config.Frequency;
    
    LidarConfig.Offset = FVector(Request.config.OffsetX, Request.config.OffsetY, Request.config.OffsetZ);
    
    // LidarConfig.Offset.X = Config.Offset.X;
    // LidarConfig.Offset.Y = Config.Offset.Y;
    // LidarConfig.Offset.Z = Config.Offset.Z;

    // LidarConfig.Orientation.Pitch = Config.Orientation.Pitch;
    // LidarConfig.Orientation.Yaw   = -Config.Orientation.Yaw;
    // LidarConfig.Orientation.Roll  = -Config.Orientation.Roll;

    LidarConfig.Orientation = FRotator(Request.config.OrientationPitch, Request.config.OrientationYaw, Request.config.OrientationRoll);

    /* LidarConfig.FOVHor  = Config.FOVHor; */
    /* LidarConfig.FOVVert = Config.FOVVert; */
    LidarConfig.FOVVertUp   = Request.config.FOVVertUp;
    LidarConfig.FOVVertDown = Request.config.FOVVertDown;
    LidarConfig.FOVHorLeft  = Request.config.FOVHorLeft;
    LidarConfig.FOVHorRight = Request.config.FOVHorRight;
    
    LidarConfig.vertRayDiff = (double)(LidarConfig.FOVVertUp + LidarConfig.FOVVertDown) / (double)(LidarConfig.BeamVertRays - 1.0);
    LidarConfig.horRayDif   = (double)(LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight) / (double)(LidarConfig.BeamHorRays);

    LidarHits->resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
    LidarSegHits->resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
    
    LidarHitsCriticalSection->Unlock();
    LidarSegHitsCriticalSection->Unlock();

    Status = true;
    
    
    Serializable::Drone::SetLidarConfig::Response Response(Status);
    Serialization::DeserializeResponse(Response, OutputStream);
}

//}

/* GetData() //{ */

void ULidar::GetData(std::stringstream& OutputStream)
{
    switch (LidarMode)
    {
        case FLidarMode::DEFAULT:
        {
            std::vector<Serializable::Drone::GetLidarData::LidarData> LidarData;
            FVector Start;
            double stamp;
            GetLidarHits(LidarData, Start, stamp);

            const bool   bProfile     = CVarLidarProfile.GetValueOnAnyThread() != 0;
            const double MarshalFrom  = FPlatformTime::Seconds();

            Serializable::Drone::GetLidarData::Response Response(true);
            Response.lidarData = std::move(LidarData);
            Response.startX = Start.X;
            Response.startY = Start.Y;
            Response.startZ = Start.Z;
            Response.stamp_ = stamp;

            const double SerializeFrom = FPlatformTime::Seconds();

            Serialization::DeserializeResponse(Response, OutputStream);

            if (bProfile) {
                const double Done = FPlatformTime::Seconds();
                UE_LOG(LogTemp, Warning, TEXT("Lidar profile: move %.1f ms, serialize %.1f ms"), 1000.0 * (SerializeFrom - MarshalFrom),
                       1000.0 * (Done - SerializeFrom));
            }
            break;
        }
        
        case FLidarMode::SEG:
        {
            std::vector<Serializable::Drone::GetLidarSegData::LidarSegData> LidarSegData;
            FVector Start;
            double stamp;
            GetSegLidarHits(LidarSegData, Start, stamp);

            Serializable::Drone::GetLidarSegData::Response Response(true);
            Response.lidarSegData = LidarSegData;
            Response.startX = Start.X;
            Response.startY = Start.Y;
            Response.startZ = Start.Z;
            Response.stamp_ = stamp;
        
            Serialization::DeserializeResponse(Response, OutputStream);
            break;
        }
        
        case FLidarMode::INTE:
        {
            std::vector<Serializable::Drone::GetLidarIntData::LidarIntData> LidarIntData;
            FVector Start;
            double stamp;
            GetIntLidarHits(LidarIntData, Start, stamp);

            Serializable::Drone::GetLidarIntData::Response Response(true);
            Response.lidarIntData = LidarIntData;
            Response.startX = Start.X;
            Response.startY = Start.Y;
            Response.startZ = Start.Z;
            Response.stamp_ = stamp;
        
            Serialization::DeserializeResponse(Response, OutputStream);
            break;
        }

        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid Lidar Mode"));
            break;
    }
}

//}
