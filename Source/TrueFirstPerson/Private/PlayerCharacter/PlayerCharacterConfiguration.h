// Copyright 2023 Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterConfiguration.generated.h"

/** Structure that defines the player characters configuration and behavior.
 *	This class provides a convenient way for designers to tweak the player settings, and can be stored inside a data asset.*/
USTRUCT(BlueprintType)
struct FPlayerCharacterConfiguration
{
	GENERATED_USTRUCT_BODY()
	
	/** Defines the default movement speed.*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = General, Meta = (DisplayName = "Default Movement Speed", ClampMin = "0", ClampMax = "400", UiMin = "0", UIMax = "400"))
	float WalkSpeed {300.f};
	
	/** When set to enabled, the character is allowed to jump. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Jumping, Meta = (DisplayName = "Enable Jumping"))
	bool IsJumpingEnabled {true};

	/** Defines the jump velocity of the character. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Jumping, Meta = (DisplayName = "Jump Velocity", ClampMin = "0.0", UIMin = "0.0", ClampMax = "1000.0", UIMax = "1000.0"))
	float JumpVelocity {700.f};

	/** When set to enabled, the character is allowed to sprint. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sprinting, Meta = (DisplayName = "Enable Sprinting"))
	bool IsSprintingEnabled {true};

	/** Defines the movement speed when sprinting. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sprinting, Meta = (DisplayName = "Sprint Speed", EditCondition= "IsSprintingEnabled", ClampMin = "400", ClampMax = "800", UiMin = "400", UIMax = "800"))
	float SprintSpeed {600.f};

	/** When set to enabled, the character is allowed to crouch. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Crouching, Meta = (Displayname = "Enable Crouching"))
	bool IsCrouchingEnabled {true};

	/** Defines the characters movement speed while crouched. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Crouching, Meta = (Displayname = "Crouch Speed", EditCondition= "IsCrouchingEnabled", ClampMin = "0", ClampMax = "300", UiMin = "0", UIMax = "300"))
	float CrouchSpeed {200.f};

	/** When set to enabled, The crouch button will act as a toggle for the crouch action.
	 *	Enable this if you do not want the player to have to hold down the crouch button to remain crouched. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Crouching, Meta = (Displayname = "Crouch Toggle Mode", EditCondition= "IsCrouchingEnabled" , AdvancedDisplay = "true"))
	bool EnableCrouchToggle {false};
	
	/** Defines the rotation rate when using a gamepad.*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Camera, Meta = (Displayname = "Gamepad Rotation Rate"))
	float RotationRate {150.f};

	/** Defines the maximum allowed camera angle. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Camera, Meta = (Displayname = "Maximum View Pitch", ClampMin = "0", ClampMax = "90", UiMin = "0", UIMax = "90"))
	float MaximumViewPitch {80.f};

	/** Defines the minimum allowed camera angle. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Camera, Meta = (Displayname = "Minimum View Pitch", ClampMin = "-90", ClampMax = "0", UiMin = "-90", UIMax = "0"))
	float MinimumViewPitch {-75.f};
	
	/** Constructor with default values. */
	FPlayerCharacterConfiguration()
	{
		ValidateData();
	}

	/** Checks some values to make sure that the character configuration is correct. */
	void ValidateData();
	
};
