#include "Lidar.h"

//#include <fstream>

#include "ueds/DronePawn.h"
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

	LidarConfig.Livox = false;

	#if PLATFORM_WINDOWS
		LidarHitsCriticalSection       = std::make_unique<FWindowsCriticalSection>();
		LidarSegHitsCriticalSection    = std::make_unique<FWindowsCriticalSection>();
		LidarIntHitsCriticalSection    = std::make_unique<FWindowsCriticalSection>();
	#else
		LidarHitsCriticalSection       = std::make_unique<FPThreadsCriticalSection>();
		LidarSegHitsCriticalSection    = std::make_unique<FPThreadsCriticalSection>();
		LidarIntHitsCriticalSection    = std::make_unique<FPThreadsCriticalSection>();
	#endif

	LidarHits     = std::make_unique<std::vector<std::tuple<double, double, double, double>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
	LidarSegHits  = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
	LidarIntHits  = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
	LidarHitStart = std::make_unique<FVector>();
	LidarHitStart = std::make_unique<FVector>();
}

void ULidar::BeginPlay()
{
	Super::BeginPlay();

    // CSVFilePath = Owner->CSVFilePath;
    //
    // if (!LoadCSVData(CSVFilePath)) {
    //     UE_LOG(LogTemp, Error, TEXT("Failed to load CSV data in BeginPlay."));
    // }
}

void ULidar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FLidarConfig ULidar::GetLidarConfig() {
    return LidarConfig;
}

bool ULidar::SetLidarConfig(const FLidarConfig& Config) {

    LidarHitsCriticalSection->Lock();
    LidarSegHitsCriticalSection->Lock();

    LidarConfig.Enable     = Config.Enable;
    LidarConfig.Livox     = Config.Livox;
    LidarConfig.ShowBeams  = Config.ShowBeams;
    LidarConfig.BeamLength = Config.BeamLength;

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

    /* LidarConfig.FOVHor  = Config.FOVHor; */
    /* LidarConfig.FOVVert = Config.FOVVert; */
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


void ULidar::GetLidarHits(std::vector<Serializable::Drone::GetLidarData::LidarData>& OutLidarData, FVector& OutStart) {

    // UE_LOG(LogTemp, Warning, TEXT("DronePawn::GetLidarHits"));

    LidarHitsCriticalSection->Lock();
  
    UpdateLidar(true);
  
    OutLidarData.resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

    for (int i = 0; i < LidarConfig.BeamHorRays * LidarConfig.BeamVertRays; i++) {
        OutLidarData[i].distance   = std::get<0>((*LidarHits)[i]);
        OutLidarData[i].directionX = std::get<1>((*LidarHits)[i]);
        OutLidarData[i].directionY = std::get<2>((*LidarHits)[i]);
        OutLidarData[i].directionZ = std::get<3>((*LidarHits)[i]);
    }

    OutStart.X = LidarConfig.Offset.X;
    OutStart.Y = LidarConfig.Offset.Y;
    OutStart.Z = LidarConfig.Offset.Z;

    LidarHitsCriticalSection->Unlock();
}

void ULidar::UpdateLidar(bool isExternallyLocked) {
    if (!isExternallyLocked) {
        LidarHitsCriticalSection->Lock();
    }

    auto World = Owner->GetWorld();
    const auto droneTransform = Owner->GetActorTransform();

    // --- Invert Roll ---
    FRotator correctedLidarOrientation = LidarConfig.Orientation;
    if(!LidarConfig.Livox){
    correctedLidarOrientation.Roll = -correctedLidarOrientation.Roll;
    }

    // Lidar's global transform
    FQuat lidarQuat = droneTransform.GetRotation() * correctedLidarOrientation.Quaternion();
    FVector lidarLocation = droneTransform.TransformPosition(LidarConfig.Offset);
    FTransform lidarGlobalTransform(lidarQuat, lidarLocation);

    
    LidarHits = std::make_unique<std::vector<std::tuple<double, double, double, double>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
    
    FVector forwardVec = lidarQuat.RotateVector(FVector::ForwardVector);
    FVector rightVector   = lidarQuat.RotateVector(FVector::RightVector);
    FVector upVec        = lidarQuat.RotateVector(FVector::UpVector);

    double totalHorFov = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
    double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

    ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
        double horAngle = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
        FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
        FVector rotatedRight = rightVector.RotateAngleAxis(horAngle, upVec);


        ParallelFor(LidarConfig.BeamVertRays, [&](int32 col) {
            auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;
            if (LidarConfig.ShowBeams) {
                const FName TraceTag(FString::Printf(TEXT("LidarTraceTag_%d"), 0));
                CollisionParams.TraceTag = TraceTag;
            }

            double vertAngle = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));

            // Ray direction in *world* coordinates.
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
        });
    });
    LidarHitStart.reset(new FVector(lidarLocation));
    

    if (!isExternallyLocked) {
        LidarHitsCriticalSection->Unlock();
    }
}


void ULidar::GetSegLidarHits(std::vector<Serializable::Drone::GetLidarSegData::LidarSegData>& OutLidarSegData, FVector& OutStart) {

    LidarSegHitsCriticalSection->Lock();

    UpdateSegLidar(true);

    OutLidarSegData.resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

    for (int i = 0; i < LidarConfig.BeamHorRays * LidarConfig.BeamVertRays; i++) {
        OutLidarSegData[i].distance     = std::get<0>((*LidarSegHits)[i]);
        OutLidarSegData[i].directionX   = std::get<1>((*LidarSegHits)[i]);
        OutLidarSegData[i].directionY   = std::get<2>((*LidarSegHits)[i]);
        OutLidarSegData[i].directionZ   = std::get<3>((*LidarSegHits)[i]);
        OutLidarSegData[i].segmentation = std::get<4>((*LidarSegHits)[i]);
    }

    OutStart.X = LidarConfig.Offset.X;
    OutStart.Y = LidarConfig.Offset.Y;
    OutStart.Z = LidarConfig.Offset.Z;

    LidarSegHitsCriticalSection->Unlock();
}

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


    LidarSegHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
    
    // Derive the lidar's world-space axes (these are now consistent)
    FVector forwardVec = lidarQuat.RotateVector(FVector::ForwardVector);
    FVector rightVector   = lidarQuat.RotateVector(FVector::RightVector);
    FVector upVec        = lidarQuat.RotateVector(FVector::UpVector);


    // Calculate total horizontal FOV and vertical FOV
    double totalHorFov  = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
    double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

    ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
        // Calculate the horizontal angle for the current ray
        double  horAngle       = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
        FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
        FVector rotatedRight   = rightVector.RotateAngleAxis(horAngle, upVec);

        ParallelFor(LidarConfig.BeamVertRays, [&](int32 col) {
        auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;

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
                // UE_LOG(LogTemp, Warning, TEXT("Ignoring actor: %s"), *HitActor->GetName());
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
        });
    });
    
  LidarHitStart.reset(new FVector(lidarLocation));

  if (!isExternallyLocked) {
    LidarSegHitsCriticalSection->Unlock();
  }
}


void ULidar::GetIntLidarHits(std::vector<Serializable::Drone::GetLidarIntData::LidarIntData>& OutLidarIntData, FVector& OutStart) {

    LidarIntHitsCriticalSection->Lock();

    UpdateIntLidar(true);
    OutLidarIntData.resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

    for (int i = 0; i < LidarConfig.BeamHorRays * LidarConfig.BeamVertRays; i++) {
        OutLidarIntData[i].distance   = std::get<0>((*LidarIntHits)[i]);
        OutLidarIntData[i].directionX = std::get<1>((*LidarIntHits)[i]);
        OutLidarIntData[i].directionY = std::get<2>((*LidarIntHits)[i]);
        OutLidarIntData[i].directionZ = std::get<3>((*LidarIntHits)[i]);
        OutLidarIntData[i].intensity  = std::get<4>((*LidarIntHits)[i]);
    }

    OutStart.X = LidarConfig.Offset.X;
    OutStart.Y = LidarConfig.Offset.Y;
    OutStart.Z = LidarConfig.Offset.Z;

    LidarIntHitsCriticalSection->Unlock();
}

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

  LidarIntHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
    

    // Derive the lidar's world-space axes
    FVector forwardVec = lidarQuat.RotateVector(FVector::ForwardVector);
    FVector rightVector   = lidarQuat.RotateVector(FVector::RightVector);
    FVector upVec        = lidarQuat.RotateVector(FVector::UpVector);



    // Calculate total horizontal FOV and vertical FOV
    double totalHorFov  = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
    double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

    ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
        // Calculate the horizontal angle for the current ray
        double  horAngle       = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
        FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
        FVector rotatedRight   = rightVector.RotateAngleAxis(horAngle, upVec);

        ParallelFor(LidarConfig.BeamVertRays, [&](int32 col) {
        auto CollisionParams                    = FCollisionQueryParams::DefaultQueryParam;
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
                // UE_LOG(LogTemp, Warning, TEXT("Ignoring actor: %s"), *HitActor->GetName());
            } else {
                std::get<0>((*LidarIntHits)[i]) = HitResult.IsValidBlockingHit() ? HitResult.Distance : LidarConfig.BeamLength;
            }
            
            // Transform ray into Lidar coordinates
            const auto ray_in_lidar_coord   = lidarGlobalTransform.InverseTransformVector(raycastAngle);

            std::get<1>((*LidarIntHits)[i]) = ray_in_lidar_coord.X;
            std::get<2>((*LidarIntHits)[i]) = ray_in_lidar_coord.Y;
            std::get<3>((*LidarIntHits)[i]) = ray_in_lidar_coord.Z;

            FVector            surfaceNormal = HitResult.ImpactNormal;
            UPhysicalMaterial* PhysMat       = HitResult.PhysMaterial.Get();
            /* UE_LOG(LogTemp, Warning, TEXT("HitResult.PhysMaterial.Get() %s"), *HitResult.PhysMaterial->GetName()); */
            double intensity = -1;
            /* double intensity = 1.0 - FMath::Abs(FVector::DotProduct(surfaceNormal, raycastAngle.GetSafeNormal())); */
            if (PhysMat) {
                FString PhysMatName = PhysMat->GetName();
                if (PhysMatName.Equals("PM_Grass", ESearchCase::IgnoreCase)) {
                intensity = 1;
                } else if (PhysMatName.Equals("PM_Road", ESearchCase::IgnoreCase)) {
                intensity = 2;
                } else if (PhysMatName.Equals("PM_Tree", ESearchCase::IgnoreCase)) {
                intensity = 3;
                } else if (PhysMatName.Equals("PM_Building", ESearchCase::IgnoreCase)) {
                intensity = 4;
                } else if (PhysMatName.Equals("PM_Fence", ESearchCase::IgnoreCase)) {
                intensity = 5;
                } else if (PhysMatName.Equals("PM_DirtRoad", ESearchCase::IgnoreCase)) {
                intensity = 6;
                } else {
                intensity = 0;
                }
            }
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
        });
    });
    
  LidarHitStart.reset(new FVector(lidarLocation));

  if (!isExternallyLocked) {
    LidarIntHitsCriticalSection->Unlock();
  }
}

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

    Serialization::DeserializeResponse(Response, OutputStream);

    UE_LOG(LogTemp, Log, TEXT("Got lidar config"));
}

void ULidar::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
{
    Serializable::Drone::SetLidarConfig::Request Request;
    Serialization::SerializeRequest(Request, *InputStream);

    bool Status;
    
    LidarHitsCriticalSection->Lock();
    LidarSegHitsCriticalSection->Lock();

    LidarConfig.Enable     = Request.config.Enable;
    LidarConfig.Livox     = Request.config.Livox;
    LidarConfig.ShowBeams  = Request.config.ShowBeams;
    LidarConfig.BeamLength = Request.config.BeamLength;

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
    
    //const auto Status = SetLidarConfig(Config);
    
    Serializable::Drone::SetLidarConfig::Response Response(Status);
    Serialization::DeserializeResponse(Response, OutputStream);
}

void ULidar::GetData(std::stringstream& OutputStream)
{
    switch (LidarMode)
    {
        case FLidarMode::DEFAULT:
        {
            std::vector<Serializable::Drone::GetLidarData::LidarData> LidarData;
            FVector Start;
            GetLidarHits(LidarData, Start);

            Serializable::Drone::GetLidarData::Response Response(true);
            Response.lidarData = LidarData;
            Response.startX = Start.X;
            Response.startY = Start.Y;
            Response.startZ = Start.Z;

            Serialization::DeserializeResponse(Response, OutputStream);
            break;
        }
        
        case FLidarMode::SEG:
        {
            std::vector<Serializable::Drone::GetLidarSegData::LidarSegData> LidarSegData;
            FVector Start;
            GetSegLidarHits(LidarSegData, Start);
        
            Serializable::Drone::GetLidarSegData::Response Response(true);
            Response.lidarSegData = LidarSegData;
            Response.startX = Start.X;
            Response.startY = Start.Y;
            Response.startZ = Start.Z;
        
            Serialization::DeserializeResponse(Response, OutputStream);
            break;
        }
        
        case FLidarMode::INTE:
        {
            std::vector<Serializable::Drone::GetLidarIntData::LidarIntData> LidarIntData;
            FVector Start;
            GetIntLidarHits(LidarIntData, Start);
        
            Serializable::Drone::GetLidarIntData::Response Response(true);
            Response.lidarIntData = LidarIntData;
            Response.startX = Start.X;
            Response.startY = Start.Y;
            Response.startZ = Start.Z;
        
            Serialization::DeserializeResponse(Response, OutputStream);
            break;
        }

        default:
            UE_LOG(LogTemp, Error, TEXT("Invalid Lidar Mode"));
            break;
    }
}
