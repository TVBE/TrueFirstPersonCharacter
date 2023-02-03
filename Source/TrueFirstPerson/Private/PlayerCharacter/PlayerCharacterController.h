// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterMovementComponent.h"
#include "PlayerCharacterConfiguration.h"
#include "PlayerCharacterController.generated.h"

/** The PlayerController for the PlayerCharacter. This class is responsible for handling all user input to the player Pawn. */
UCLASS(Blueprintable, ClassGroup=(PlayerCharacter))
class APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** The configuration to use for this player character. */
	UPROPERTY(BlueprintGetter = GetPlayerCharacterConfiguration, EditAnywhere, Category = Configuration, Meta = (DisplayName = "Configuration"))
	FPlayerCharacterConfiguration CharacterConfiguration;

	/** Reference to the controlled pawn as a PlayerCharacter instance.*/
	UPROPERTY(BlueprintReadOnly, Category = Actors, Meta = (DisplayName = "Player Character"))
	class APlayerCharacter* PlayerCharacter {nullptr};

	/** If true, the player is currently pressing the sprint button. */
	UPROPERTY(BlueprintReadOnly, Category = Intention, Meta = (DisplayName = "Is Sprint Pending"))
	bool IsSprintPending {false};

	/** If true, the player is currently pressing the crouch button. */
	UPROPERTY(BlueprintReadOnly, Category = Intention, Meta = (DisplayName = "Is Crouch Pending"))
	bool IsCrouchPending {false};

public:
	APlayerCharacterController();

public:
	/** Returns whether the PlayerController has any movement input or not. */
	UFUNCTION(BlueprintPure, Category = Input, Meta = (DisplayName = "Has Any Movement Input"))
	bool GetHasMovementInput();

	/** Returns the current horizontal rotation input value from the PlayerController. */
	UFUNCTION(BlueprintPure, Category = Input, Meta = (DisplayName = "Get Horizontal Rotation Input"))
	float GetHorizontalRotationInput();
	
	/** Returns the character configuration. */
	UFUNCTION(BlueprintGetter, Category = Configuration, Meta = (DisplayName = "Get Player Character Configuration"))
	FORCEINLINE FPlayerCharacterConfiguration GetPlayerCharacterConfiguration() const {return CharacterConfiguration;}
	

protected:
	/** Checks whether the player can currently rotate. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Can Rotate"))
	bool CanRotate();
	
	/** Checks whether the player can currently move. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Can Move"))
	bool CanMove();
	
	/** Checks whether the player can currently jump. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Can Jump"))
	bool CanJump();
	
	/** Checks whether the player can currently sprint. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Can Sprint"))
	bool CanSprint();
	
	/** Checks whether the player can currently enter crouch. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Can Crouch"))
	bool CanCrouch();
	
	/** Checks whether the player can stand up and stop crouching. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Can Stand Up"))
	bool CanStandUp();
	
	/** Checks whether the player is currently looking at an interactable object. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Can Interact"))
	bool CanInteract();
	
	/** Checks whether the player can toggle the flashlight. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Can Toggle Flashlight"))
	bool CanToggleFlashlight();
	
	/** Increases the PlayerMovementComponent maximum forward speed. */
	UFUNCTION(BlueprintCallable, Category = Locomotion, Meta = (DisplayName = "Start Sprinting"))
	void StartSprinting();
	
	/** Sets the PlayerMovementComponent maximum forward speed to its default value. */
	UFUNCTION(BlueprintCallable, Category = Locomotion, Meta = (DisplayName = "Stop Sprinting"))
	void StopSprinting();
	
	/** Set the PlayerMovementComponent to crouch. */
	UFUNCTION(BlueprintCallable, Category = Locomotion, Meta = (DisplayName = "Start Crouching"))
	void StartCrouching();
	
	/** Set the PlayerMovementComponent to stop crouching. */
	UFUNCTION(BlueprintCallable, Category = Locomotion, Meta = (DisplayName = "Sop Crouching"))
	void StopCrouching();
	
	/** Performs a collision query above the Pawn and returns the clearance. This will return -1.f if the query did not produce any hit results. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Get Clearance Above Pawn"))
	float GetClearanceAbovePawn() const;
	
	/** Performs a collision query in front of the camera and returns the hit result. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Get Camera Look At Query"))
	FHitResult GetCameraLookAtQuery() const;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	// User Input Callback Functions
	/** Adjusts the character's horizontal orientation using a gamepad or mouse. */
	UFUNCTION()
	void HandleHorizontalRotation(float Value);

	/** Adjusts the character's vertical orientation using a gamepad or mouse. */
	UFUNCTION()
	void HandleVerticalRotation(float Value);

	/** Tries to move the character forward or backward depending on the input. */
	UFUNCTION()
	void HandleLongitudinalMovementInput(float Value);

	/** Tries to move the character forward or backward depending on the input. */
	UFUNCTION()
	void HandleLateralMovementInput(float Value);

	/** Handles the callback for when the player has pressed the Jump button */
	UFUNCTION()
	void HandleJumpActionPressed();

	/** Handles the callback for when the player has pressed the Sprint button. */
	UFUNCTION()
	void HandleSprintActionPressed();

	/** Handles the callback for when the player has released the Sprint button. */
	UFUNCTION()
	void HandleSprintActionReleased();

	/** Handles the callback for when the player has pressed the Crouch button. */
	UFUNCTION()
	void HandleCrouchActionPressed();

	/** Handles the callback for when the player has released the Crouch button. */
	UFUNCTION()
	void HandleCrouchActionReleased();

	/** Handles the callback for when the player has pressed the ToggleFlashlight button. */
	UFUNCTION()
	void HandleFlashlightActionPressed();
	
	/** Checks if any player actions are currently pending and tries to complete them. */
	UFUNCTION()
	void UpdatePendingActions();

	/** Checks if the player can continue with any actions they are currently performing. */
	UFUNCTION()
	void UpdateCurrentActions();
};
