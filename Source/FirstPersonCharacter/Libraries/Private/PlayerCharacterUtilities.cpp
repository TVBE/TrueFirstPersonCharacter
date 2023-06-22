// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "PlayerCharacterUtilities.h"
#include "StepData.h"

FStepData UPlayerCharacterUtilities::GetStepData(const UObject* WorldContextObject, const AActor* Actor, const float TraceLength)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return FStepData();
	}
	
	FStepData FootstepData {FStepData()};
	if (Actor)
	{
		FVector Location {Actor->GetActorLocation()};
		FootstepData.Location = Location;
		FootstepData.Velocity = Actor->GetVelocity().Length();
		
		FHitResult HitResult;
		FVector TraceStart = Location;
		FVector TraceEnd = Location - FVector(0, 0, TraceLength);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Actor);
		Params.bTraceComplex = false;
		Params.bReturnPhysicalMaterial = true;

		if (World && World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
		{
			FootstepData.Object = HitResult.GetActor();
			FootstepData.PhysicalMaterial = HitResult.PhysMaterial.Get();
			FootstepData.Location = Location;
		}
	}
	return FootstepData;
}
