// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "AmbiverseSoundDistributionData.generated.h"

/** Defines the area around the player that an AmbienceSoundSource can play in. */
USTRUCT(BlueprintType)
struct FAmbiverseSoundDistributionData
{
	GENERATED_USTRUCT_BODY()
	
	/** Defines the horizontal range in which a sound source can play in relation to the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Play Range", Meta = (ClampMin = "0"))
	FVector2D HorizontalRange {FVector2D(500, 1000)};

	/** Defines a vertical offset. This can be used to ensure that the sound is always played above or below the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Play Range")
	float VerticalOffset {0.0f};
	
	/** Defines the vertical range in which a sound source can play in relation to the player. Is not affected by ExclusionRadius. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Play Range")
	float VerticalRange {100.0f};
	
	static FTransform GetSoundTransform(const FAmbiverseSoundDistributionData& DistributionData, const FVector& ListenerLocation);
};
