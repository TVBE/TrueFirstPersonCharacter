// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerVfxComponent.generated.h"

class UNiagaraComponent;
class APlayerCharacter;

/** UPlayerAudioController is an Actor Component responsible for managing all VFX specific to the player character. 
 *	This class provides a simple and convenient way for designers to customize the player's VFX implementation.
 *	@Brief ActorComponent for managing player visual effects.
 */
UCLASS(Blueprintable, ClassGroup = "PlayerCharacter", Meta = (BlueprintSpawnableComponent,
	DisplayName = "Player VFX Component", ShortToolTip = "Component that manages player VFX."))
class FIRSTPERSONCHARACTER_API UPlayerVfxComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// COMPONENTS
	/** The particle emitter for the player's left foot. */
	UPROPERTY(BlueprintGetter = GetLeftFootParticleEmitter)
	UNiagaraComponent* LeftFootEmitter;
	
	/** The particle emitter for the player's right foot. */
	UPROPERTY(BlueprintGetter = GetRightFootParticleEmitter)
	UNiagaraComponent* RightFootEmitter;
	
public:	
	// Sets default values for this component's properties
	UPlayerVfxComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void CleanupComponent();

public:
	/** Returns the left foot ParticleSystem. */
	UFUNCTION(BlueprintGetter, Category = "PlayerCharacter|Components", Meta = (DisplayName = "Left Foot Particle Emitter"))
	FORCEINLINE UNiagaraComponent* GetLeftFootParticleEmitter() const {return LeftFootEmitter; }

	/** Returns the right foot ParticleSystem. */
	UFUNCTION(BlueprintGetter, Category = "PlayerCharacter|Components", Meta = (DisplayName = "Right Foot Particle Emitter"))
	FORCEINLINE UNiagaraComponent* GetRightFootParticleEmitter() const {return RightFootEmitter; }
};
