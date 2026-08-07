#pragma once


#include "CoreMinimal.h"
#include "Lidar.h"
#include "LidarLivox.generated.h"


// The wire protocol drives every lidar request through the primary ULidar slot,
// which replays a Livox pattern whenever LidarConfig.Livox is set. This
// subclass only pins that flag on, for setups that attach a dedicated Livox
// sensor directly.
UCLASS()
class UEDS_API ULidarLivox : public ULidar
{
	GENERATED_BODY()

public:
	ULidarLivox();
};
