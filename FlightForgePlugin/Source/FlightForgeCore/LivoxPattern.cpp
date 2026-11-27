#include "LivoxPattern.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/ScopeLock.h"

/* .ffpat layout //{ */

// Written by Content/Lidar/convert_livox_csv.py. Angles are already converted
// to UE conventions there (elevation = 90 - zenith, azimuth negated for the
// left-handed frame) and baked into unit direction vectors.
namespace
{
constexpr uint32 PatternMagic   = 0x54504646;  // 'F','F','P','T' little-endian
constexpr uint32 PatternVersion = 1;

#pragma pack(push, 1)
struct FPatternHeader
{
  uint32 Magic;
  uint32 Version;
  uint32 Count;
  uint32 Reserved;
  double PointRate;
  double Period;
  double MinRangeCm;
  double MaxRangeCm;
};
#pragma pack(pop)

static_assert(sizeof(FPatternHeader) == 48, "unexpected .ffpat header padding");
}  // namespace

//}

/* LoadPattern() //{ */

namespace
{
bool LoadPattern(const FString& SensorName, FLivoxPattern& Out) {

  // Sensor names come off the wire and are pasted into a path.
  if (SensorName.IsEmpty() || !FPaths::ValidatePath(SensorName) || SensorName.Contains(TEXT("/")) || SensorName.Contains(TEXT("\\")) ||
      SensorName.Contains(TEXT(".."))) {
    UE_LOG(LogTemp, Error, TEXT("Livox: rejecting sensor name '%s'"), *SensorName);
    return false;
  }

  TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("FlightForgePlugin"));

  if (!Plugin.IsValid()) {
    UE_LOG(LogTemp, Error, TEXT("Livox: FlightForgePlugin not found, cannot locate scan patterns"));
    return false;
  }

  const FString Path = FPaths::Combine(Plugin->GetContentDir(), TEXT("Lidar"), SensorName + TEXT(".ffpat"));

  TArray<uint8> Raw;

  if (!FFileHelper::LoadFileToArray(Raw, *Path)) {
    UE_LOG(LogTemp, Error, TEXT("Livox: cannot read scan pattern '%s'"), *Path);
    return false;
  }

  if (Raw.Num() < (int64)sizeof(FPatternHeader)) {
    UE_LOG(LogTemp, Error, TEXT("Livox: '%s' is truncated"), *Path);
    return false;
  }

  FPatternHeader Header;
  FMemory::Memcpy(&Header, Raw.GetData(), sizeof(Header));

  if (Header.Magic != PatternMagic || Header.Version != PatternVersion) {
    UE_LOG(LogTemp, Error, TEXT("Livox: '%s' has bad magic/version (got %u/%u)"), *Path, Header.Magic, Header.Version);
    return false;
  }

  const int64 Expected = (int64)sizeof(FPatternHeader) + (int64)Header.Count * 3 * sizeof(float);

  if (Header.Count == 0 || Header.PointRate <= 0.0 || Raw.Num() < Expected) {
    UE_LOG(LogTemp, Error, TEXT("Livox: '%s' holds %d bytes at %.0f Hz, expected %lld for %u points"), *Path, Raw.Num(), Header.PointRate, Expected,
           Header.Count);
    return false;
  }

  Out.PointRate  = Header.PointRate;
  Out.Period     = Header.Period;
  Out.MinRangeCm = Header.MinRangeCm;
  Out.MaxRangeCm = Header.MaxRangeCm;

  Out.Directions.SetNumUninitialized(Header.Count);
  FMemory::Memcpy(Out.Directions.GetData(), Raw.GetData() + sizeof(FPatternHeader), (int64)Header.Count * 3 * sizeof(float));

  UE_LOG(LogTemp, Log, TEXT("Livox: loaded '%s' - %u points, %.0f kHz, %.2f s loop, range %.2f-%.2f m"), *SensorName, Header.Count,
         Header.PointRate / 1000.0, Header.Period, Header.MinRangeCm / 100.0, Header.MaxRangeCm / 100.0);

  return true;
}
}  // namespace

//}

/* Get() //{ */

const FLivoxPattern* FLivoxPatternLibrary::Get(const FString& SensorName) {

  if (SensorName.IsEmpty()) {
    return nullptr;
  }

  static FCriticalSection                            CacheMutex;
  static TMap<FString, TSharedPtr<const FLivoxPattern>> Cache;

  const FString Key = SensorName.ToLower();

  FScopeLock Lock(&CacheMutex);

  // A failed load caches a null entry so a missing file is not retried on every
  // frame by every drone.
  if (TSharedPtr<const FLivoxPattern>* Found = Cache.Find(Key)) {
    return Found->Get();
  }

  TSharedPtr<FLivoxPattern> Pattern = MakeShared<FLivoxPattern>();

  if (!LoadPattern(Key, *Pattern)) {
    Cache.Add(Key, nullptr);
    return nullptr;
  }

  Cache.Add(Key, Pattern);

  return Pattern.Get();
}

//}
