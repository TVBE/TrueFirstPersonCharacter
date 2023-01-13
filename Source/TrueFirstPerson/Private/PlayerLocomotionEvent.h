// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerLocomotionEvent.generated.h"

/** Enumeration for different locomotion triggers for the player. These are used to trigger corresponding sounds and camera animations. */
UENUM(BlueprintType)
enum PlayerLocomotionEvent
{
	JUMP				UMETA(DisplayName = "Jump"),
	FALL				UMETA(DisplayName = "Fall"),
	LANDINGSOFT			UMETA(DisplayName = "Soft Landing"),
	LANDINGHARD			UMETA(DisplayName = "Hard Landing"),
	LANDINGHEAVY		UMETA(DisplayName = "Heavy Landing"),
	CROUCHSTART			UMETA(DisplayName = "Start Crouching"),
	CHROUCHEND			UMETA(DisplayName = "Stop Crouching"),
};
