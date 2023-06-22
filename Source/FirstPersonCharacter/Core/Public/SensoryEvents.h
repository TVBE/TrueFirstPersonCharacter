#pragma once

#include "CoreMinimal.h"
#include "SensoryEvents.Generated.h"

USTRUCT(BlueprintType)
struct FAuditoryEvent
{
	GENERATED_BODY()

	/** The loudness of the event in decibels. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auditory Event", Meta = (DisplayName = "Loudness", ForceUnits = "dB", ClampMin = "30", ClampMax = "120", UIMin = "30", UIMax = "120"))
	float Loudness {0.0f};
	
	FVector Location;
};