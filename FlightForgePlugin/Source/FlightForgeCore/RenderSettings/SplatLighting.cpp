#include "SplatLighting.h"

#include "Components/DirectionalLightComponent.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "UObject/UObjectHash.h"
#include "UObject/UObjectIterator.h"

/* console variables //{ */

namespace
{

TAutoConsoleVariable<int32> CVarSplatRelight(TEXT("FlightForge.SplatRelight"), -1,
                                             TEXT("ModelForge splats lit by the level's sun and sky light: 1 on, 0 off, -1 as the map sets it (-FlightForgeSplatRelight=)"));

TAutoConsoleVariable<float> CVarSunPitch(TEXT("FlightForge.SunPitch"), 1000.0f,
                                         TEXT("the level sun's pitch in degrees (-90 noon, -5 low); >= 360 leaves it as the map sets it (-FlightForgeSunPitch=)"));

const TCHAR* SplatClassPath = TEXT("/Script/ModelForgeSplat.ModelForgeSplatComponent");

void ReadCommandLineOnce()
{
	static bool bRead = false;
	if (bRead)
	{
		return;
	}
	bRead = true;

	int32 Relight = 0;
	if (FParse::Value(FCommandLine::Get(), TEXT("FlightForgeSplatRelight="), Relight))
	{
		CVarSplatRelight->Set(Relight, ECVF_SetByCommandline);
	}
	float Pitch = 0.0f;
	if (FParse::Value(FCommandLine::Get(), TEXT("FlightForgeSunPitch="), Pitch))
	{
		CVarSunPitch->Set(Pitch, ECVF_SetByCommandline);
	}
}

}  // namespace

//}

/* USplatLighting //{ */

bool USplatLighting::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void USplatLighting::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	ReadCommandLineOnce();
	bStarted = true;
}

TStatId USplatLighting::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(USplatLighting, STATGROUP_Tickables);
}

void USplatLighting::Tick(float DeltaTime)
{
	// every tick: splats a scene loader adds later get the setting too
	ApplyRelight(CVarSplatRelight.GetValueOnGameThread());
	ApplySunPitch(CVarSunPitch.GetValueOnGameThread());
}

void USplatLighting::ApplyRelight(const int32 Relight)
{
	static UClass* SplatClass = FindObject<UClass>(nullptr, SplatClassPath);
	if (Relight < 0 || !SplatClass)
	{
		return;
	}
	const FBoolProperty* Property = FindFProperty<FBoolProperty>(SplatClass, TEXT("bRelight"));
	if (!Property)
	{
		return;
	}

	TArray<UObject*> Splats;
	GetObjectsOfClass(SplatClass, Splats, true, RF_ClassDefaultObject);
	for (UObject* Splat : Splats)
	{
		if (Splat->GetWorld() == GetWorld() && Property->GetPropertyValue_InContainer(Splat) != (Relight > 0))
		{
			Property->SetPropertyValue_InContainer(Splat, Relight > 0);
			UE_LOG(LogTemp, Display, TEXT("FlightForge: relight %s on %s"), Relight > 0 ? TEXT("on") : TEXT("off"), *Splat->GetPathName());
		}
	}
}

void USplatLighting::ApplySunPitch(const float Pitch)
{
	if (Pitch >= 360.0f)
	{
		return;
	}

	if (!Sun.IsValid())
	{
		for (TObjectIterator<UDirectionalLightComponent> It; It; ++It)
		{
			if (It->GetWorld() == GetWorld() && It->IsRegistered() && (!Sun.IsValid() || It->IsUsedAsAtmosphereSunLight()))
			{
				Sun = *It;
			}
		}
	}
	// held every tick: a ModelForge splat turns the sun to its capture's on its first tick
	if (!Sun.IsValid() || FMath::IsNearlyEqual(Sun->GetComponentRotation().Pitch, Pitch, 0.01))
	{
		return;
	}

	// a static or stationary light ignores a rotation at runtime
	Sun->SetMobility(EComponentMobility::Movable);
	FRotator Rotation = Sun->GetComponentRotation();
	Rotation.Pitch    = Pitch;
	Sun->SetWorldRotation(Rotation);
	UE_LOG(LogTemp, Display, TEXT("FlightForge: sun %s pitch %.1f"), *Sun->GetPathName(), Pitch);
}

//}
