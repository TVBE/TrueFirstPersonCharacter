// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeshCollisionTriggerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionTriggerDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionTriggerLimitReachedDelegate);

class UArrowComponent;

UCLASS(NotBlueprintable, BlueprintType, ClassGroup = "Physics", Meta = (BlueprintSpawnableComponent, Displayname = "Collision Trigger Component"))
class UMeshCollisionTriggerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	/** Delegate that is called when a collision with sufficient force and at the correct angle has occured on the mesh. */
	UPROPERTY(BlueprintAssignable)
	FOnCollisionTriggerDelegate OnCollisionTrigger;

	/** Delegate that is called when the max collision trigger limit, if enabled, is reached. */
	UPROPERTY(BlueprintAssignable)
	FOnCollisionTriggerLimitReachedDelegate OnCollisionTriggerLimitReached;

	/** The impulse force threshold for the collision event to be triggered. */
	UPROPERTY(EditAnywhere, Meta = (Displayname = "Force Threshold", Units = "Newtons"))
	float ImpulseForceThreshold {100.0f};

	/** If true, We expect the collision to come from a certain direction. */
	UPROPERTY(Meta = (InlineEditConditionToggle))
	bool RestrictCollisionAngle {false};
	
	/** The maximum allowed angle for the collision to be triggered. */
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Maximum Allowed Angle", Units = "Degrees",
		EditCondition = "RestrictCollisionAngle"))
	float MaxAllowedAngle {45.0f};

	/** If true, we enforce a max trigger count for this collision trigger component.
	 *	If the limit is reached, OnCollisionTriggerLimitReached is broadcast.
	 *	It is possible to reset the trigger limit afterwards. */
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Enable Trigger Limit"))
	bool IsTriggerLimitEnabled {false};

private:
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, Meta = (EditCondition = "IsTriggerLimitEnabled", EditConditionHides))
	int32 TriggerLimit {1};

	UPROPERTY()
	int32 Triggers {0};

	/** If true, the collision trigger component destroys itself after the trigger limit is reached. */
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Auto Destroy",
		EditCondition = "IsTriggerLimitEnabled", EditConditionHides))
	bool DestroyAfterTriggerLimitReached {true};

public:	
	UMeshCollisionTriggerComponent();
	
	/** Resets the trigger count. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ResetTriggerCount() { Triggers = 0; }

	/** Sets the force threshold required to cause a collision trigger. */
	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Set Force Threshold"))
	FORCEINLINE void SetImpulseForceThreshold(const float Value) { ImpulseForceThreshold = Value; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnStaticMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
