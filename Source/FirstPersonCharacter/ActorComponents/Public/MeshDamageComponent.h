// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeshDamageComponent.generated.h"

class UNiagaraSystem;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamagePercentageChangedDelegate, const float, NewPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageThresholdReachedDelegate);

UCLASS(NotBlueprintable, BlueprintType, ClassGroup = "Physics", Meta = (BlueprintSpawnableComponent, Displayname = "Damage Component"))
class UMeshDamageComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	/** Delegate that is called when the damage percentage of the component is changed. */
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnDamagePercentageChangedDelegate OnDamaged;

	/** Delegate that is called when the damage threshold of the component is reached. */
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnDamageThresholdReachedDelegate OnDamageThresholdReached;

private:
	/** The damage threshold for the component to broadcast it's OnDamageThresholdReached delegate. */
	UPROPERTY(EditAnywhere, Meta = (Units = "Newtons"))
	float Threshold {100000.0f};

	/** The minimum impact force required to increment the damage. */
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Minimum Required Force", Units = "Newtons"))
	float ImpulseForceThreshold {500.0f};

	/** If enabled, we destroy the actor this component is part of when the damage threshold is reached. */
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Destroy Owner When Threshold Is Reached"))
	bool DestroyOwnerOnThresholdReached {false};
	
	UPROPERTY(EditAnywhere, Meta = (InlineEditConditionToggle))
	bool PlayParticleEffects {false};

	/** If enabled, we play a particle effect when the mesh gets destroyed. */
	UPROPERTY(EditAnywhere, Category = "VFX", Meta = (DisplayName = "Destruction Particle Effect",
		EditCondition = "PlayParticleEffects"))
	UNiagaraSystem* DestructionNiagaraSystem;

	UPROPERTY(EditAnywhere, Meta = (InlineEditConditionToggle))
	bool PlayAudioEffects {false};

	/** If enabled, we play a sound effect when the mesh gets destroyed. */
	UPROPERTY(EditAnywhere, Category = "Audio", Meta = (DisplayName = "Destruction Sound Effect",
		EditCondition = "PlayAudioEffects"))
	USoundBase* DestructionSound;
	
	float DamageLevel {0.0f};
	
	float DamagePercentage {0.0f};

	bool IsThresholdReached {false};

	bool IsCooldownActive {false};
	
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	FTimerHandle CooldownTimerHandle;
	
public:	
	UMeshDamageComponent();

	/** Resets the damage component. */
	UFUNCTION(BlueprintCallable)
	void ResetDamage();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnStaticMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void AddDamage(const float Value);
	
	void SetDamageLevel(const float Value);

	void UpdateDamagePercentage();

	void HandleDamageThresholdReached();

	void StartCooldown();
	
	UFUNCTION()
	void HandleCooldownFinished();
	
};
