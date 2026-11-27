#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#pragma push_macro("verify")
#undef check
#undef verify
#include "Generated/flightforge_generated.h"
#pragma pop_macro("verify")
#pragma pop_macro("check")
THIRD_PARTY_INCLUDES_END
