#pragma once

#include "CoreMinimal.h"

struct FClassAuditSummary
{
	int32  Components       = 0;
	int32  Classified       = 0;
	double AreaM2           = 0.0;
	double ClassifiedAreaM2 = 0.0;

	/** classified share of bounds area, or of components where nothing has measurable area */
	double ClassifiedShare() const;
};

/** Logs the class coverage, writes Saved/FlightForge/classes_<map>.csv and returns the totals. */
FLIGHTFORGECORE_API FClassAuditSummary AuditWorldClasses(UWorld* World);
