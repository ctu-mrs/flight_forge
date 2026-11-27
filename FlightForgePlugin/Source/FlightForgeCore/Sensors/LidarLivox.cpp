#include "LidarLivox.h"

ULidarLivox::ULidarLivox() : ULidar()
{
    SensorType = SensorType::LIDAR_LIVOX;
    Name = TEXT("Lidar Livox");

    LidarConfig.Livox = true;
}
