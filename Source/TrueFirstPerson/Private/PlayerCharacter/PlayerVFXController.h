// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerVFXController.generated.h"

/** UPlayerAudioController is an Actor Component responsible for managing all VFX specific to the player character. 
 *	This class provides a simple and convenient way for designers to customize the player's VFX implementation.
 *	@Brief ActorComponent for managing player visual effects.
 */
UCLASS(Blueprintable, ClassGroup=(PlayerCharacter), meta=(BlueprintSpawnableComponent) )
class UPlayerVFXController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerVFXController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
