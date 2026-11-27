#include "DeviceMeshLibrary.h"

#include "Async/Async.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "glTFRuntimeAsset.h"
#include "glTFRuntimeParser.h"

/* UDeviceMeshLoad //{ */

void UDeviceMeshLoad::Begin(const FString& InPath)
{
	Path  = InPath;
	Start = FPlatformTime::Seconds();

	TWeakObjectPtr<UDeviceMeshLoad> WeakThis(this);
	const FglTFRuntimeConfig        LoaderConfig;

	// the parser is plain C++ and safe to build on a worker; the asset and mesh are UObjects and stay on the game thread
	Async(EAsyncExecution::Thread, [WeakThis, InPath, LoaderConfig]() {
		TSharedPtr<FglTFRuntimeParser> Parser = FglTFRuntimeParser::FromFilename(InPath, LoaderConfig);

		AsyncTask(ENamedThreads::GameThread, [WeakThis, Parser]() {
			UDeviceMeshLoad* Self = WeakThis.Get();
			if (!Self)
			{
				return;
			}

			if (!Parser.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("DeviceMesh: '%s' could not be parsed as glTF"), *Self->Path);
				return;
			}

			Self->Asset = NewObject<UglTFRuntimeAsset>(Self);
			Self->Asset->SetParser(Parser.ToSharedRef());

			FglTFRuntimeStaticMeshConfig MeshConfig;
			MeshConfig.Outer = Self;

			FglTFRuntimeStaticMeshAsync Done;
			Done.BindDynamic(Self, &UDeviceMeshLoad::OnMesh);
			Self->Asset->LoadStaticMeshRecursiveAsync(FString(), TArray<FString>(), Done, MeshConfig);
		});
	});
}

void UDeviceMeshLoad::Add(const FTarget& Target)
{
	if (Mesh)
	{
		Apply(Target);
	}
	else
	{
		Targets.Add(Target);
	}
}

void UDeviceMeshLoad::OnMesh(UStaticMesh* StaticMesh)
{
	if (!StaticMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("DeviceMesh: no static mesh could be built from '%s'"), *Path);
		Targets.Empty();
		return;
	}

	Mesh = StaticMesh;
	UE_LOG(LogTemp, Log, TEXT("DeviceMesh: %s imported in %.2f s, half extents %s cm"), *FPaths::GetCleanFilename(Path), FPlatformTime::Seconds() - Start,
	       *Mesh->GetBounds().BoxExtent.ToCompactString());

	for (const FTarget& Target : Targets)
	{
		Apply(Target);
	}
	Targets.Empty();
}

void UDeviceMeshLoad::Apply(const FTarget& Target) const
{
	if (UStaticMeshComponent* Component = Target.Component.Get())
	{
		Component->SetStaticMesh(Mesh);
		Component->SetRelativeScale3D(FVector::OneVector);
		Component->SetRelativeLocation(Target.Offset);
		Component->SetRelativeRotation(Target.Rotation);
	}
}

//}

/* UDeviceMeshLibrary //{ */

void UDeviceMeshLibrary::Request(const FString& Path, UStaticMeshComponent* Component, const FVector& Offset, const FRotator& Rotation)
{
	TObjectPtr<UDeviceMeshLoad>& Load = Loads.FindOrAdd(Path);

	if (!Load)
	{
		Load = NewObject<UDeviceMeshLoad>(this);
		Load->Begin(Path);
	}

	Load->Add({Component, Offset, Rotation});
}

//}
