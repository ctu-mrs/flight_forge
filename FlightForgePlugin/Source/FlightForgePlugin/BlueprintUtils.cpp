// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintUtils.h"

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
			*GateData.GateName, GateData.Vector.X, GateData.Vector.Y, GateData.Vector.Z, GateData.Heading);
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
