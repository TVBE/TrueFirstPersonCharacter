// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseDistributor.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseDistributor, LogAmbiverseDistributor);

void UAmbiverseDistributor::Activate(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (APlayerController* PlayerController {WorldContextObject->GetWorld()->GetFirstPlayerController()})
		{
			if (APlayerCameraManager* CameraManager {PlayerController->PlayerCameraManager})
			{
				Listener = CameraManager->GetViewTarget();
			}
		}
	}
}

FVector UAmbiverseDistributor::GetRandomPointInRadiusAroundListener(float Radius)
{
	if (!Listener)
	{
		UE_LOG(LogAmbiverseDistributor, Warning, TEXT("GetRandomPointInRadiusAroundListener: Listener is null."));
		return FVector{0.0f, 0.0f, 0.0f};
	}

	const float RandomAngle {FMath::RandRange(0.0f, 2 * PI)};
	const float RandomRadius {FMath::Sqrt(FMath::RandRange(0.0f, FMath::Square(Radius)))};

	const double X {Listener->GetActorLocation().X + RandomRadius * FMath::Cos(RandomAngle)};
	const double Y {Listener->GetActorLocation().Y + RandomRadius * FMath::Sin(RandomAngle)};
	const double Z {Listener->GetActorLocation().Z};

	return FVector{X, Y, Z};
}

FVector UAmbiverseDistributor::GetPointAtDistanceAndAngleFromListener(float Distance, float Angle)
{
	if (!Listener)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPointAtDistanceAndAngleFromListener: Listener is null."));
		return FVector{0.0f, 0.0f, 0.0f};
	}

	const double X {Listener->GetActorLocation().X + Distance * FMath::Cos(Angle)};
	const double Y {Listener->GetActorLocation().Y + Distance * FMath::Sin(Angle)};
	const double Z {Listener->GetActorLocation().Z};

	return FVector{X, Y, Z};
}

void UAmbiverseDistributor::SnapToFloor(FVector& Location, float Offset)
{
	if (UWorld* World {GetWorldFromListener()})
	{
		const FVector StartLocation {Listener->GetActorLocation()};
		const FVector EndLocation {StartLocation + FVector{0.0f, 0.0f, -FLT_MAX}};

		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("SnapToFloorTrace")), true, Listener);
		TraceParams.bTraceComplex = false;

		const bool Hit {World->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility,
			TraceParams
		)};

		if (Hit)
		{
			Location = HitResult.Location + FVector(0, 0, Offset);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SnapToFloor: Failed to snap to floor."));
		}
	}
}

void UAmbiverseDistributor::SetLocationByTrace(FVector& Location, float Offset)
{
	UWorld* World = GetWorldFromListener();
	if (!World)
	{
		return;
	}

	const FVector StartLocation {Listener->GetActorLocation()};
	const FVector EndLocation {Location};
	const FVector TraceDirection {(EndLocation - StartLocation).GetSafeNormal()};

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("SetLocationByTraceTrace")), true, Listener);
	TraceParams.bTraceComplex = true;

	const bool Hit {World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		TraceParams
	)};

	if (Hit)
	{
		Location = HitResult.Location + TraceDirection * Offset;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetLocationByTrace: Failed to find hit."));
	}
}

UWorld* UAmbiverseDistributor::GetWorldFromListener()
{
	if (!Listener)
	{
		UE_LOG(LogAmbiverseDistributor, Warning, TEXT("GetWorldFromListener: Listener is null."));
		return nullptr;
	}

	return {Listener->GetWorld()};
}
