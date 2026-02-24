/* includes //{ */

#include "DronePawn.h"

#include "ImageUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include <fstream>
#include "PhysicalMaterials/PhysicalMaterial.h"
#include <cereal/details/helpers.hpp>

#include "NaniteSceneProxy.h"
#include "Kismet/BlueprintTypeConversions.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

#include "glTFRuntimeFunctionLibrary.h"
#include "glTFRuntimeAsset.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"
#include "Node.h"
#include "Parsing.h"
#include "yaml.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "glTFRuntimeAsset.h"

#include <fstream>
#include <sstream>
#include <string> 
//}

/*ApplyExternalPropellerConfig()//{*/

void ADronePawn::ApplyExternalPropellerConfig(const TArray<FExternalPropellerConfig>& PropConfigs, const FString& PropellerMeshPath)
{
    ClearDynamicPropellers(); 

    UStaticMesh* PropMesh = nullptr;
    if (!PropellerMeshPath.IsEmpty())
    {
        PropMesh = LoadObject<UStaticMesh>(nullptr, *PropellerMeshPath);
         if (!PropMesh) UE_LOG(LogTemp, Warning, TEXT("Could not load propeller mesh specified in YAML: %s. Falling back to default."), *PropellerMeshPath);
    }
    if (!PropMesh)
    {
         //PropMesh = DefaultExternalPropellerMesh.LoadSynchronous();
         PropMesh = LoadObject<UStaticMesh>(nullptr, *DefaultExternalPropellerMeshPath);
         if (!PropMesh) UE_LOG(LogTemp, Error, TEXT("Failed to load default external propeller mesh! Cannot create propellers."));
         if (!PropMesh) return;
    }

    UE_LOG(LogTemp, Log, TEXT("Applying external config for %d propellers."), PropConfigs.Num());

    for (int i = 0; i < PropConfigs.Num(); ++i)
    {
        const FExternalPropellerConfig& Config = PropConfigs[i];

        FName CompName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), FName(*FString::Printf(TEXT("DynamicPropeller_%s_%d"), *Config.Name, i)));
        UStaticMeshComponent* CurrentPropComp = NewObject<UStaticMeshComponent>(this, CompName);

        if (CurrentPropComp)
        {
            CurrentPropComp->SetupAttachment(RootMeshComponent);
            CurrentPropComp->RegisterComponent(); 
            CurrentPropComp->SetRelativeTransform(Config.RelativeTransform);
            CurrentPropComp->SetStaticMesh(PropMesh);
            CurrentPropComp->SetVisibility(true);
            DynamicPropellers.Add(CurrentPropComp); 

             CurrentPropComp->SetCollisionProfileName(FName(TEXT("NoCollision"))); 
             CurrentPropComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

             UE_LOG(LogTemp, Verbose, TEXT("Created dynamic propeller component %d: %s"), i, *CompName.ToString());
        }
        else
        {
             UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic propeller component %d!"), i);
        }
    }
     bIsExternalModelLoaded = true; 
}

/*//}*/

/*LoadExternalModel()//{*/

bool ADronePawn::LoadExternalModel(const FString& ModelName)
{
    FString ModelFolderPath = FPaths::Combine(ExternalModelBasePath, ModelName);
    FString GltfPath = FPaths::Combine(ModelFolderPath, ModelName + TEXT(".glb")); 
    FString YamlPath = FPaths::Combine(ModelFolderPath, ModelName + TEXT(".yaml"));

    UE_LOG(LogTemp, Log, TEXT("Attempting to load external model: %s"), *ModelName);
    UE_LOG(LogTemp, Log, TEXT("  Checking GLB Path: %s"), *GltfPath);
    UE_LOG(LogTemp, Log, TEXT("  YAML Path: %s"), *YamlPath);

    if (!FPaths::FileExists(GltfPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("GLB file not found, checking for .gltf: %s"), *GltfPath);
        GltfPath = FPaths::Combine(ModelFolderPath, ModelName + TEXT(".gltf"));
        UE_LOG(LogTemp, Log, TEXT("  Checking glTF Path: %s"), *GltfPath);
        if (!FPaths::FileExists(GltfPath))
        {
             UE_LOG(LogTemp, Error, TEXT("External model file not found (tried .glb and .gltf): %s"), *FPaths::Combine(ModelFolderPath, ModelName));
             return false;
        }
    }

    /*if (!GltfLoaderActor)
    {
        UE_LOG(LogTemp, Error, TEXT("LoadExternalModel: GltfLoaderActor is not assigned in DronePawn for model '%s'. Cannot load mesh."), *ModelName);
        return false;
    }*/

    UE_LOG(LogTemp, Log, TEXT("LoadExternalModel: Calling Blueprint GLBtoSM for path: %s"), *GltfPath);

    /*UStaticMesh* LoadedMesh = GltfLoaderActor->GLBtoSM(GltfPath);*/

    UStaticMesh* LoadedMesh = LoadGLBtoSMs_BlueprintEvent(*GltfPath);

    if (LoadedMesh)
    {
        RootMeshComponent->SetStaticMesh(LoadedMesh);
        UE_LOG(LogTemp, Log, TEXT("LoadExternalModel: Blueprint GLBtoSM returned VALID mesh for '%s'."), *ModelName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("LoadExternalModel: Blueprint GLBtoSM returned NULL mesh for path: %s. Check Blueprint implementation."), *GltfPath);
        ClearDynamicPropellers(); 
        return false;
    }

    TArray<FExternalPropellerConfig> PropConfigs;
    FString PropellerMeshPathFromYaml;
    if (ParsePropellerYAML(YamlPath, PropConfigs, PropellerMeshPathFromYaml))
    {
        UE_LOG(LogTemp, Log, TEXT("LoadExternalModel: Successfully parsed propeller config: %s"), *YamlPath);
        ApplyExternalPropellerConfig(PropConfigs, PropellerMeshPathFromYaml);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadExternalModel: Failed to parse or apply propeller config '%s'. Model loaded without external propellers OR config is missing/invalid."), *YamlPath);
        ClearDynamicPropellers(); 
    }

    bIsExternalModelLoaded = true;
    return true;
}

/*//}*/

/*SetStaticMeshByName()//{*/

void ADronePawn::SetStaticMeshByName(const FString& ModelName)
{
    UE_LOG(LogTemp, Log, TEXT("SetStaticMeshByName called with: %s"), *ModelName);

    ClearDynamicPropellers(); 
    RootMeshComponent->SetStaticMesh(nullptr);

    int PredefinedFrameId = -1;
    const FramePropellersTransform* TransformsData = FramePropellersTransforms.GetData();
    for (int i = 0; i < FramePropellersTransforms.Num(); ++i)
    {
        if (TransformsData[i].FrameName.Equals(ModelName, ESearchCase::IgnoreCase))
        {
            PredefinedFrameId = i;
            break; 
        }
    }

    if (PredefinedFrameId != -1)
    {
         UE_LOG(LogTemp, Log, TEXT("Model name '%s' matches predefined frame index %d. Attempting to load baked-in mesh."), *ModelName, PredefinedFrameId);

         FString MeshPath = FString::Printf(TEXT("/FlightForgePlugin/Meshes/_Drones_/%s/%s.%s"),
            *TransformsData[PredefinedFrameId].FrameName,
            *TransformsData[PredefinedFrameId].FrameName,
            *TransformsData[PredefinedFrameId].FrameName);

         UStaticMesh* FrameMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);

         if (FrameMesh) 
         {
             RootMeshComponent->SetStaticMesh(FrameMesh);
             UE_LOG(LogTemp, Log, TEXT("Successfully loaded predefined mesh: %s"), *MeshPath);

             if (!ModelName.Contains("wing", ESearchCase::IgnoreCase))
             {
                SetPredefinedPropellersTransform(PredefinedFrameId);
             }
             else {
                 UE_LOG(LogTemp, Log, TEXT("Predefined model '%s' identified as wing type, skipping default propeller setup."), *ModelName);
             }

             bIsExternalModelLoaded = false; 
             return; 
         }
         else 
         {
              UE_LOG(LogTemp, Error, TEXT("Found predefined frame name '%s', but FAILED to load mesh at path: %s. Check asset existence and path."), *ModelName, *MeshPath);
              RootMeshComponent->SetStaticMesh(nullptr);
              ClearDynamicPropellers();
              return;
         }
    }

    UE_LOG(LogTemp, Log, TEXT("Model name '%s' not found in predefined list or failed to load. Attempting external load."), *ModelName);

    if (LoadExternalModel(ModelName))
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully loaded external model '%s'."), *ModelName);
        return; 
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load model '%s' from both predefined and external sources."), *ModelName);
        RootMeshComponent->SetStaticMesh(nullptr);
        ClearDynamicPropellers();
        return;
    }
}

/*//}*/

/*SetPredefinedPropellersTransform()//{*/

void ADronePawn::SetPredefinedPropellersTransform(const int& frame_id)
{
    if (bIsExternalModelLoaded)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attempted to SetPredefinedPropellersTransform on an externally loaded model. Skipping."));
        return;
    }

     ClearDynamicPropellers();

    if (!FramePropellersTransforms.IsValidIndex(frame_id)) {
        UE_LOG(LogTemp, Error, TEXT("Invalid frame_id %d for SetPredefinedPropellersTransform"), frame_id);
        return;
    }

    const FramePropellersTransform& Transforms = FramePropellersTransforms[frame_id];

    FString MeshPath = "/FlightForgePlugin/Meshes/Propellers/propeller_" + Transforms.PropellerType + ".propeller_" + Transforms.PropellerType;
    UStaticMesh* PropellerMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);

    if (!PropellerMesh) {
        UE_LOG(LogTemp, Error, TEXT("Predefined propeller mesh not loaded! Path: %s. Cannot create propellers."), *MeshPath);
        return; 
    }

    TArray<FTransform> PredefinedTransforms = {
        Transforms.FrontLeft,
        Transforms.FrontRight,
        Transforms.RearLeft,
        Transforms.RearRight
    };
     TArray<FString> PredefinedNames = { TEXT("FL"), TEXT("FR"), TEXT("RL"), TEXT("RR") }; // Optional names

    UE_LOG(LogTemp, Log, TEXT("Applying predefined config for 4 propellers (frame: %s)."), *Transforms.FrameName);

    for(int i = 0; i < PredefinedTransforms.Num(); ++i)
    {
        FName CompName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), FName(*FString::Printf(TEXT("PredefPropeller_%s_%d"), *PredefinedNames[i], i)));
        UStaticMeshComponent* CurrentPropComp = NewObject<UStaticMeshComponent>(this, CompName);

         if (CurrentPropComp)
        {
            CurrentPropComp->SetupAttachment(RootMeshComponent);
            CurrentPropComp->RegisterComponent();
            CurrentPropComp->SetRelativeTransform(PredefinedTransforms[i]);
            CurrentPropComp->SetStaticMesh(PropellerMesh);
            CurrentPropComp->SetVisibility(true);
            CurrentPropComp->SetCollisionProfileName(FName(TEXT("NoCollision")));
            CurrentPropComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

            DynamicPropellers.Add(CurrentPropComp); // Add to the dynamic list
            UE_LOG(LogTemp, Verbose, TEXT("Created predefined propeller component %d: %s"), i, *CompName.ToString());
        }
        else
        {
             UE_LOG(LogTemp, Error, TEXT("Failed to create predefined propeller component %d!"), i);
        }
    }

}

/*//}*/

/*ParsePropellerYAML()//{*/

bool ADronePawn::ParsePropellerYAML(const FString& YamlPath, TArray<FExternalPropellerConfig>& OutPropConfigs, FString& OutPropellerMeshPath)
{
    OutPropConfigs.Empty();
    OutPropellerMeshPath.Empty();

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.FileExists(*YamlPath))
    {
        UE_LOG(LogTemp, Error, TEXT("ParsePropellerYAML: Config file not found: %s"), *YamlPath);
        return false;
    }

    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *YamlPath))
    {
        UE_LOG(LogTemp, Error, TEXT("ParsePropellerYAML: Failed to read propeller config file: %s"), *YamlPath);
        return false;
    }

    try
    {
        std::string FileContentStd = TCHAR_TO_UTF8(*FileContent); 
        YAML::Node RootNode = YAML::Load(FileContentStd);      

        /*if (!RootNode.IsMap()) {
             UE_LOG(LogTemp, Error, TEXT("ParsePropellerYAML: Root node in '%s' is not a Map."), *YamlPath);
             return false;
        }*/

        if (RootNode["propeller_mesh"] && RootNode["propeller_mesh"].IsScalar()) {
             OutPropellerMeshPath = UTF8_TO_TCHAR(RootNode["propeller_mesh"].as<std::string>().c_str());
             UE_LOG(LogTemp, Log, TEXT("ParsePropellerYAML: Found propeller_mesh path: %s"), *OutPropellerMeshPath);
        } else if (RootNode["propeller_mesh"]) {
             UE_LOG(LogTemp, Warning, TEXT("ParsePropellerYAML: 'propeller_mesh' key found in '%s', but it's not a scalar string. Ignoring."), *YamlPath);
        }

        if (RootNode["propellers"] && RootNode["propellers"].IsSequence())
        {
          const YAML::Node& PropellerNodes = RootNode["propellers"];
          const int32 NumPropellerNodes = PropellerNodes.size();
          UE_LOG(LogTemp, Log, TEXT("ParsePropellerYAML: Found %d entries under 'propellers' key."), NumPropellerNodes);

          int32 CurrentIndex = 0;
          for (YAML::const_iterator it = PropellerNodes.begin(); it != PropellerNodes.end(); ++it, ++CurrentIndex) 
          {
            const YAML::Node& PropNode = *it;
            if (!PropNode.IsMap())
            {
              UE_LOG(LogTemp, Warning, TEXT("ParsePropellerYAML: Item %d in 'propellers' sequence (in '%s') is not a valid Map. Skipping."), CurrentIndex, *YamlPath);
              continue;
            }

            FString PropName = FString::Printf(TEXT("Propeller_%d"), CurrentIndex);
            bool bParsedNameFromYaml = false;

            FVector PropLocation = FVector::ZeroVector;
            FRotator PropRotation = FRotator::ZeroRotator;
            FVector PropScale = FVector::OneVector;
            bool bParsedLocation = false;
            bool bParsedRotation = false;
            bool bParsedScale = false;

            if (PropNode["name"] && PropNode["name"].IsScalar()) {
              PropName = UTF8_TO_TCHAR(PropNode["name"].as<std::string>().c_str());
              bParsedNameFromYaml = true;
            }

            UE_LOG(LogTemp, Verbose, TEXT("Processing Propeller Index %d, Final Name: %s (From YAML: %s)"),
                CurrentIndex, *PropName, bParsedNameFromYaml ? TEXT("Yes") : TEXT("No"));

                if (PropNode["location"] && PropNode["location"].IsSequence() && PropNode["location"].size() == 3) {
                    try {
                        PropLocation.X = PropNode["location"][0].as<double>()*100;
                        PropLocation.Y = PropNode["location"][1].as<double>()*-100;
                        PropLocation.Z = PropNode["location"][2].as<double>()*100;
                        bParsedLocation = true;
                    } catch (const YAML::BadConversion& e) {
                        UE_LOG(LogTemp, Warning, TEXT("ParsePropellerYAML: Propeller '%s': Bad conversion for 'location'. Error: %s. Skipping."), *PropName, UTF8_TO_TCHAR(e.what())); continue;
                    }
                } else { UE_LOG(LogTemp, Warning, TEXT("ParsePropellerYAML: Propeller '%s': 'location' key is missing, not a sequence, or wrong size. Skipping."), *PropName); continue; }


                if (PropNode["orientation"] && PropNode["orientation"].IsSequence() && PropNode["orientation"].size() == 3) 
                {
                     try {
                        PropRotation.Roll = PropNode["orientation"][0].as<double>();
                        PropRotation.Pitch = PropNode["orientation"][1].as<double>();
                        PropRotation.Yaw = PropNode["orientation"][2].as<double>();
                         bParsedRotation = true;
                     } catch (const YAML::Exception& e) { 
                        UE_LOG(LogTemp, Warning, TEXT("ParsePropellerYAML: Propeller '%s': Error parsing 'orientation'. Error: %s. Skipping."), *PropName, UTF8_TO_TCHAR(e.what())); continue;
                    }
                }
                else { UE_LOG(LogTemp, Warning, TEXT("ParsePropellerYAML: Propeller '%s': Neither 'orientation' map nor 'rotation' sequence found/valid. Skipping."), *PropName); continue; }


                if (PropNode["scale"] && PropNode["scale"].IsSequence() && PropNode["scale"].size() == 3) {
                     try {
                        PropScale.X = PropNode["scale"][0].as<double>();
                        PropScale.Y = PropNode["scale"][1].as<double>();
                        PropScale.Z = PropNode["scale"][2].as<double>();
                        bParsedScale = true;
                     } catch (const YAML::BadConversion& e) {
                        UE_LOG(LogTemp, Warning, TEXT("ParsePropellerYAML: Propeller '%s': Bad conversion for 'scale'. Error: %s. Skipping."), *PropName, UTF8_TO_TCHAR(e.what())); continue;
                    }
                } else { UE_LOG(LogTemp, Warning, TEXT("ParsePropellerYAML: Propeller '%s': 'scale' key is missing, not a sequence, or wrong size. Skipping."), *PropName); continue; }


                if (bParsedLocation && bParsedRotation && bParsedScale) {
                    OutPropConfigs.Add(FExternalPropellerConfig(FTransform(PropRotation, PropLocation, PropScale), PropName));
                    UE_LOG(LogTemp, Verbose, TEXT("ParsePropellerYAML: Successfully parsed config for propeller '%s'"), *PropName);
                }
            } 
        }
        else {
             UE_LOG(LogTemp, Error, TEXT("ParsePropellerYAML: Required key 'propellers' not found or not a sequence in '%s'."), *YamlPath);
        }
    }
    catch (const YAML::Exception& e) {
        UE_LOG(LogTemp, Error, TEXT("ParsePropellerYAML: Failed to parse YAML file '%s'. Error: %s"), *YamlPath, UTF8_TO_TCHAR(e.what()));
        return false; 
    }


    if (OutPropConfigs.Num() == 0 && OutPropellerMeshPath.IsEmpty())
    {
         UE_LOG(LogTemp, Warning, TEXT("ParsePropellerYAML: File '%s' parsed, but no valid propeller configurations OR propeller_mesh were found."), *YamlPath);
    }

    UE_LOG(LogTemp, Log, TEXT("ParsePropellerYAML: Finished parsing '%s'. Found %d valid propeller configs."), *YamlPath, OutPropConfigs.Num());
    return true; 
}

/*//}*/

/*LoadCSVData()//{*/
bool ADronePawn::LoadCSVData(const FString& FilePath)
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
/*//}*/

/* ADronePawn() //{ */

// Sets default values
ADronePawn::ADronePawn() {

  // UE_LOG(LogTemp, Warning, TEXT("ADronePawn constructor"));

  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickGroup    = TG_PrePhysics;

#if PLATFORM_WINDOWS
  RgbCameraBufferCriticalSection    = std::make_unique<FWindowsCriticalSection>();
  StereoCameraBufferCriticalSection = std::make_unique<FWindowsCriticalSection>();
  RgbSegCameraBufferCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
  RgbCameraBufferCriticalSection    = std::make_unique<FPThreadsCriticalSection>();
  StereoCameraBufferCriticalSection = std::make_unique<FPThreadsCriticalSection>();
  RgbSegCameraBufferCriticalSection = std::make_unique<FPThreadsCriticalSection>();
#endif

  InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<ADronePawn>>>>();

  RootMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMeshComponent"));
  
  SetRootComponent(RootMeshComponent); 

  FramePropellersTransforms.Empty();

  // X500
  FramePropellersTransforms.Add(FramePropellersTransform(FString(TEXT("x500")), FString(TEXT("x500")),
                                                         FTransform(FRotator(0, 0, 0), FVector(-18.8, -18.8, 6.5), FVector(-0.85, 0.85, -0.8)),
                                                         FTransform(FRotator(0, 0, 0), FVector(-18.8, 18.8, 6.5), FVector(-0.85, -0.85, -0.8)),
                                                         FTransform(FRotator(0, 0, 0), FVector(18.8, -18.8, 6.5), FVector(-0.85, -0.85, -0.8)),
                                                         FTransform(FRotator(0, 0, 0), FVector(18.8, 18.8, 6.5), FVector(-0.85, 0.85, -0.8))));

  // T650
  FramePropellersTransforms.Add(FramePropellersTransform(FString(TEXT("t650")), FString(TEXT("x500")),
                                                         FTransform(FRotator(0, 0, 0), FVector(-26.3, -26.3, 4.6), FVector(-1, 1, -1)),
                                                         FTransform(FRotator(0, 0, 0), FVector(-26.3, 26.3, 4.6), FVector(-1, -1, -1)),
                                                         FTransform(FRotator(0, 0, 0), FVector(26.3, -26.3, 4.6), FVector(-1, -1, -1)),
                                                         FTransform(FRotator(0, 0, 0), FVector(26.3, 26.3, 4.6), FVector(-1, 1, -1))));

  // A300
  FramePropellersTransforms.Add(FramePropellersTransform(
      FString(TEXT("a300")), FString(TEXT("robofly")), FTransform(FRotator(0, 0, 0), FVector(-9.45, -11.7, 2.4), FVector(1, -1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(-9.45, 11.7, 2.4), FVector(1, 1, 1)), FTransform(FRotator(0, 0, 0), FVector(9.45, -11.7, 2.4), FVector(1, 1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(9.45, 11.7, 2.4), FVector(1, -1, 1))));

  // RoboFly
  FramePropellersTransforms.Add(FramePropellersTransform(
      FString(TEXT("robofly")), FString(TEXT("robofly")), FTransform(FRotator(0, 0, 0), FVector(-9.55, -9.55, 1.7), FVector(1, -1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(-9.55, 9.55, 1.7), FVector(1, 1, 1)), FTransform(FRotator(0, 0, 0), FVector(9.55, -9.55, 1.7), FVector(1, 1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(9.55, 9.55, 1.7), FVector(1, -1, 1))));

  // Wings
  FramePropellersTransforms.Add(FramePropellersTransform(
      FString(TEXT("wing")), FString(TEXT("robofly")), FTransform(FRotator(0, 0, 0), FVector(-9.55, -9.55, 1.7), FVector(1, -1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(-9.55, 9.55, 1.7), FVector(1, 1, 1)), FTransform(FRotator(0, 0, 0), FVector(9.55, -9.55, 1.7), FVector(1, 1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(9.55, 9.55, 1.7), FVector(1, -1, 1))));

  FramePropellersTransforms.Add(FramePropellersTransform(
      FString(TEXT("wing_2")), FString(TEXT("robofly")), FTransform(FRotator(0, 0, 0), FVector(-9.55, -9.55, 1.7), FVector(1, -1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(-9.55, 9.55, 1.7), FVector(1, 1, 1)), FTransform(FRotator(0, 0, 0), FVector(9.55, -9.55, 1.7), FVector(1, 1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(9.55, 9.55, 1.7), FVector(1, -1, 1))));

  //Gimbal UAV
  FramePropellersTransforms.Add(FramePropellersTransform(
 FString(TEXT("gimbal")),
 FString(TEXT("robofly")),
   FTransform(FRotator(0, 0, 0), FVector(-13.65, -16.58, -4), FVector(2, -2, 2)),
   FTransform(FRotator(0, 0, 0), FVector(-13.65, 16.58, -4), FVector(2, 2, 2)),
   FTransform(FRotator(0, 0, 0), FVector(13.65, -16.58, -4), FVector(2, 2, 2)),
   FTransform(FRotator(0, 0, 0), FVector(13.65, 16.58, -4), FVector(2, -2, 2))));

  SceneCaptureMeshHolderRgb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolderRgb"));
  SceneCaptureMeshHolderRgb->SetupAttachment(RootMeshComponent);
  
  SceneCaptureMeshHolderStereoLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolderStereoLeft"));
  SceneCaptureMeshHolderStereoLeft->SetupAttachment(RootMeshComponent);

  SceneCaptureMeshHolderStereoRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolderStereoRight"));
  SceneCaptureMeshHolderStereoRight->SetupAttachment(RootMeshComponent);

  SceneCaptureComponent2DRgb = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DRgb"));
  SceneCaptureComponent2DRgb->SetupAttachment(SceneCaptureMeshHolderRgb);

  SceneCaptureComponent2DRgbSeg = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DRgbSeg"));
  SceneCaptureComponent2DRgbSeg->SetupAttachment(SceneCaptureMeshHolderRgb);

  SceneCaptureComponent2DStereoLeft = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DStereoLeft"));
  SceneCaptureComponent2DStereoLeft->SetupAttachment(SceneCaptureMeshHolderStereoLeft);

  SceneCaptureComponent2DStereoRight = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DStereoRight"));
  SceneCaptureComponent2DStereoRight->SetupAttachment(SceneCaptureMeshHolderStereoRight);

  ThirdPersonCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonCameraSpringArm"));
  ThirdPersonCameraSpringArm->SetupAttachment(RootMeshComponent);
  ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
  ThirdPersonCamera->SetupAttachment(ThirdPersonCameraSpringArm);

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

  StartIndex = 0;


  /* CSVFilePath = FString(TEXT("/home/david/mid360.csv")); */
  /* CSVFilePath = FPaths::ProjectContentDir() + TEXT("mid360.csv"); */ 
  /* CSVFilePath = FPaths::ProjectSavedDir() + TEXT("mid360.csv"); */
  /* FString PluginContentDir = FString(TEXT("/Game/../Plugins/flight_forge/FlightForgePlugin/Content/")); */

  /* CSVFilePath = PluginContentDir + TEXT("mid360.csv"); */

    FString PluginName = TEXT("FlightForgePlugin");
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(PluginName);
    if (Plugin.IsValid())
    {
        FString PluginContentDir = Plugin->GetContentDir();
        FString CSVRelativePath = TEXT("Lidar/mid360.csv");
        CSVFilePath = FPaths::Combine(PluginContentDir, CSVRelativePath);

    }
    else
    {
      UE_LOG(LogTemp, Error, TEXT("Plugin not found"));
    }

  FTimerHandle OusterTimerHandle;

  RangefinderConfig.BeamLength = DEFAULT_RANGEFINDER_BEAM_LENGTH;
  RangefinderConfig.Offset     = FVector(0, 0, -10);

    
  ExternalModelBasePath = FPaths::ProjectPluginsDir() + TEXT("flight_forge/FlightForgePlugin/Content/ExternalModels/");
    
  /* TODO: proper path to the propeller */  
  DefaultExternalPropellerMesh = TSoftObjectPtr<UStaticMesh>(FSoftObjectPath(FPaths::ProjectPluginsDir() + TEXT("flight_forge/FlightForgePlugin/Content/Meshes/Propellers/propeller_X500.propeller_X500")));

  DefaultExternalPropellerMeshPath = "/FlightForgePlugin/Meshes/Propellers/propeller_x500.propeller_x500";
    


#if PLATFORM_WINDOWS
  LidarHitsCriticalSection       = std::make_unique<FWindowsCriticalSection>();
  LidarSegHitsCriticalSection    = std::make_unique<FWindowsCriticalSection>();
  LidarIntHitsCriticalSection    = std::make_unique<FWindowsCriticalSection>();
  RangefinderHitsCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
  LidarHitsCriticalSection          = std::make_unique<FPThreadsCriticalSection>();
  LidarSegHitsCriticalSection       = std::make_unique<FPThreadsCriticalSection>();
  LidarIntHitsCriticalSection       = std::make_unique<FPThreadsCriticalSection>();
  RangefinderHitsCriticalSection    = std::make_unique<FPThreadsCriticalSection>();
#endif

  LidarHits     = std::make_unique<std::vector<std::tuple<double, double, double, double>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
  LidarSegHits  = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
  LidarIntHits  = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
  LidarHitStart = std::make_unique<FVector>();
  LidarHitStart = std::make_unique<FVector>();

  droneServer = std::make_shared<DroneServer>(*this);
}

//}

/* beginPlay() //{ */

// Called when the game starts or when spawned
void ADronePawn::BeginPlay() {

  Super::BeginPlay();

  // TODO check https://github.com/TimmHess/UnrealImageCapture
  RenderTarget2DRgb = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DRgb->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DRgb->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DRgb->bGPUSharedFlag     = true;

  RenderTarget2DStereoLeft = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DStereoLeft->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DStereoLeft->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DStereoLeft->bGPUSharedFlag     = true;

  RenderTarget2DStereoRight = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DStereoRight->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DStereoRight->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DStereoRight->bGPUSharedFlag     = true;

  RenderTarget2DRgbSeg = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DRgbSeg->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DRgbSeg->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DRgbSeg->bGPUSharedFlag     = true;
  
  SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DRgb->TextureTarget = RenderTarget2DRgb;
  SceneCaptureComponent2DRgb->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DRgb->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DRgb->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DRgb->bCaptureOnMovement           = false;
  SceneCaptureComponent2DRgb->bUseRayTracingIfEnabled      = true;

  SceneCaptureComponent2DRgbSeg->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DRgbSeg->TextureTarget = RenderTarget2DRgbSeg;
  SceneCaptureComponent2DRgbSeg->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DRgbSeg->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DRgbSeg->bCaptureOnMovement           = false;
  SceneCaptureComponent2DRgbSeg->bUseRayTracingIfEnabled      = false;
  SceneCaptureComponent2DRgbSeg->AddOrUpdateBlendable(PostProcessMaterial, 1);

  SceneCaptureComponent2DStereoLeft->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DStereoLeft->TextureTarget = RenderTarget2DStereoLeft;
  SceneCaptureComponent2DStereoLeft->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DStereoLeft->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DStereoLeft->bCaptureOnMovement           = false;
  SceneCaptureComponent2DStereoLeft->bUseRayTracingIfEnabled      = true;

  SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DStereoRight->TextureTarget = RenderTarget2DStereoRight;
  SceneCaptureComponent2DStereoRight->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DStereoRight->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DStereoRight->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DStereoRight->bCaptureOnMovement           = false;
  SceneCaptureComponent2DStereoRight->bUseRayTracingIfEnabled      = true;

  rgb_camera_config_.ShowCameraComponent    = false;
  rgb_camera_config_.Offset                 = FVector(0, 0, 0);
  rgb_camera_config_.Orientation            = FRotator(0, 0, 0);
  rgb_camera_config_.FOVAngle               = 90;
  rgb_camera_config_.Width                  = 640;
  rgb_camera_config_.Height                 = 480;
  rgb_camera_config_.enable_motion_blur     = true;
  rgb_camera_config_.motion_blur_amount     = 1.0;
  rgb_camera_config_.motion_blur_distortion = 50.0;

  stereo_camera_config_.ShowCameraComponent = false;
  stereo_camera_config_.Offset              = FVector(0, 0, 0);
  stereo_camera_config_.Orientation         = FRotator(0, 0, 0);
  stereo_camera_config_.FOVAngle            = 90;
  stereo_camera_config_.Width               = 640;
  stereo_camera_config_.Height              = 480;
  stereo_camera_config_.baseline            = 0.1;

  SetRgbCameraConfig(rgb_camera_config_);
  SetStereoCameraConfig(stereo_camera_config_);

  if (!LoadCSVData(CSVFilePath)) {
        UE_LOG(LogTemp, Error, TEXT("Failed to load CSV data in BeginPlay."));
    }

}

//}

/*ClearDynamicPropellers()//{*/

void ADronePawn::ClearDynamicPropellers()
{
    UE_LOG(LogTemp, Log, TEXT("Clearing %d dynamic propellers."), DynamicPropellers.Num());
    for (UStaticMeshComponent* Comp : DynamicPropellers)
    {
        if (Comp && Comp->IsValidLowLevel() && !Comp->IsBeingDestroyed()) 
        {
            Comp->UnregisterComponent(); 
            Comp->DestroyComponent();
        }
    }
    DynamicPropellers.Empty(); 

    bIsExternalModelLoaded = false; 
}

/*//}*/

/* startServer() //{ */

void ADronePawn::StartServer() {
  // UE_LOG(LogTemp, Warning, TEXT("Starting drone server"));
  droneServer->Run();
}

//}

/* endPlay() //{ */

void ADronePawn::EndPlay(const EEndPlayReason::Type EndPlayReason) {

  RgbCameraBuffer.Empty();
  StereoLeftCameraBuffer.Empty();
  StereoRightCameraBuffer.Empty();
  droneServer->Stop();
}

//}

/* setupPlayerInputComponent() //{ */

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//}

/* updateLidar() //{ */
void ADronePawn::UpdateLidar(bool isExternallyLocked) {
    if (!isExternallyLocked) {
        LidarHitsCriticalSection->Lock();
    }

    auto World = GetWorld();
    const auto droneTransform = GetActorTransform();

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

    if (LidarConfig.Livox) {
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
                        if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this) {
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

    } else {

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
                        if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this)
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
    }

    if (!isExternallyLocked) {
        LidarHitsCriticalSection->Unlock();
    }
}
//}

/* updateSegLidar() //{ */

void ADronePawn::UpdateSegLidar(bool isExternallyLocked) {

  if (!isExternallyLocked) {
    LidarSegHitsCriticalSection->Lock();
  }

  auto       World          = GetWorld();
  const auto droneTransform = GetActorTransform();

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


  if (LidarConfig.Livox) {
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
                        if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this) {
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

    } else {

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
                if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this)
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
    }
  LidarHitStart.reset(new FVector(lidarLocation));

  if (!isExternallyLocked) {
    LidarSegHitsCriticalSection->Unlock();
  }
}

//}

/* updateIntLidar() //{ */

void ADronePawn::UpdateIntLidar(bool isExternallyLocked) {

  if (!isExternallyLocked) {
    LidarIntHitsCriticalSection->Lock();
  }

  auto       World          = GetWorld();
  const auto droneTransform = GetActorTransform();

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

    if (LidarConfig.Livox) {
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
                        if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this) {
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

    }else{

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
                if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this)
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
    }
  LidarHitStart.reset(new FVector(lidarLocation));

  if (!isExternallyLocked) {
    LidarIntHitsCriticalSection->Unlock();
  }
}

//}

/* getCrashedState() //{ */

bool ADronePawn::GetCrashState(void) {

  // UE_LOG(LogTemp, Warning, TEXT("ADronePawn::GetCrashState"));

  return uav_crashed_;
}

//}

/*GetRangefinderData()//{*/
void ADronePawn::GetRangefinderData(double& range) {
  RangefinderHitsCriticalSection->Lock();

  FVector Start                = GetActorLocation() + GetActorRotation().RotateVector(RangefinderConfig.Offset);
  FVector RangefinderDirection = -GetActorUpVector();

  if (UWorld* World = GetWorld()) {
    FHitResult HitResult;
    if (World->LineTraceSingleByChannel(HitResult, Start, Start + RangefinderConfig.BeamLength * RangefinderDirection, ECollisionChannel::ECC_Visibility)) {
      if (HitResult.bBlockingHit) {
        range = HitResult.Distance;
      } else {
        range = -1;
      }
      // UE_LOG(LogTemp, Warning, TEXT("Rangefinder range = %lf"), range);
    }
  }

  RangefinderHitsCriticalSection->Unlock();
}
/*//}*/

/* getLidarHits() //{ */

void ADronePawn::GetLidarHits(std::vector<Serializable::Drone::GetLidarData::LidarData>& OutLidarData, FVector& OutStart) {

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

//}

/* getSegLidarHits() //{ */

void ADronePawn::GetSegLidarHits(std::vector<Serializable::Drone::GetLidarSegData::LidarSegData>& OutLidarSegData, FVector& OutStart) {

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

//}

/* getIntLidarHits() //{ */

void ADronePawn::GetIntLidarHits(std::vector<Serializable::Drone::GetLidarIntData::LidarIntData>& OutLidarIntData, FVector& OutStart) {

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

//}

/* updateCamera() //{ */

void ADronePawn::UpdateCamera(bool isExternallyLocked, int type = 1, double stamp = 0.0) {

  switch (type) {

    case CAMERA_MODE_NONE: {
      break;
    }

    case CAMERA_MODE_RGB: {

      if (!isExternallyLocked) {
        RgbCameraBufferCriticalSection->Lock();
      }

      const auto Resource = RenderTarget2DRgb->GameThread_GetRenderTargetResource();

      Resource->ReadPixels(RgbCameraBuffer);

      RgbCameraDataNeedsCompress = true;

      rgb_stamp_ = stamp;

      RgbCameraRendered = true;

      if (!isExternallyLocked) {
        RgbCameraBufferCriticalSection->Unlock();
      }

      break;
    }

    case CAMERA_MODE_STEREO: {

      if (!isExternallyLocked) {
        StereoCameraBufferCriticalSection->Lock();
      }

      const auto ResourceLeft = RenderTarget2DStereoLeft->GameThread_GetRenderTargetResource();

      const auto ResourceRight = RenderTarget2DStereoRight->GameThread_GetRenderTargetResource();

      ResourceLeft->ReadPixels(StereoLeftCameraBuffer);
      ResourceRight->ReadPixels(StereoRightCameraBuffer);

      StereoCameraDataNeedsCompress = true;

      StereoCameraRendered = true;

      stereo_stamp_ = stamp;

      if (!isExternallyLocked) {
        StereoCameraBufferCriticalSection->Unlock();
      }

      break;
    }

    case CAMERA_MODE_RGB_SEG: {

      SceneCaptureComponent2DRgbSeg->CaptureSource = SCS_FinalColorLDR;
      SceneCaptureComponent2DRgbSeg->TextureTarget = RenderTarget2DRgbSeg;
      // SceneCaptureComponent2DRgb->CaptureScene();

      if (!isExternallyLocked) {
        RgbSegCameraBufferCriticalSection->Lock();
      }

      const auto ResourceSeg = RenderTarget2DRgbSeg->GameThread_GetRenderTargetResource();

      ResourceSeg->ReadPixels(RgbSegCameraBuffer);

      RgbSegCameraDataNeedsCompress = false;

      RgbSegCameraRendered = true;

      rgb_seg_stamp_ = stamp;

      if (!isExternallyLocked) {
        RgbSegCameraBufferCriticalSection->Unlock();
      }

      break;
    }

    default: {
      SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorLDR;
      SceneCaptureComponent2DRgb->TextureTarget = RenderTarget2DRgb;
      break;
    }
  }
}

const TArray<FramePropellersTransform>& ADronePawn::GetPredefinedFrameTransforms() const
{
    return FramePropellersTransforms;
}

/* void ADronePawn::SetPropellersTransform(const int& frame_id) { */
/*   const FramePropellersTransform* Transforms = FramePropellersTransforms.GetData(); */

/*   FString mesh_path = "/FlightForgePlugin/Meshes/Propellers/propeller_" + Transforms[frame_id].PropellerType; */

/*   if (UStaticMesh* PropellerMesh = LoadObject<UStaticMesh>(nullptr, *mesh_path)) { */
/*     PropellerFrontLeft->SetStaticMesh(PropellerMesh); */
/*     PropellerFrontRight->SetStaticMesh(PropellerMesh); */
/*     PropellerRearLeft->SetStaticMesh(PropellerMesh); */
/*     PropellerRearRight->SetStaticMesh(PropellerMesh); */
/*   } else { */
/*     UE_LOG(LogTemp, Error, TEXT("The Propeller was not loaded!")); */
/*   } */

/*   PropellerFrontLeft->SetRelativeTransform(Transforms[frame_id].FrontLeft); */
/*   PropellerFrontRight->SetRelativeTransform(Transforms[frame_id].FrontRight); */
/*   PropellerRearLeft->SetRelativeTransform(Transforms[frame_id].RearLeft); */
/*   PropellerRearRight->SetRelativeTransform(Transforms[frame_id].RearRight); */
/* } */

/* void ADronePawn::SetStaticMesh(const int& frame_id) { */
/*   FString mesh_path = "/FlightForgePlugin/Meshes/_Drones_/"; */

/*   int predefined_frame_count = FramePropellersTransforms.Num(); */

/*   // last "empty" frame is not included in "FramePropellersTransforms" */
/*   if (predefined_frame_count == frame_id) { */
/*     return; */
/*   } */

/*   FString frame_name = FramePropellersTransforms.GetData()[frame_id].FrameName; */

/*   mesh_path += frame_name + "/" + frame_name + "." + frame_name; */

/*   if (UStaticMesh* FrameMesh = LoadObject<UStaticMesh>(nullptr, *mesh_path)) { */
/*     RootMeshComponent->SetStaticMesh(FrameMesh); */
/*   } else { */
/*     UE_LOG(LogTemp, Error, TEXT("The Frame was not loaded!")); */
/*   } */

/*   // wing has not propellers */
/*   if (frame_name.Contains("wing")) { */
/*     return; */
/*   } */

/*   SetPropellersTransform(frame_id); */
/* } */

void ADronePawn::Simulate_UE_Physics(const float& stop_simulation_delay) {
  // RootMeshComponent->SetSimulatePhysics(true);
  GetWorldTimerManager().SetTimer(TimerHandle_Disabled_Physics, this, &ADronePawn::DisabledPhysics_StartRotatePropellers, stop_simulation_delay, false);
}


/**
 * Visibility settings
 * @param bEnable
 * 
 * - true -- UAVs see each others
 *
 * - false -- RL purpose, UAVs do not see each others
 */
void ADronePawn::SetVisibilityOtherDrones(bool bEnable)
{
    ECollisionResponse NewResponse = bEnable ? ECR_Block : ECR_Ignore;
    bCanSeeOtherDrone = bEnable; // Update the flag used by Lidar

    // Set collision for the root body
    if (RootMeshComponent) // Always check pointers
    {
        RootMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, NewResponse);
        RootMeshComponent->MarkRenderStateDirty();
    }

    // --- FIX: Iterate through dynamic propellers ---
    for (UStaticMeshComponent* PropComp : DynamicPropellers)
    {
        if (PropComp) // Check pointer
        {
            PropComp->SetCollisionResponseToChannel(ECC_Pawn, NewResponse);
            PropComp->MarkRenderStateDirty();
        }
    }
}

void ADronePawn::DisabledPhysics_StartRotatePropellers()
{
  UE_LOG(LogTemp, Warning, TEXT("Disabled physics after a 3 second delay"));

  RootMeshComponent->SetSimulatePhysics(false);
  this->propellers_rotate = true;
}

//}

/* GetLeftCameraDataFromServerThread() //{ */

bool ADronePawn::GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double& stamp) {

  RgbCameraBufferCriticalSection->Lock();

  rgb_camera_last_request_time_ = FPlatformTime::Seconds();

  if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
    auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
    Instruction->Function = [](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(true, CAMERA_MODE_RGB); };
    InstructionQueue->Enqueue(Instruction);

    FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
  }

  if (!RgbCameraRendered) {
    UE_LOG(LogTemp, Warning, TEXT("DronePawn(): rgb camera is not rendered, returning"));
    RgbCameraBufferCriticalSection->Unlock();
    return false;
  }

  if (RgbCameraDataNeedsCompress) {
    FImageUtils::ThumbnailCompressImageArray(RenderTarget2DRgb->SizeX, RenderTarget2DRgb->SizeY, RgbCameraBuffer, *CompressedRgbCameraData);
    RgbCameraDataNeedsCompress = false;
  }

  const auto Size = (*CompressedRgbCameraData).Num();
  OutArray.SetNumUninitialized(Size);
  FMemory::Memcpy(OutArray.GetData(), (*CompressedRgbCameraData).GetData(), Size * sizeof(uint8));

  stamp = rgb_stamp_;

  RgbCameraBufferCriticalSection->Unlock();

  return true;
}

bool ADronePawn::GetStereoCameraDataFromServerThread(TArray<uint8>& image_left, TArray<uint8>& image_right, double& stamp) {

  {
    StereoCameraBufferCriticalSection->Lock();

    stereo_camera_last_request_time_ = FPlatformTime::Seconds();

    if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
      auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
      Instruction->Function = [](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(true, CAMERA_MODE_STEREO); };
      InstructionQueue->Enqueue(Instruction);
      FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
    }

    if (!StereoCameraRendered) {
      UE_LOG(LogTemp, Warning, TEXT("DronePawn(): stereo camera is not rendered, returning"));
      StereoCameraBufferCriticalSection->Unlock();
      return false;
    }

    if (StereoCameraDataNeedsCompress) {

      FImageUtils::ThumbnailCompressImageArray(RenderTarget2DStereoLeft->SizeX, RenderTarget2DStereoLeft->SizeY, StereoLeftCameraBuffer,
                                               *CompressedStereoLeftCameraData);
      FImageUtils::ThumbnailCompressImageArray(RenderTarget2DStereoRight->SizeX, RenderTarget2DStereoRight->SizeY, StereoRightCameraBuffer,
                                               *CompressedStereoRightCameraData);
      StereoCameraDataNeedsCompress = false;
    }

    {
      const auto Size = (*CompressedStereoLeftCameraData).Num();
      image_left.SetNumUninitialized(Size);
      FMemory::Memcpy(image_left.GetData(), (*CompressedStereoLeftCameraData).GetData(), Size * sizeof(uint8));
    }

    {
      const auto Size = (*CompressedStereoRightCameraData).Num();
      image_right.SetNumUninitialized(Size);
      FMemory::Memcpy(image_right.GetData(), (*CompressedStereoRightCameraData).GetData(), Size * sizeof(uint8));
    }

    stamp = stereo_stamp_;

    StereoCameraBufferCriticalSection->Unlock();

    return true;
  }
}

//}

/* getCameraSegFromServerThread() //{ */

void ADronePawn::TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor>& SrcData, TArray<uint8>& DstData) {
  // Calculate the memory size required for the image data
  int32 MemorySize = ImageWidth * ImageHeight * 4;  // 4 bytes per pixel (RGBA)

  // Resize the destination array to hold the transformed data
  DstData.SetNumUninitialized(MemorySize);

  // Pointer to the source data (BGRA format)
  const FColor* SrcDataPtr = SrcData.GetData();

  // Pointer to the destination data (RGBA format)
  uint8* DstDataPtr = DstData.GetData();

  // Iterate over each pixel and transform BGRA to RGBA
  for (int32 Index = 0; Index < ImageWidth * ImageHeight; Index++) {
    // Copy and swap channels
    DstDataPtr[Index * 3 + 0] = SrcDataPtr[Index].B;  // Red channel
    DstDataPtr[Index * 3 + 1] = SrcDataPtr[Index].G;  // Green channel
    DstDataPtr[Index * 3 + 2] = SrcDataPtr[Index].R;  // Blue channel
  }
}

bool ADronePawn::GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double& stamp) {

  RgbSegCameraBufferCriticalSection->Lock();

  rgb_seg_camera_last_request_time_ = FPlatformTime::Seconds();

  if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
    auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
    Instruction->Function = [](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(true, CAMERA_MODE_RGB_SEG); };
    InstructionQueue->Enqueue(Instruction);
    FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
  }

  if (!RgbSegCameraRendered) {
    UE_LOG(LogTemp, Warning, TEXT("DronePawn(): rgb seg camera is not rendered, returning"));
    RgbSegCameraBufferCriticalSection->Unlock();
    return false;
  }

  /* if (RgbSegCameraDataNeedsCompress) { */
  /*   FImageUtils::ThumbnailCompressImageArray(RenderTarget2DRgbSeg->SizeX, RenderTarget2DRgbSeg->SizeY, RgbSegCameraBuffer, *CompressedRgbSegCameraData); */
  /*   RgbSegCameraDataNeedsCompress = false; */
  /* } */

  auto byteArray = MakeUnique<TArray<uint8>>();
  ADronePawn::TransformImageArray(RenderTarget2DRgbSeg->SizeX, RenderTarget2DRgbSeg->SizeY, RgbSegCameraBuffer, *byteArray);

  const auto Size = (*byteArray).Num();
  OutArray.SetNumUninitialized(Size);
  FMemory::Memcpy(OutArray.GetData(), (*byteArray).GetData(), Size * sizeof(uint8));

  RgbSegCameraBufferCriticalSection->Unlock();

  stamp = rgb_seg_stamp_;

  return true;
}

//}

/* setCameraCaptureMode() //{ */

void ADronePawn::SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode) {

  // CameraCaptureMode = CaptureMode;

  // if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES || CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_MOVEMENT) {
  //   UpdateCamera(true, CAMERA_MODE_RGB);
  // }
}

//}

/* getLidarConfig() //{ */

FLidarConfig ADronePawn::GetLidarConfig() {
  return LidarConfig;
}

//}

/* setLidarConfig() //{ */

bool ADronePawn::SetLidarConfig(const FLidarConfig& Config) {

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

//}

/* getRgbCameraConfig() //{ */

FRgbCameraConfig ADronePawn::GetRgbCameraConfig() {

  return rgb_camera_config_;
}

//}

/* getStereoCameraConfig() //{ */

FStereoCameraConfig ADronePawn::GetStereoCameraConfig() {

  return stereo_camera_config_;
}

//}

/* setRgbCameraConfig() //{ */

bool ADronePawn::SetRgbCameraConfig(const FRgbCameraConfig& Config) {

  UE_LOG(LogTemp, Warning, TEXT("ADronePawn::SetRgbCameraConfig"));

  rgb_camera_config_ = Config;

  RgbCameraBufferCriticalSection->Lock();
  RgbSegCameraBufferCriticalSection->Lock();

  SceneCaptureMeshHolderRgb->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderRgb->SetRelativeLocation(Config.Offset);
  SceneCaptureMeshHolderRgb->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderRgb->SetRelativeRotation(Config.Orientation);

  SceneCaptureComponent2DRgb->FOVAngle = Config.FOVAngle;

  SceneCaptureComponent2DRgbSeg->FOVAngle = Config.FOVAngle;

  if (Config.enable_hdr) {
    SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorHDR;
  } else {
    SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorLDR;
  }

  SceneCaptureComponent2DRgb->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

  SceneCaptureComponent2DRgb->bUseRayTracingIfEnabled = Config.enable_raytracing;

  SceneCaptureComponent2DRgb->TextureTarget                        = RenderTarget2DRgb;
  SceneCaptureComponent2DRgb->bAlwaysPersistRenderingState         = true;
  SceneCaptureComponent2DRgb->bCaptureEveryFrame                   = false;
  SceneCaptureComponent2DRgb->bCaptureOnMovement                   = false;
  SceneCaptureComponent2DRgb->PostProcessSettings.MotionBlurAmount = Config.motion_blur_amount;
  SceneCaptureComponent2DRgb->PostProcessSettings.MotionBlurMax    = Config.motion_blur_distortion;
  SceneCaptureComponent2DRgb->ShowFlags.SetMotionBlur(Config.enable_motion_blur);

  SceneCaptureComponent2DRgbSeg->TextureTarget                = RenderTarget2DRgbSeg;
  SceneCaptureComponent2DRgbSeg->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DRgbSeg->bCaptureOnMovement           = false;
  SceneCaptureComponent2DRgbSeg->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);


  if (Config.Width > 0 && Config.Height > 0) {
    RenderTarget2DRgb->ResizeTarget(Config.Width, Config.Height);
    RenderTarget2DRgbSeg->ResizeTarget(Config.Width, Config.Height);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
  }

  RgbCameraBufferCriticalSection->Unlock();
  RgbSegCameraBufferCriticalSection->Unlock();

  return true;
}

//}

/* setStereoCameraConfig() //{ */

bool ADronePawn::SetStereoCameraConfig(const FStereoCameraConfig& Config) {

  UE_LOG(LogTemp, Warning, TEXT("ADronePawn::SetStereoCameraConfig"));

  stereo_camera_config_ = Config;

  StereoCameraBufferCriticalSection->Lock();

  SceneCaptureMeshHolderStereoLeft->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderStereoLeft->SetRelativeLocation(Config.Offset);
  SceneCaptureMeshHolderStereoLeft->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderStereoLeft->SetRelativeRotation(Config.Orientation);

  SceneCaptureMeshHolderStereoRight->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderStereoRight->SetRelativeLocation(Config.Offset + FVector(0.0, 100.0 * Config.baseline, 0.0));
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(Config.Orientation);

  SceneCaptureComponent2DStereoLeft->FOVAngle  = Config.FOVAngle;
  SceneCaptureComponent2DStereoRight->FOVAngle = Config.FOVAngle;

  if (Config.enable_hdr) {
    SceneCaptureComponent2DStereoLeft->CaptureSource  = SCS_FinalColorHDR;
    SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorHDR;
  } else {
    SceneCaptureComponent2DStereoLeft->CaptureSource  = SCS_FinalColorLDR;
    SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorLDR;
  }

  SceneCaptureComponent2DStereoLeft->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);
  SceneCaptureComponent2DStereoRight->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

  SceneCaptureComponent2DStereoLeft->bUseRayTracingIfEnabled  = Config.enable_raytracing;
  SceneCaptureComponent2DStereoRight->bUseRayTracingIfEnabled = Config.enable_raytracing;

  if (Config.Width > 0 && Config.Height > 0) {
    RenderTarget2DStereoLeft->ResizeTarget(Config.Width, Config.Height);
    RenderTarget2DStereoRight->ResizeTarget(Config.Width, Config.Height);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
  }

  StereoCameraBufferCriticalSection->Unlock();

  return true;
}

//}

/* setLocation() //{ */

void ADronePawn::SetLocation(FVector& Location, FVector& TeleportedToLocation, bool CheckCollisions, FHitResult& HitResult) {

  bool DrawLine      = DrawMovementLine;
  auto StartLocation = DrawLine ? GetActorLocation() : FVector();

  SetActorLocation(Location, CheckCollisions, &HitResult);
  TeleportedToLocation = CheckCollisions ? GetActorLocation() : Location;

  if (DrawLine) {
    DrawDebugLine(GetWorld(), StartLocation, TeleportedToLocation, FColor::Green, true, -1, 0, 3);
  }
}

//}

/* tick() //{ */


void ADronePawn::Tick(float DeltaSeconds) {

  auto start = FPlatformTime::Seconds();

  std::shared_ptr<FInstruction<ADronePawn>> Instruction;

  while (InstructionQueue->Dequeue(Instruction)) {
    Instruction->Function(*this);
    Instruction->Finished = true;
  }

  if (propellers_rotate) 
  {
      float RotationSpeed = 2000.0f * DeltaSeconds; 

      for (int i = 0; i < DynamicPropellers.Num(); ++i)
      {
          UStaticMeshComponent* Prop = DynamicPropellers[i];
          if(Prop && Prop->IsVisible() && !Prop->IsBeingDestroyed()) 
          {
              float CurrentRotationSpeed = (i % 2 == 0) ? RotationSpeed : -RotationSpeed; 
               Prop->AddLocalRotation(FRotator(0, CurrentRotationSpeed, 0), false, nullptr, ETeleportType::None); 
          }
      }
  }
  {
    RgbCameraBufferCriticalSection->Lock();

    if ((FPlatformTime::Seconds() - rgb_camera_last_request_time_) > 1.0) {
      if (SceneCaptureComponent2DRgb->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgb->bCaptureEveryFrame = false;
        RgbCameraRendered                              = false;
        UE_LOG(LogTemp, Warning, TEXT("Turning off RGB capture"));
      }
    } else {
      if (!SceneCaptureComponent2DRgb->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgb->bCaptureEveryFrame = true;
        UE_LOG(LogTemp, Warning, TEXT("Turning on RGB capture"));
      } else {
        UpdateCamera(true, CAMERA_MODE_RGB, start);
      }
    }

    RgbCameraBufferCriticalSection->Unlock();
  }

  {
    RgbSegCameraBufferCriticalSection->Lock();

    if ((FPlatformTime::Seconds() - rgb_seg_camera_last_request_time_) > 1.0) {
      if (SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame = false;
        RgbSegCameraRendered                              = false;
        UE_LOG(LogTemp, Warning, TEXT("Turning off RGB segmented capture"));
      }
    } else {
      if (!SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame = true;
        UE_LOG(LogTemp, Warning, TEXT("Turning on RGB segmented capture"));
      } else {
        UpdateCamera(true, CAMERA_MODE_RGB_SEG, start);
      }
    }

    RgbSegCameraBufferCriticalSection->Unlock();
  }

  {
    StereoCameraBufferCriticalSection->Lock();

    if ((FPlatformTime::Seconds() - stereo_camera_last_request_time_) > 1.0) {
      if (SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame) {
        SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame  = false;
        SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = false;
        StereoCameraRendered                                   = false;
        UE_LOG(LogTemp, Warning, TEXT("Turning off Stereo capture"));
      }
    } else {
      if (!SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame) {
        UE_LOG(LogTemp, Warning, TEXT("Turning on Stereo capture"));
        SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame  = true;
        SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = true;
      } else {
        UpdateCamera(true, CAMERA_MODE_STEREO, start);
      }
    }

    StereoCameraBufferCriticalSection->Unlock();
  }

  Super::Tick(DeltaSeconds);

  // UE_LOG(LogTemp, Warning, TEXT("Tick(), delta %f, took %f s"), DeltaSeconds, FPlatformTime::Seconds() - start);
}

//}
