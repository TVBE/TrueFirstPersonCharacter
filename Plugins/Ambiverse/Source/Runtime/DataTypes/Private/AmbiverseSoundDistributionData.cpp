// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseSoundDistributionData.h"

FTransform FAmbiverseSoundDistributionData::GetSoundTransform(const FAmbiverseSoundDistributionData& DistributionData,
	const FVector& ListenerLocation)
{
	FTransform Transform;

	const double X {FMath::RandRange(-DistributionData.HorizontalRange.X, DistributionData.HorizontalRange.X)};
	const double Y {FMath::RandRange(-DistributionData.HorizontalRange.Y, DistributionData.HorizontalRange.Y)};
	
	double Z {FMath::RandRange(DistributionData.VerticalRange * -0.5, DistributionData.VerticalRange * 0.5)};
	Z += DistributionData.VerticalOffset;

	const FVector Location {FVector(X, Y, Z) + ListenerLocation};

	Transform.SetLocation(Location);
	
	return Transform;
}
