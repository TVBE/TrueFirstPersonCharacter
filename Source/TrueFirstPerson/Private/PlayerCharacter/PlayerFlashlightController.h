// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerFlashlightController.generated.h"

/** UPlayerFlashlightController is an Actor Component responsible for controlling the player's flashlight. 
 *	This class provides a simple and convenient way for designers to customize the player's flashlight behavior.
 *	@Brief ActorComponent for controlling the player's flashlight.
 */
UCLASS(Blueprintable, ClassGroup=(PlayerCharacter), meta=(BlueprintSpawnableComponent) )
class UPlayerFlashlightController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFlashlightController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
