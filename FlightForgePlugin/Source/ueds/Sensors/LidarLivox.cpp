#include "LidarLivox.h"

#include "ueds/DronePawn.h"
#include "Serialize.h"

ULidarLivox::ULidarLivox() : ULidar()
{
    SensorType = SensorType::LIDAR_LIVOX;
    Name = TEXT("Lidar Livox");

	StartIndex = 0;
}

void ULidarLivox::BeginPlay()
{
	Super::BeginPlay();

	CSVFilePath = Owner->CSVFilePath;
    
	if (!LoadCSVData(CSVFilePath)) {
		UE_LOG(LogTemp, Error, TEXT("Failed to load CSV data in BeginPlay."));
	}
}


void ULidarLivox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool ULidarLivox::LoadCSVData(const FString& FilePath)
{
    // Convert FString to std::string for CSV parsing
    std::string filePathStr = std::string(TCHAR_TO_UTF8(*FilePath));
    std::ifstream file(filePathStr);

    if (!file.is_open()) {
        UE_LOG(LogTemp, Error, TEXT("Could not open CSV file: %s"), *FilePath);
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip the header line

    LivoxData.Empty(); // Clear any existing data

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        FLivoxDataPoint dataPoint;
        
        // Read values from the CSV line
        std::getline(ss, value, ','); // Read Time/s
        try {
            dataPoint.Time = std::stod(value);
        }
        catch (const std::invalid_argument& e) {
            UE_LOG(LogTemp, Error, TEXT("Invalid data in CSV (Time): %s, %s"), UTF8_TO_TCHAR(value.c_str()), UTF8_TO_TCHAR(e.what()));
            file.close(); // Close file on read error
            return false;
        }

        std::getline(ss, value, ','); // Read Azimuth/deg
         try {
            dataPoint.Azimuth = FMath::DegreesToRadians(std::stod(value));  // Convert to radians
        }
        catch (const std::invalid_argument& e) {
           UE_LOG(LogTemp, Error, TEXT("Invalid data in CSV (Azimuth): %s, %s"), UTF8_TO_TCHAR(value.c_str()), UTF8_TO_TCHAR(e.what()));
           file.close();
           return false;
        }

        std::getline(ss, value, ','); // Read Zenith/deg
        try {
            dataPoint.Zenith = FMath::DegreesToRadians(std::stod(value));  // Convert to radians
        } catch (const std::invalid_argument& e) {
            UE_LOG(LogTemp, Error, TEXT("Invalid data in CSV (Zenith): %s, %s"), UTF8_TO_TCHAR(value.c_str()), UTF8_TO_TCHAR(e.what()));
            file.close();
            return false;
        }
        
        LivoxData.Add(dataPoint); // Add the data point to the array
    }

    file.close();

    if (LivoxData.Num() == 0) {
        UE_LOG(LogTemp, Warning, TEXT("CSV file is empty or contains no valid data."));
        return false;  // Return false if no data was loaded
    }

    UE_LOG(LogTemp, Log, TEXT("Loaded %d data points from CSV."), LivoxData.Num());
    return true;
}

void ULidarLivox::UpdateLidar(bool isExternallyLocked)
{
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
    
    // Livox Mode (CSV Data)
    int pointsPerFrame = 20000;

    ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
        for (int32 col = 0; col < LidarConfig.BeamVertRays; ++col) {
            int i = row * LidarConfig.BeamVertRays + col;

            int dataIndex = (StartIndex + i) % LivoxData.Num();
            const FLivoxDataPoint& dataPoint = LivoxData[dataIndex];

            double azimuth = dataPoint.Azimuth;
            double zenith = dataPoint.Zenith; 

            // Ray direction in *lidar's local frame
            FVector rayDirection_local(
                FMath::Cos(zenith) * FMath::Cos(azimuth),
                FMath::Cos(zenith) * FMath::Sin(azimuth),
                FMath::Sin(zenith)
            );

            // Transform to *world* coordinates
            FVector rayDirection_world = lidarGlobalTransform.TransformVector(rayDirection_local);
            rayDirection_world *= LidarConfig.BeamLength;


            FHitResult HitResult;
            auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;
            rayDirection_local = lidarGlobalTransform.InverseTransformVector(rayDirection_world);
            if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + rayDirection_world, ECC_Visibility, CollisionParams))
            {
                if (HitResult.bBlockingHit) {
                    AActor* HitActor = HitResult.GetActor();
                    if (!Owner->GetVisibilityOtherDrones() && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != Owner) {
                         // Store distance and *local* ray direction.
                        (*LidarHits)[i] = std::make_tuple(-1.0, rayDirection_local.X, rayDirection_local.Y, rayDirection_local.Z);
                    }
                    else{
                        (*LidarHits)[i] = std::make_tuple(HitResult.Distance, rayDirection_local.X, rayDirection_local.Y, rayDirection_local.Z); // CORRECTED
                    }
                } else {
                    // No hit. Store *local* ray direction.
                    (*LidarHits)[i] = std::make_tuple(-1.0, rayDirection_local.X, rayDirection_local.Y, rayDirection_local.Z);
                }
            }
            else{
                (*LidarHits)[i] = std::make_tuple(-1.0, rayDirection_local.X, rayDirection_local.Y, rayDirection_local.Z);  // No hit
            }
        }
    });

    StartIndex = (StartIndex + pointsPerFrame) % LivoxData.Num();
    LidarHitStart.reset(new FVector(lidarLocation));
        

    if (!isExternallyLocked) {
        LidarHitsCriticalSection->Unlock();
    }
}

void ULidarLivox::UpdateSegLidar(bool isExternallyLocked)
{
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

    
    // Livox Mode (CSV Data)
    int pointsPerFrame = 20000;

    ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
        for (int32 col = 0; col < LidarConfig.BeamVertRays; ++col) {
            int i = row * LidarConfig.BeamVertRays + col;

            int dataIndex = (StartIndex + i) % LivoxData.Num();
            const FLivoxDataPoint& dataPoint = LivoxData[dataIndex];

            double azimuth = dataPoint.Azimuth;
            double zenith = dataPoint.Zenith; 

            // Ray direction in *lidar's local frame
            FVector rayDirection_local(
                FMath::Cos(zenith) * FMath::Cos(azimuth),
                FMath::Cos(zenith) * FMath::Sin(azimuth),
                FMath::Sin(zenith)
            );

            // Transform to *world* coordinates
            FVector rayDirection_world = lidarGlobalTransform.TransformVector(rayDirection_local);
            rayDirection_world *= LidarConfig.BeamLength;


            FHitResult HitResult;
            auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;
            rayDirection_local = lidarGlobalTransform.InverseTransformVector(rayDirection_world);
            if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + rayDirection_world, ECC_Visibility, CollisionParams))
            {
                if (HitResult.bBlockingHit) {
                    AActor* HitActor = HitResult.GetActor();
                    if (!Owner->GetVisibilityOtherDrones() && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != Owner) {
                        std::get<0>((*LidarSegHits)[i]) = -1.0;
                        std::get<1>((*LidarSegHits)[i]) = rayDirection_local.X;
                        std::get<2>((*LidarSegHits)[i]) = rayDirection_local.Y;
                        std::get<3>((*LidarSegHits)[i]) = rayDirection_local.Z;
                        std::get<4>((*LidarSegHits)[i]) = -1;
                    }
                    else{
                        std::get<0>((*LidarSegHits)[i]) = HitResult.Distance;
                        std::get<1>((*LidarSegHits)[i]) = rayDirection_local.X;
                        std::get<2>((*LidarSegHits)[i]) = rayDirection_local.Y;
                        std::get<3>((*LidarSegHits)[i]) = rayDirection_local.Z;
                         auto component = Cast<UStaticMeshComponent>(HitResult.GetComponent());

                        if (HitResult.GetComponent()->GetName().Contains("Landscape")) {
                            std::get<4>((*LidarSegHits)[i]) = 1;
                        } else {
                            std::get<4>((*LidarSegHits)[i]) = component->CustomDepthStencilValue;
                        }
                    }
                } else {
                    // No hit. 
                    std::get<0>((*LidarSegHits)[i]) = -1.0;
                    std::get<1>((*LidarSegHits)[i]) = rayDirection_local.X;
                    std::get<2>((*LidarSegHits)[i]) = rayDirection_local.Y;
                    std::get<3>((*LidarSegHits)[i]) = rayDirection_local.Z;
                    std::get<4>((*LidarSegHits)[i]) = -1;
                }
            }
            else{
                std::get<0>((*LidarSegHits)[i]) = -1.0;
                std::get<1>((*LidarSegHits)[i]) = rayDirection_local.X;
                std::get<2>((*LidarSegHits)[i]) = rayDirection_local.Y;
                std::get<3>((*LidarSegHits)[i]) = rayDirection_local.Z;
                std::get<4>((*LidarSegHits)[i]) = -1;  // No hit
            }
        }
    });

    StartIndex = (StartIndex + pointsPerFrame) % LivoxData.Num();
      
  LidarHitStart.reset(new FVector(lidarLocation));

  if (!isExternallyLocked) {
    LidarSegHitsCriticalSection->Unlock();
  }
}


void ULidarLivox::UpdateIntLidar(bool isExternallyLocked)
{
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
    
    // Livox Mode (CSV Data)
    int pointsPerFrame = 20000;

    ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
        for (int32 col = 0; col < LidarConfig.BeamVertRays; ++col) {
            int i = row * LidarConfig.BeamVertRays + col;

            int dataIndex = (StartIndex + i) % LivoxData.Num();
            const FLivoxDataPoint& dataPoint = LivoxData[dataIndex];

            double azimuth = dataPoint.Azimuth;
            double zenith = dataPoint.Zenith; 

            // Ray direction in *lidar's local frame
            FVector rayDirection_local(
                FMath::Cos(zenith) * FMath::Cos(azimuth),
                FMath::Cos(zenith) * FMath::Sin(azimuth),
                FMath::Sin(zenith)
            );

            // Transform to *world* coordinates
            FVector rayDirection_world = lidarGlobalTransform.TransformVector(rayDirection_local);
            rayDirection_world *= LidarConfig.BeamLength;


            FHitResult HitResult;
            auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;
            CollisionParams.bReturnPhysicalMaterial = true;
            rayDirection_local = lidarGlobalTransform.InverseTransformVector(rayDirection_world);
            if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + rayDirection_world, ECC_Visibility, CollisionParams))
            {
                if (HitResult.bBlockingHit) {
                    AActor* HitActor = HitResult.GetActor();
                    if (!Owner->GetVisibilityOtherDrones() && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != Owner) {
                        std::get<0>((*LidarIntHits)[i]) = -1.0;
                        std::get<1>((*LidarIntHits)[i]) = rayDirection_local.X;
                        std::get<2>((*LidarIntHits)[i]) = rayDirection_local.Y;
                        std::get<3>((*LidarIntHits)[i]) = rayDirection_local.Z;
                        std::get<4>((*LidarIntHits)[i]) = -1;
                    }
                    else{
                        std::get<0>((*LidarIntHits)[i]) = HitResult.Distance;
                        std::get<1>((*LidarIntHits)[i]) = rayDirection_local.X;
                        std::get<2>((*LidarIntHits)[i]) = rayDirection_local.Y;
                        std::get<3>((*LidarIntHits)[i]) = rayDirection_local.Z;
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
                    }
                } else {
                    // No hit. 
                    std::get<0>((*LidarIntHits)[i]) = -1.0;
                    std::get<1>((*LidarIntHits)[i]) = rayDirection_local.X;
                    std::get<2>((*LidarIntHits)[i]) = rayDirection_local.Y;
                    std::get<3>((*LidarIntHits)[i]) = rayDirection_local.Z;
                    std::get<4>((*LidarIntHits)[i]) = -1;
                }
            }
            else{
                std::get<0>((*LidarIntHits)[i]) = -1.0;
                std::get<1>((*LidarIntHits)[i]) = rayDirection_local.X;
                std::get<2>((*LidarIntHits)[i]) = rayDirection_local.Y;
                std::get<3>((*LidarIntHits)[i]) = rayDirection_local.Z;
                std::get<4>((*LidarIntHits)[i]) = -1;  // No hit
            }
        }
    });

    StartIndex = (StartIndex + pointsPerFrame) % LivoxData.Num();

    
    LidarHitStart.reset(new FVector(lidarLocation));

    if (!isExternallyLocked) {
        LidarIntHitsCriticalSection->Unlock();
    }
}

