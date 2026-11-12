#include "SkyControl.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Misc/PackageName.h"
#include "UObject/StructOnScope.h"

namespace
{

/* reflection helpers //{ */

FString Squashed(const FString& Name)
{
	FString Out;
	for (const TCHAR C : Name)
	{
		if (FChar::IsAlnum(C))
		{
			Out.AppendChar(FChar::ToLower(C));
		}
	}
	return Out;
}

// Blueprint members are named as displayed ("Time of Day"), so only letters and digits are compared
FProperty* FindPropertyNamed(const UStruct* Struct, const TCHAR* Name)
{
	const FString Wanted = Squashed(Name);
	for (TFieldIterator<FProperty> It(Struct); It; ++It)
	{
		if (Squashed(It->GetName()) == Wanted)
		{
			return *It;
		}
	}
	return nullptr;
}

UFunction* FindFunctionNamed(const UClass* Class, const TCHAR* Name)
{
	const FString Wanted = Squashed(Name);
	for (TFieldIterator<UFunction> It(Class); It; ++It)
	{
		if (Squashed(It->GetName()) == Wanted)
		{
			return *It;
		}
	}
	return nullptr;
}

bool SetNumber(UObject* Object, const TCHAR* Name, const double Value)
{
	FNumericProperty* Property = CastField<FNumericProperty>(FindPropertyNamed(Object->GetClass(), Name));
	if (!Property)
	{
		return false;
	}

	void* Address = Property->ContainerPtrToValuePtr<void>(Object);
	if (Property->IsFloatingPoint())
	{
		Property->SetFloatingPointPropertyValue(Address, Value);
	}
	else
	{
		Property->SetIntPropertyValue(Address, static_cast<int64>(FMath::RoundToDouble(Value)));
	}
	return true;
}

bool GetNumber(UObject* Object, const TCHAR* Name, double& Value)
{
	const FNumericProperty* Property = CastField<FNumericProperty>(FindPropertyNamed(Object->GetClass(), Name));
	if (!Property)
	{
		return false;
	}

	const void* Address = Property->ContainerPtrToValuePtr<void>(Object);
	Value = Property->IsFloatingPoint() ? Property->GetFloatingPointPropertyValue(Address) : static_cast<double>(Property->GetSignedIntPropertyValue(Address));
	return true;
}

bool GetBool(UObject* Object, const TCHAR* Name)
{
	const FBoolProperty* Property = CastField<FBoolProperty>(FindPropertyNamed(Object->GetClass(), Name));
	return Property && Property->GetPropertyValue_InContainer(Object);
}

void SetBool(UObject* Object, const TCHAR* Name, const bool bValue)
{
	if (FBoolProperty* Property = CastField<FBoolProperty>(FindPropertyNamed(Object->GetClass(), Name)))
	{
		Property->SetPropertyValue_InContainer(Object, bValue);
	}
}

// subclasses count: a level may place a child Blueprint of the sky
AActor* FindActorOfClassNamed(UWorld* World, const TCHAR* ClassName)
{
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		for (const UClass* Class = It->GetClass(); Class; Class = Class->GetSuperClass())
		{
			if (Class->GetName() == ClassName)
			{
				return *It;
			}
		}
	}
	return nullptr;
}

/* //} */

// flightforge.wire.Weather order
constexpr const TCHAR* WeatherPresets[] = {
    TEXT("Clear_Skies"), TEXT("Cloudy"),         TEXT("Foggy"), TEXT("Rain"),          TEXT("Rain_Light"), TEXT("Rain_Thunderstorm"),
    TEXT("Sand_Dust_Calm"), TEXT("Sand_Dust_Storm"), TEXT("Snow"), TEXT("Snow_Blizzard"), TEXT("Overcast"),
};

}  // namespace

/* SetDaytime() //{ */

bool USkyControl::SetDaytime(const int32 Hour, const int32 Minute, FString& Error)
{
	if (Hour < 0 || Hour > 23 || Minute < 0 || Minute > 59)
	{
		Error = FString::Printf(TEXT("%d:%02d is not a time of day"), Hour, Minute);
		return false;
	}

	UWorld*      World = GetWorld();
	const double Hours = Hour + Minute / 60.0;

	if (AActor* Sky = FindActorOfClassNamed(World, TEXT("Ultra_Dynamic_Sky_C")))
	{
		// 100 per hour
		if (!SetNumber(Sky, TEXT("Time of Day"), Hours * 100.0))
		{
			Error = FString::Printf(TEXT("%s has no Time of Day property"), *Sky->GetName());
			return false;
		}
		UE_LOG(LogTemp, Display, TEXT("FlightForge sky: %02d:%02d through %s"), Hour, Minute, *Sky->GetClass()->GetName());
		return true;
	}

	if (AActor* SunSky = FindActorOfClassNamed(World, TEXT("SunSky_C")))
	{
		UFunction* UpdateSun = FindFunctionNamed(SunSky->GetClass(), TEXT("UpdateSun"));
		if (!SetNumber(SunSky, TEXT("SolarTime"), Hours) || !UpdateSun || UpdateSun->ParmsSize > 0)
		{
			Error = FString::Printf(TEXT("%s has no SolarTime property or UpdateSun function"), *SunSky->GetName());
			return false;
		}
		SunSky->ProcessEvent(UpdateSun, nullptr);
		UE_LOG(LogTemp, Display, TEXT("FlightForge sky: %02d:%02d through %s"), Hour, Minute, *SunSky->GetClass()->GetName());
		return true;
	}

	const bool bOk = SetLightsDaytime(Hours, Error);
	if (bOk)
	{
		const UDirectionalLightComponent* Sun = Baseline.Sun.Get();
		const TCHAR* SunKind = !Sun                                          ? TEXT("no")
		                       : Sun->Mobility == EComponentMobility::Movable    ? TEXT("a movable")
		                       : Sun->Mobility == EComponentMobility::Stationary ? TEXT("a stationary")
		                                                                         : TEXT("a static, untouched,");
		UE_LOG(LogTemp, Display, TEXT("FlightForge sky: %02d:%02d through the level's lights (%s sun, %d sky lights)"), Hour, Minute, SunKind,
		       Baseline.SkyLights.Num());
	}
	return bOk;
}

/* //} */

/* SetDaytimeAnimated() //{ */

bool USkyControl::SetDaytimeAnimated(const bool bAnimate, FString& Error)
{
	UWorld* World = GetWorld();
	if (AActor* UltraSky = FindActorOfClassNamed(World, TEXT("Ultra_Dynamic_Sky_C")))
	{
		if (!FindPropertyNamed(UltraSky->GetClass(), TEXT("Animate Time of Day")))
		{
			Error = FString::Printf(TEXT("%s has no Animate Time of Day property"), *UltraSky->GetName());
			return false;
		}
		SetBool(UltraSky, TEXT("Animate Time of Day"), bAnimate);
		double DayMinutes = 0.0, NightMinutes = 0.0;
		GetNumber(UltraSky, TEXT("Day Length"), DayMinutes);
		GetNumber(UltraSky, TEXT("Night Length"), NightMinutes);
		UE_LOG(LogTemp, Display, TEXT("FlightForge sky: day cycle %s through %s (day %g min, night %g min)"), bAnimate ? TEXT("on") : TEXT("off"),
		       *UltraSky->GetClass()->GetName(), DayMinutes, NightMinutes);
		return true;
	}

	if (bAnimate)
	{
		double  Hours     = 0.0;
		bool    bAnimated = false;
		FString Sky;
		if (GetDaytime(Hours, bAnimated, Sky, Error))
		{
			Error = FString::Printf(TEXT("this world's sky (%s) has no day cycle of its own; only Ultra Dynamic Sky animates time of day"), *Sky);
		}
		return false;
	}
	return true;
}

/* //} */

/* LightsReachCameras() //{ */

bool USkyControl::LightsReachCameras(FString& Error)
{
	UWorld* World = GetWorld();

	if (!Baseline.bTaken)
	{
		Baseline.bTaken = true;

		UDirectionalLightComponent* Sun = nullptr;
		for (TObjectIterator<UDirectionalLightComponent> It; It; ++It)
		{
			if (It->GetWorld() != World || !It->IsRegistered())
			{
				continue;
			}
			const bool bAtmosphereSun = It->IsUsedAsAtmosphereSunLight() && It->GetAtmosphereSunLightIndex() == 0;
			if (!Sun || bAtmosphereSun || (!Sun->IsUsedAsAtmosphereSunLight() && It->Intensity > Sun->Intensity))
			{
				Sun = *It;
				if (bAtmosphereSun)
				{
					break;
				}
			}
		}

		if (Sun)
		{
			Baseline.Sun          = Sun;
			Baseline.SunRotation  = Sun->GetComponentRotation();
			Baseline.SunIntensity = Sun->Intensity;
		}

		for (TObjectIterator<USkyLightComponent> It; It; ++It)
		{
			if (It->GetWorld() == World && It->IsRegistered() && It->Mobility != EComponentMobility::Static)
			{
				Baseline.SkyLights.Emplace(*It, It->Intensity);
			}
		}
	}

	// a hidden light lights nothing, as the cave's sun and sky light are
	const auto CanLight = [](const ULightComponentBase* Light) {
		return Light && Light->Mobility != EComponentMobility::Static && Light->IsVisible() && Light->bAffectsWorld;
	};

	bool bCanChange = CanLight(Baseline.Sun.Get());
	for (const TPair<TWeakObjectPtr<USkyLightComponent>, float>& Entry : Baseline.SkyLights)
	{
		bCanChange |= CanLight(Entry.Key.Get());
	}
	if (!bCanChange)
	{
		Error = FString::Printf(TEXT("this world %s: no Ultra_Dynamic_Sky or SunSky actor and no visible directional or sky light that can change at runtime"),
		                        NoSkySystem);
	}
	return bCanChange;
}

/* //} */

/* SetLightsDaytime() //{ */

bool USkyControl::SetLightsDaytime(const double Hours, FString& Error)
{
	if (!LightsReachCameras(Error))
	{
		return false;
	}

	// the sun rises at 6:00 in the light's own azimuth, stands overhead at 12:00 and sets at 18:00
	const double Elevation = (Hours - 6.0) * 15.0;
	const double SinElev   = FMath::Sin(FMath::DegreesToRadians(Elevation));
	const double SkyScale  = FMath::Clamp(SinElev * 4.0 + 0.2, 0.02, 1.0);

	UDirectionalLightComponent* Sun = Baseline.Sun.Get();
	if (Sun && Sun->Mobility == EComponentMobility::Movable)
	{
		Sun->SetWorldRotation(FRotator(-Elevation, Baseline.SunRotation.Yaw, Baseline.SunRotation.Roll));
	}
	if (Sun && Sun->Mobility != EComponentMobility::Static)
	{
		Sun->SetIntensity(SinElev > 0.0 ? Baseline.SunIntensity : 0.0f);
	}

	for (const TPair<TWeakObjectPtr<USkyLightComponent>, float>& Entry : Baseline.SkyLights)
	{
		if (USkyLightComponent* SkyLight = Entry.Key.Get())
		{
			SkyLight->SetIntensity(Entry.Value * SkyScale);
			if (!SkyLight->bRealTimeCapture && SkyLight->SourceType == SLS_CapturedScene)
			{
				SkyLight->RecaptureSky();
			}
		}
	}

	LightsHours = Hours;
	return true;
}

/* //} */

/* GetDaytime() //{ */

bool USkyControl::GetDaytime(double& Hours, bool& bAnimated, FString& Sky, FString& Error)
{
	UWorld* World = GetWorld();
	bAnimated     = false;

	if (AActor* UltraSky = FindActorOfClassNamed(World, TEXT("Ultra_Dynamic_Sky_C")))
	{
		double TimeOfDay = 0.0;
		if (!GetNumber(UltraSky, TEXT("Time of Day"), TimeOfDay))
		{
			Error = FString::Printf(TEXT("%s has no Time of Day property"), *UltraSky->GetName());
			return false;
		}
		// 100 per hour, past 2400 until the sky rolls the date over
		Hours     = FMath::Fmod(TimeOfDay / 100.0, 24.0);
		bAnimated = GetBool(UltraSky, TEXT("Animate Time of Day"));
		Sky       = TEXT("ultra_dynamic_sky");
		return true;
	}

	if (AActor* SunSky = FindActorOfClassNamed(World, TEXT("SunSky_C")))
	{
		if (!GetNumber(SunSky, TEXT("SolarTime"), Hours))
		{
			Error = FString::Printf(TEXT("%s has no SolarTime property"), *SunSky->GetName());
			return false;
		}
		Sky = TEXT("sun_sky");
		return true;
	}

	if (!LightsReachCameras(Error))
	{
		return false;
	}
	Hours = LightsHours;
	Sky   = TEXT("lights");
	return true;
}

/* //} */

/* SetWeather() //{ */

bool USkyControl::SetWeather(const int32 Weather, FString& Error)
{
	if (Weather < 0 || Weather >= static_cast<int32>(UE_ARRAY_COUNT(WeatherPresets)))
	{
		Error = FString::Printf(TEXT("%d is not a weather type"), Weather);
		return false;
	}

	AActor* WeatherActor = FindActorOfClassNamed(GetWorld(), TEXT("Ultra_Dynamic_Weather_C"));
	if (!WeatherActor)
	{
		Error = FString::Printf(TEXT("this world %s for weather: no Ultra_Dynamic_Weather actor"), NoSkySystem);
		return false;
	}

	UFunction* ChangeWeather = FindFunctionNamed(WeatherActor->GetClass(), TEXT("Change Weather"));
	if (!ChangeWeather)
	{
		Error = FString::Printf(TEXT("%s has no Change Weather function"), *WeatherActor->GetName());
		return false;
	}

	// the presets ship beside the weather Blueprint
	const FString PresetName = WeatherPresets[Weather];
	const FString Folder     = FPackageName::GetLongPackagePath(WeatherActor->GetClass()->GetPackage()->GetName());
	const FString PresetPath = FString::Printf(TEXT("%s/Weather_Effects/Weather_Presets/%s.%s"), *Folder, *PresetName, *PresetName);
	UObject*      Preset     = LoadObject<UObject>(nullptr, *PresetPath, nullptr, LOAD_NoWarn);
	if (!Preset)
	{
		Error = FString::Printf(TEXT("the weather preset %s is not in this build"), *PresetPath);
		return false;
	}

	FStructOnScope Params(ChangeWeather);
	bool           bPresetSet = false;
	for (TFieldIterator<FProperty> It(ChangeWeather); It && It->HasAnyPropertyFlags(CPF_Parm); ++It)
	{
		if (It->HasAnyPropertyFlags(CPF_ReturnParm | CPF_OutParm))
		{
			continue;
		}
		if (FObjectPropertyBase* Object = CastField<FObjectPropertyBase>(*It); Object && Preset->IsA(Object->PropertyClass))
		{
			Object->SetObjectPropertyValue_InContainer(Params.GetStructMemory(), Preset);
			bPresetSet = true;
		}
		else if (FNumericProperty* Seconds = CastField<FNumericProperty>(*It); Seconds && Seconds->IsFloatingPoint())
		{
			Seconds->SetFloatingPointPropertyValue(Seconds->ContainerPtrToValuePtr<void>(Params.GetStructMemory()), 0.0);
		}
	}

	if (!bPresetSet)
	{
		Error = FString::Printf(TEXT("%s's Change Weather takes no %s"), *WeatherActor->GetName(), *Preset->GetClass()->GetName());
		return false;
	}

	// a random variation would wander off the asked weather
	SetBool(WeatherActor, TEXT("Use Random Weather Variation"), false);
	WeatherActor->ProcessEvent(ChangeWeather, Params.GetStructMemory());
	UE_LOG(LogTemp, Display, TEXT("FlightForge sky: weather %s through %s"), *PresetName, *WeatherActor->GetClass()->GetName());
	return true;
}

/* //} */
