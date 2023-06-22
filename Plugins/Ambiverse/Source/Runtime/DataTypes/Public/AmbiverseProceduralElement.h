// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "AmbiverseElement.h"
#include "AmbiverseProceduralElement.generated.h"

class UAmbiverseElement;

/** Defines the spawning behavior of a spatial AmbienceSoundSource */
USTRUCT(BlueprintType)
struct FAmbiverseProceduralElement
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	UAmbiverseElement* Element {nullptr};

	/** The volume of this element. */
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "0", ClampMax = "2", UIMin = "0", UIMax = "2"))
	float Volume {1.0f};
	
	/** The minimum and maximum delay between instances of an AmbienceSystem preset entry occuring.
	 *	The play interval is randomized between these two values. */
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Interval", ClampMin = "0"))
	FVector2D IntervalRange {FVector2D(10, 30)};
	
	UPROPERTY(Transient)
	float Time {0.0f};
	
	/** The reference time the entry was initialized with.
	 *	We use this time value to be able to dynamically apply parameters in real time without breaking th existing queue. */
	UPROPERTY(Transient)
	float ReferenceTime {0.0f};
	
	bool IsValid() const
	{
		return (Element != nullptr);
	}

	void RandomizeTime(float Scalar);
};


