// Copyright 2023 Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterConfiguration.generated.h"

/** Structure that defines the player characters configuration and behavior. */
USTRUCT(BlueprintType)
struct FPlayerCharacterConfiguration
{
	GENERATED_USTRUCT_BODY()
	
	/** Defines the default movement speed.*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = General, Meta = (DisplayName = "Default Movement Speed"))
	float MovementSpeed {300};
	
	/** When set to enabled, the character is allowed to jump. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Jumping, Meta = (DisplayName = "Enable Jumping"))
	bool IsJumpingEnabled {true};

	/** When set to enabled, the character is allowed to sprint. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sprinting, Meta = (DisplayName = "Enable Sprinting"))
	bool IsSprintingEnabled {true};

	/** Defines the movement speed when sprinting. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sprinting, Meta = (DisplayName = "Sprint Speed", EditCondition="IsSprintingEnabled"))
	float SprintSpeed {600};

	/** When set to enabled, the character is allowed to crouch. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Crouching, Meta = (Displayname = "Enable Crouching"))
	bool IsCrouchingEnabled {true};

	/** Defines the characters movement speed while crouched. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Crouching, Meta = (Displayname = "Crouch Speed", EditCondition="IsCrouchingEnabled"))
	float CrouchSpeed {200};
	
 
	/** Constructor with default values. */
	FPlayerCharacterConfiguration()
	{
	}
 
};
