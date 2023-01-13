// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/** The PlayerController for the PlayerCharacter. This class is responsible for handling all user input to the player Pawn.
 */
UCLASS(Blueprintable)
class APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	/** Checks whether the player can currently move. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Default, Meta = (DisplayName = "Can Move"))
	const bool CanMove();
	
	/** Checks whether the player can currently jump. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Default, Meta = (DisplayName = "Can Jump"))
	const bool CanJump();
	
	/** Checks whether the player can currently sprint. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Default, Meta = (DisplayName = "Can Sprint"))
	const bool CanSprint();
	
	/** Checks whether the player can currently enter crouch. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Default, Meta = (DisplayName = "Can Crouch"))
	const bool CanCrouch();
	
	/** Checks whether the player can stand up and stop crouching. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Default, Meta = (DisplayName = "Can Stand Up"))
	const bool CanStandUp();
	
	/** Checks whether the player is currently looking at an interactable object. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Default, Meta = (DisplayName = "Can Interact"))
	const bool CanInteract();
	
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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Default, Meta = (DisplayName = "Get Clearance Above Pawn"))
	const float GetClearanceAbovePawn();
	
	/// /** Performs a collision query in front of the camera and returns the hit result. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Default, Meta = (DisplayName = "Get Camera Look At Query"))
	const FHitResult GetCameraLookAtQuery();
	
};
