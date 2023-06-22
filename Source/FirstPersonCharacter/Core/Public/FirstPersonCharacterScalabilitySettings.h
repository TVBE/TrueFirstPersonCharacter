// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "FirstPersonCharacterScalabilitySettings.generated.h"

/** Struct defining the graphical settings for the game. */
USTRUCT(BlueprintType)
struct FFirstPersonCharacterScalabilitySettings
{
	GENERATED_USTRUCT_BODY()

	/***/
 
	/** Constructor with default values. */
	FFirstPersonCharacterScalabilitySettings()
	{
	}
 
};

/** Enum for defining which foot performed a footstep. */
UENUM(BlueprintType)
enum class EAntiAliasingSetting : uint8
{
	TSR			UMETA(DisplayName = "Temporal Super Resolution"),
	TAA			UMETA(DisplayName = "Temporal Anti-Aliasing"),
	FXAA		UMETA(DisplayName = "Fast Approximate Anti-Aliasing"),
};
