// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AmbiverseDistributor.generated.h"


class UAmbiverseElement;
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "Ambiverse", Meta = (DisplayName = "Ambiverse Distributor",
	ShortToolTip = "A distributor allows for blueprint scripting of custom spawn behavior."))
class AMBIVERSE_API UAmbiverseDistributor : public UObject
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseDistributor, Log, All)

private:
	UPROPERTY(Transient)
	AActor* Listener;

public:
	void Activate(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintImplementableEvent, Meta = (WorldContext = "WorldContextObject"))
	bool ExecuteDistribution(UObject* WorldContextObject, FTransform& Transform, FVector Location, UAmbiverseElement* Element);

	/** Gets a random point in a specified radius around the listener. */
	UFUNCTION(BlueprintCallable)
	FVector GetRandomPointInRadiusAroundListener(float Radius);

	/** Calculates a point at a specific distance and angle from the listener. */
	UFUNCTION(BlueprintCallable)
	FVector GetPointAtDistanceAndAngleFromListener(float Distance, float Angle);
	
	/** Performs a line trace downwards from the listener's location, and sets the passed FVector& to the hit location.
	 * @param Location A reference to an FVector which will be set to the hit location.
	 * @param Offset A float value that represents the distance above the floor. */
	UFUNCTION(BlueprintCallable, Category = "Ambiverse", Meta = (AutoCreateRefTerm = "Location"))
	void SnapToFloor(UPARAM(ref) FVector& Location, float Offset);
	
	/** Performs a line trace from the listener's location to a given FVector, and sets the FVector to the hit location.
	 * @param Location A reference to an FVector which will be set to the hit location.
	 * @param Offset A float value that represents a offset from the hit location. */
	UFUNCTION(BlueprintCallable, Category = "Ambiverse", Meta = (AutoCreateRefTerm = "Location"))
	void SetLocationByTrace(UPARAM(ref) FVector& Location, float Offset);

protected:
	UFUNCTION(BlueprintPure, Meta = (CompactNodeTitle = "Listener"))
	FORCEINLINE AActor* GetListener() { return Listener; }

private:
	UWorld* GetWorldFromListener();
};
