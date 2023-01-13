// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerLocomotionState.generated.h"

/** Enumeration for different locomotion states for the player. */
UENUM(BlueprintType)
enum PlayerLocomotionState
{
	IDLE				UMETA(DisplayName = "Idle"),
	MOVING				UMETA(DisplayName = "Moving"),
	SPRINTING			UMETA(DisplayName = "Sprinting"),
	JUMPING				UMETA (DisplayName = "Jumping"), // We consider the player to be jumping if they are off the ground and have an upwards velocity.
	FALLING				UMETA(DisplayName = "Falling"),	// We consider the player to be falling if they are off the ground and have a downwards velocity.
	LANDING				UMETA(DisplayName = "Landing"),
};
