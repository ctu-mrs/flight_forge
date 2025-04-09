// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintUtils.h"

// yaml-cpp libs
#include "Node.h"
#include "Parsing.h"

void UBlueprintUtils::WriteGateTransformToCsv(const FString& Filename, const TArray<FString>& GateNames, const TArray<FVector>& Vectors, const TArray<float>& Heading, bool& bOutResult, FString& OutString)
{
	if (GateNames.Num() != Vectors.Num() || Vectors.Num() != Heading.Num())
	{
		bOutResult = false;
		OutString = "Array sizes do not match.";
		return;
	}

	// Combine all data into a sortable array


	TArray<FGateData> GateDataArray;
	for (int i = 0; i < GateNames.Num(); i++)
	{
		GateDataArray.Add({ GateNames[i], Vectors[i], Heading[i] });
	}

	// Sort by GateName
	GateDataArray.Sort([](const FGateData& A, const FGateData& B)
	{
		return A.GateName < B.GateName;
	});
	
	FString Csv2Print;
	Csv2Print.Appendf(TEXT("GATE_NAME, X[m], Y[m], Z[m], HEADING[deg] \r\n"));

	for (auto GateData : GateDataArray)
	{
		Csv2Print.Appendf(TEXT("%s, %.3lf, %.3lf, %.3lf, %.3lf\r\n"),
			//*GateNames[i], Vectors[i].X, Vectors[i].Y, Vectors[i].Z, Heading[i]);
			*GateData.GateName, GateData.Location.X, GateData.Location.Y, GateData.Location.Z, GateData.Heading);
	}

	FString FilePath = FPaths::ProjectContentDir() + Filename + ".csv";
	if(!FFileHelper::SaveStringToFile(Csv2Print,*FilePath))
	{
		bOutResult = false;
		OutString = "Failed to write to file.";
	}

	bOutResult = true;
	OutString = "Finished writing to file.";
}

void UBlueprintUtils::ReadYamlFromFile(const FString& FilePath)
{
	FYamlNode Node;
	UYamlParsing::LoadYamlFromFile(FPaths::ProjectContentDir() + "example1.yaml", Node);  // Load and Parse from 
	
	FString filePath = FString(FPaths::ProjectContentDir() + "example1.yaml");
	UE_LOG(LogTemp, Log, TEXT("Try load YAML Node from: %s"), *filePath);

	// Accessing and converting an attribute
	FString Message = Node["message"].As<FString>();
	int32 Answer =    Node["answer"].As<int32>();
	FVector Origin =  Node["origin"].As<FVector>(FVector::ForwardVector);  // User ForwardVector if the conversion is not possible

	// // Iterate over Fibonacci's
	// int32 Sum = 0;
	// for (const auto& Fib : Node["fibonacci"]) {
	// 	Sum += Fib.As<int32>(0);  // "The End!" cannot be converted, so 0 will be used
	// }
	//
	UE_LOG(LogTemp, Log, TEXT("%d"), Answer);
}

void UBlueprintUtils::ReadGatesTransformFromYaml(const FString& FilePath, TArray<FGateDataYaml>& OutGatesData)
{
	FString FullPath = FPaths::ProjectContentDir() + FilePath;

	if (!FPaths::FileExists(FullPath))
	{
		UE_LOG(LogTemp, Error, TEXT("YAML file not found: %s"), *FullPath);
		return;
	}

	FString FileContent;
	if (FFileHelper::LoadFileToString(FileContent, *FullPath))
	{
		YAML::Node RootNode = YAML::Load(TCHAR_TO_UTF8(*FileContent));

		if (RootNode["gates"])
		{
			for (const auto& GateNode : RootNode["gates"])
			{
				FGateDataYaml Gate;
				Gate.Position.X = GateNode["position"]["x"].as<float>();
				Gate.Position.Y = GateNode["position"]["y"].as<float>();
				Gate.Position.Z = GateNode["position"]["z"].as<float>();

				Gate.Orientation.Roll = GateNode["orientation"]["roll"].as<float>();
				Gate.Orientation.Pitch = GateNode["orientation"]["pitch"].as<float>();
				Gate.Orientation.Yaw = GateNode["orientation"]["yaw"].as<float>();

				// UE_LOG(LogTemp, Warning, TEXT("Loaded position:  [%lf,%lf,%lf] Orientation: [%lf,%lf,%lf]"),
				// 	Gate.Position.X,Gate.Position.Y,Gate.Position.Z, Gate.Orientation.Roll, Gate.Orientation.Pitch, Gate.Orientation.Yaw);

				OutGatesData.Add(Gate);
			}

			UE_LOG(LogTemp, Log, TEXT("Successfully loaded %d gates"), OutGatesData.Num());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No 'gates' section found in YAML file"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to read YAML file"));
	}
}

void UBlueprintUtils::TransformToUECoord(const FVector& RightHandLocation, const FVector& RightHandOrientation, const FVector& WorldOrigin, FTransform& Out_UETransform)
{
	float PlayerStartOffset = 92.12;
	
	FVector LocationUE;
	LocationUE.X = WorldOrigin.X + RightHandLocation.X * 100;
	LocationUE.Y = WorldOrigin.Y - RightHandLocation.Y * 100;
	LocationUE.Z = WorldOrigin.Z + RightHandLocation.Z * 100 - PlayerStartOffset;
	
	FRotator OrientationUE;
	OrientationUE.Roll = RightHandOrientation.X; //180 * (RightHandOrientation.X / PI);
	OrientationUE.Pitch = -RightHandOrientation.Y; //180 * (-RightHandOrientation.Y / PI);
	OrientationUE.Yaw = -RightHandOrientation.Z; //180 * (-RightHandOrientation.Z / PI);

	// UE_LOG(LogTemp, Warning, TEXT("Heading: %lf [right hand], %lf [UE]"), RightHandOrientation.Z, OrientationUE.Yaw);

	Out_UETransform.SetLocation(LocationUE);
	Out_UETransform.SetRotation(OrientationUE.Quaternion());
}
