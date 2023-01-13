// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterConfiguration.h"
#include "PlayerCharacterController.generated.h"

/** The PlayerController for the PlayerCharacter. This class is responsible for handling all user input to the player Pawn.
 *	The actual input bindings for this controller are implemented in a Blueprint derived class of this class to allow convenient tweaks to input . */
UCLASS(Blueprintable)
class APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** The configuration to use for this player character. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Configuration, Meta = (DisplayName = "Configuration"))
	FPlayerCharacterConfiguration CharacterConfiguration;

	/** Reference to the controlled pawn as a PlayerCharacter instance.*/
	UPROPERTY(BlueprintReadOnly, Category = Actors, Meta = (DisplayName = "Player Character"))
	class APlayerCharacter* PlayerCharacter;

private:
	virtual void BeginPlay() override;

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
	float GetClearanceAbovePawn();
	
	/** Performs a collision query in front of the camera and returns the hit result. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Get Camera Look At Query"))
	FHitResult GetCameraLookAtQuery();
	
};
