#pragma once

#include "CoreMinimal.h"

#include <string>

/* ToFString //{ */

inline FString ToFString(const std::string& In)
{
  return FString(UTF8_TO_TCHAR(In.c_str()));
}

/* //} */
