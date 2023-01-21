// Copyright 2023 Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "PlayerGroundMovementType.generated.h"

/** Enumeration for scatter patterns of debris and shrapnel fragments. */
UENUM(BlueprintType)
enum class EPlayerGroundMovementType : uint8
{
	Idle				UMETA(DisplayName = "Idle"),
	Walking				UMETA(DisplayName = "Walking"),
	Sprinting			UMETA(DisplayName = "Sprinting"),
};
