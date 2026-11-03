#include "Lidar.h"

#include "Async/ParallelFor.h"
#include "CollisionQueryParams.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/HitResult.h"
#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "FlightForgeCore/DronePawn.h"
#include "FlightForgeCore/Segmentation/SegmentationTagger.h"
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

	LidarHitsCriticalSection = std::make_unique<FCriticalSection>();
}

/* BeginPlay() //{ */

void ULidar::BeginPlay()
{
	Super::BeginPlay();
}

//}

/* IgnoreOwnBody() //{ */

// A real lidar sees the airframe it is bolted to, and the self-occlusion that produces
// is part of what a rig has to be designed around. Only the sensor's own housing is
// skipped, because a beam cannot be blocked by the aperture it leaves from.
void ULidar::IgnoreOwnBody(FCollisionQueryParams& Params) const
{
	if (const UPrimitiveComponent* Body = GetSensorBody())
	{
		Params.AddIgnoredComponent(Body);
	}
	else
	{
		Params.AddIgnoredActor(Owner);
	}
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

    const bool    LivoxWas       = LidarConfig.Livox;
    const FString LivoxSensorWas = LidarConfig.LivoxSensor;

    LidarConfig.Enable      = Config.Enable;
    LidarConfig.Livox       = Config.Livox;
    LidarConfig.LivoxSensor = Config.LivoxSensor;
    LidarConfig.ShowBeams      = Config.ShowBeams;
    LidarConfig.BeamLength     = Config.BeamLength;
    LidarConfig.ShowSensorBody = Config.ShowSensorBody;

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

    LidarHitsCriticalSection->Unlock();

    // a puck roughly the size of a mid-360, at the mount the beams actually leave from
    UpdateSensorBody(LidarConfig.ShowSensorBody, LidarConfig.Offset, LidarConfig.Orientation, FVector(0.10, 0.10, 0.07));

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

// floor for fast polls, which a slow render rate bunches
#define LIVOX_MIN_INTEGRATION_TIME 0.1

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
    const double MaxIntegration =
        FMath::Min(LIVOX_MAX_INTEGRATION_TIME, FMath::Max(LIVOX_MIN_INTEGRATION_TIME, LIVOX_MAX_INTEGRATION_FRAMES / FMath::Max(LidarConfig.Frequency, 1.0)));

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
        IgnoreOwnBody(CollisionParams);

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

    return NumRays;
}

//}

static TAutoConsoleVariable<int32> CVarLidarProfile(TEXT("FlightForge.Lidar.Profile"), 0,
                                                    TEXT("Log the cast time of each lidar request."), ECVF_Default);

/* Scan() //{ */

// One trace fills every channel asked for; the three legacy messages are views of it.
void ULidar::Scan(FLivoxCursor& Cursor, const uint8 Channels, FLidarScan& Out) {

    LidarHitsCriticalSection->Lock();

    const bool   bWantIntensity = (Channels & LidarChannelIntensity) != 0;
    const bool   bWantLabels    = (Channels & LidarChannelLabels) != 0;
    const bool   bProfile       = CVarLidarProfile.GetValueOnAnyThread() != 0;
    const double CastFrom       = FPlatformTime::Seconds();

    Out.Stamp = CastFrom;
    Out.Start = LidarConfig.Offset;

    auto       World          = Owner->GetWorld();
    const auto droneTransform = Owner->GetActorTransform();

    // ROS roll convention for the raster lidar; the Livox patterns already carry it
    FRotator correctedLidarOrientation = LidarConfig.Orientation;
    if (!LidarConfig.Livox) {
        correctedLidarOrientation.Roll = -correctedLidarOrientation.Roll;
    }

    const FQuat      lidarQuat     = droneTransform.GetRotation() * correctedLidarOrientation.Quaternion();
    const FVector    lidarLocation = droneTransform.TransformPosition(LidarConfig.Offset);
    const FTransform lidarGlobalTransform(lidarQuat, lidarLocation);

    // hits become ids and classes after the parallel loop; the tagger is not for worker threads
    std::vector<const AActor*>             HitActors;
    std::vector<const UPrimitiveComponent*> HitComponents;

    auto Prepare = [&](int32 NumRays) {
        Out.Points.assign(NumRays, Serializable::Drone::GetLidarData::LidarData{});
        Out.Intensity.assign(bWantIntensity ? NumRays : 0, -1);
        Out.ClassId.assign(bWantLabels ? NumRays : 0, -1);
        Out.InstanceId.assign(bWantLabels ? NumRays : 0, 0u);
        HitActors.assign(bWantLabels ? NumRays : 0, nullptr);
        HitComponents.assign(bWantLabels ? NumRays : 0, nullptr);
    };

    auto Store = [&](int32 i, double Distance, const FVector& Dir, const FHitResult& Hit, bool bHasReturn) {
        Out.Points[i].distance   = Distance;
        Out.Points[i].directionX = Dir.X;
        Out.Points[i].directionY = Dir.Y;
        Out.Points[i].directionZ = Dir.Z;

        if (!bHasReturn) {
            return;
        }
        if (bWantIntensity) {
            const UPhysicalMaterial* PhysMat = Hit.PhysMaterial.Get();
            Out.Intensity[i]                 = PhysMat ? MaterialIntensity(PhysMat->GetName()) : -1;
        }
        if (bWantLabels) {
            HitActors[i]     = Hit.GetActor();
            HitComponents[i] = Hit.GetComponent();
        }
    };

    if (LidarConfig.Livox) {
        CastLivoxRays(Cursor, Prepare, Store, bWantIntensity);
    } else {
        const int32 NumRays = LidarConfig.BeamHorRays * LidarConfig.BeamVertRays;
        Prepare(NumRays);

        const FVector forwardVec  = lidarQuat.RotateVector(FVector::ForwardVector);
        const FVector rightVector = lidarQuat.RotateVector(FVector::RightVector);
        const FVector upVec       = lidarQuat.RotateVector(FVector::UpVector);

        const double totalHorFov  = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
        const double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

        ParallelFor(NumRays, [&](int32 rayIndex) {
            const int32 row = rayIndex / LidarConfig.BeamVertRays;
            const int32 col = rayIndex % LidarConfig.BeamVertRays;

            const double  horAngle       = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
            const FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
            const FVector rotatedRight   = rightVector.RotateAngleAxis(horAngle, upVec);

            FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
            IgnoreOwnBody(CollisionParams);
            CollisionParams.bReturnPhysicalMaterial = bWantIntensity;
            if (LidarConfig.ShowBeams) {
                CollisionParams.TraceTag = FName(TEXT("LidarTraceTag_0"));
            }

            const double  vertAngle    = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));
            const FVector raycastWorld = rotatedForward.RotateAngleAxis(-vertAngle, rotatedRight) * LidarConfig.BeamLength;
            const FVector raycastLidar = lidarGlobalTransform.InverseTransformVector(raycastWorld);

            FHitResult HitResult;
            bool       bHasReturn = false;
            double     Distance   = -1.0;

            if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + raycastWorld, ECC_Visibility, CollisionParams) &&
                HitResult.bBlockingHit) {
                const AActor* HitActor     = HitResult.GetActor();
                const bool    bHiddenDrone = !Owner->GetVisibilityOtherDrones() && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != Owner;
                if (!bHiddenDrone) {
                    bHasReturn = true;
                    Distance   = HitResult.Distance;
                }
            }

            Store(rayIndex, Distance, raycastLidar, HitResult, bHasReturn);
        });
    }

    if (bWantLabels) {
        if (USegmentationTagger* Tagger = World ? World->GetSubsystem<USegmentationTagger>() : nullptr) {
            TMap<const UPrimitiveComponent*, int32> ClassCache;  // the rules are wildcard matches, once per component not per point
            for (size_t i = 0; i < HitActors.size(); ++i) {
                if (!HitActors[i]) {
                    continue;
                }
                Out.InstanceId[i] = Tagger->GetOrAssignInstanceId(HitActors[i]);
                if (const int32* Cached = ClassCache.Find(HitComponents[i])) {
                    Out.ClassId[i] = *Cached;
                    continue;
                }
                // the same class the instance camera paints; landscape has no mesh asset, so the legacy 1 stays
                const int32 Class = Tagger->ClassOf(HitActors[i], HitComponents[i]);
                Out.ClassId[i]    = Class > 0 ? Class : (HitComponents[i] && HitComponents[i]->GetName().Contains("Landscape") ? 1 : 0);
                ClassCache.Add(HitComponents[i], Out.ClassId[i]);
            }
        }
    }

    if (bProfile) {
        UE_LOG(LogTemp, Warning, TEXT("Lidar profile: %d points, channels %d, cast %.1f ms"), static_cast<int32>(Out.Points.size()), Channels,
               1000.0 * (FPlatformTime::Seconds() - CastFrom));
    }

    LidarHitsCriticalSection->Unlock();
}

//}

/* legacy per-channel messages //{ */

void ULidar::GetLidarHits(std::vector<Serializable::Drone::GetLidarData::LidarData>& OutLidarData, FVector& OutStart, double& OutStamp) {
    FLidarScan S;
    Scan(LivoxCursor, LidarChannelNone, S);
    OutLidarData = std::move(S.Points);
    OutStart     = S.Start;
    OutStamp     = S.Stamp;
}

void ULidar::GetSegLidarHits(std::vector<Serializable::Drone::GetLidarSegData::LidarSegData>& OutLidarSegData, FVector& OutStart, double& OutStamp) {
    FLidarScan S;
    Scan(LivoxSegCursor, LidarChannelLabels, S);
    OutLidarSegData.resize(S.Points.size());
    for (size_t i = 0; i < S.Points.size(); ++i) {
        OutLidarSegData[i].distance     = S.Points[i].distance;
        OutLidarSegData[i].directionX   = S.Points[i].directionX;
        OutLidarSegData[i].directionY   = S.Points[i].directionY;
        OutLidarSegData[i].directionZ   = S.Points[i].directionZ;
        OutLidarSegData[i].segmentation = S.ClassId[i];
    }
    OutStart = S.Start;
    OutStamp = S.Stamp;
}

void ULidar::GetIntLidarHits(std::vector<Serializable::Drone::GetLidarIntData::LidarIntData>& OutLidarIntData, FVector& OutStart, double& OutStamp) {
    FLidarScan S;
    Scan(LivoxIntCursor, LidarChannelIntensity, S);
    OutLidarIntData.resize(S.Points.size());
    for (size_t i = 0; i < S.Points.size(); ++i) {
        OutLidarIntData[i].distance   = S.Points[i].distance;
        OutLidarIntData[i].directionX = S.Points[i].directionX;
        OutLidarIntData[i].directionY = S.Points[i].directionY;
        OutLidarIntData[i].directionZ = S.Points[i].directionZ;
        OutLidarIntData[i].intensity  = S.Intensity[i];
    }
    OutStart = S.Start;
    OutStamp = S.Stamp;
}

//}

/* GetScan() //{ */

void ULidar::GetScan(const Serializable::Drone::GetLidarScan::Request& Request, std::stringstream& OutputStream) {
    FLidarScan S;
    Scan(LivoxCursor, (Request.with_intensity_ ? LidarChannelIntensity : 0) | (Request.with_labels_ ? LidarChannelLabels : 0), S);

    Serializable::Drone::GetLidarScan::Response Response(true);
    Response.startX       = S.Start.X;
    Response.startY       = S.Start.Y;
    Response.startZ       = S.Start.Z;
    Response.stamp_       = S.Stamp;
    Response.points_      = std::move(S.Points);
    Response.intensity_   = std::move(S.Intensity);
    Response.class_id_    = std::move(S.ClassId);
    Response.instance_id_ = std::move(S.InstanceId);

    Serialization::DeserializeResponse(Response, OutputStream);
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
    Response.config.ShowBeams      = LidarConfig.ShowBeams;
    Response.config.ShowSensorBody = LidarConfig.ShowSensorBody;
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

    const bool    LivoxWas       = LidarConfig.Livox;
    const FString LivoxSensorWas = LidarConfig.LivoxSensor;

    LidarConfig.Enable      = Request.config.Enable;
    LidarConfig.Livox       = Request.config.Livox;
    LidarConfig.LivoxSensor = FString(UTF8_TO_TCHAR(Request.config.LivoxSensor.c_str()));
    LidarConfig.ShowBeams      = Request.config.ShowBeams;
    LidarConfig.BeamLength     = Request.config.BeamLength;
    LidarConfig.ShowSensorBody = Request.config.ShowSensorBody;

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

    LidarHitsCriticalSection->Unlock();

    UpdateSensorBody(LidarConfig.ShowSensorBody, LidarConfig.Offset, LidarConfig.Orientation, FVector(0.10, 0.10, 0.07));

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
