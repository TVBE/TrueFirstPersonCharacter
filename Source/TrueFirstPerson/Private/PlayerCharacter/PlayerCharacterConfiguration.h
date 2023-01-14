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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sprinting, Meta = (DisplayName = "Sprint Speed", EditCondition= "IsSprintingEnabled") )
	float SprintSpeed {600};

	/** When set to enabled, the character is allowed to crouch. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Crouching, Meta = (Displayname = "Enable Crouching"))
	bool IsCrouchingEnabled {true};

	/** Defines the characters movement speed while crouched. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Crouching, Meta = (Displayname = "Crouch Speed", EditCondition= "IsCrouchingEnabled" ))
	float CrouchSpeed {200};

	/** When set to enabled, The crouch button will act as a toggle for the crouch action.
	 *	Enable this if you do not want the player to have to hold down the crouch button to remain crouched. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Crouching, Meta = (Displayname = "Crouch Toggle Mode", EditCondition= "IsCrouchingEnabled" , AdvancedDisplay = "true"))
	bool EnableCrouchToggle {false};
	
	/** Defines the rotation rate when using a gamepad.*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Camera, Meta = (Displayname = "Gamepad Rotation Rate"))
	float RotationRate {150};
 
	/** Constructor with default values. */
	FPlayerCharacterConfiguration()
	{
	}
 
};
