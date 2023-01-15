// Copyright 2023 Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "GroundMovementType.generated.h"

/** Enumeration that describes the current ground movement type of the Player Character, such as sprinting, walking, or idle. */
UENUM(BlueprintType)
enum GroundMovementType
{
	IDLE				UMETA(DisplayName = "Idle"),
	WALKING				UMETA(DisplayName = "Walking"),
	SPRINTING			UMETA(DisplayName = "Sprinting"),
};
