// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LandingIntensity.h"
#include "PlayerVFXController.h"
#include "PlayerCharacter.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:

protected:
	/** If true, the character is currently jumping. We assume the character is jumping if the character has not left the ground yet, but the jump action is triggered. */
	UPROPERTY(BlueprintReadOnly, Category = Locomotion, Meta = (DisplayName = "Is Jumping"))
	bool IsJumping;

private:
	/** The camera for the player. */
	UPROPERTY(BlueprintGetter = GetCamera, VisibleAnywhere, Category = Camera, Meta = (DisplayName = "Camera"))
	class UCameraComponent* Camera {nullptr};

	/** The flashlight for the player. */
	UPROPERTY(BlueprintGetter = GetFlashlight, VisibleAnywhere, Category = Flashlight, Meta = (DisplayName = "Flashlight", AllowPrivateAccess = "true"))
	class USpotLightComponent* Flashlight {nullptr};

	/** The SpringArmComponent the flashlight is attached to. */
	UPROPERTY(BlueprintGetter = GetFlashlightSpringArm)
	class USpringArmComponent* FlashlightSpringArm {nullptr};

	/** The PlayerAudioController that handles player audio. */
	UPROPERTY(BlueprintGetter = GetAudioController, VisibleAnywhere, Category = Components, Meta = (DisplayName = "Player Audio Controller"))
	class UPlayerAudioController* AudioController {nullptr};

	/** The PlayerVFXController that handles player VFX. */
	UPROPERTY(BlueprintGetter = GetVFXController, VisibleAnywhere, Category = Components, Meta = (DisplayName = "Player VFX Controller"))
	class UPlayerVFXController* VFXController {nullptr};

	/** The CameraController that handles first person camera behavior. */
	UPROPERTY(BlueprintGetter = GetCameraController, VisibleAnywhere, Category = Components, Meta = (DisplayName = "Player Camera Controller"))
	class UPlayerCameraController* CameraController {nullptr};

	/** The FlashlightController that handles the flashlight behavior. */
	UPROPERTY(BlueprintGetter = GetFlashlightController, VisibleAnywhere, Category = Components, Meta = (DisplayName = "Player Flashlight Controller"))
	class UPlayerFlashlightController* FlashlightController {nullptr};

	/** The PlayerCharacterMovementComponent that handles the PlayerCharacter's movement. */
	UPROPERTY(BlueprintGetter = GetPlayerCharacterMovement, VisibleAnywhere, Category = Components, Meta = (DisplayName = "Player Character Movement Component"))
	class UPlayerCharacterMovementComponent* PlayerCharacterMovement {nullptr};
	
	/** If true, the character is currently sprinting. This value can only be set by a PlayerController. */
	UPROPERTY(BlueprintGetter = GetIsSprinting)
	bool IsSprinting {false};


public:
	// Sets default values for this character's properties
	APlayerCharacter();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitProperties() override;


public:	
	/** Returns the camera component. */
	UFUNCTION(BlueprintGetter, Category = Components, Meta = (DisplayName = "Get Camera"))
	FORCEINLINE UCameraComponent* GetCamera() const {return Camera; }

	/** Returns the flashlight component. */
	UFUNCTION(BlueprintGetter, Category = Components, Meta = (DisplayName = "Get Flashlight"))
	FORCEINLINE USpotLightComponent* GetFlashlight() const {return Flashlight; }

	/** Returns the flashlight SpringArmComponent. */
	UFUNCTION(BlueprintGetter, Category = Components, Meta = (DisplayName = "Get Flashlight Spring Arm"))
	FORCEINLINE USpringArmComponent* GetFlashlightSpringArm() const {return FlashlightSpringArm; }

	/** Returns the Player Camera Controller. */
	UFUNCTION(BlueprintGetter, Category = Components, Meta = (DisplayName = "Get Camera Controller"))
	FORCEINLINE UPlayerCameraController* GetCameraController() const {return CameraController; }

	/** Returns the Player Flashlight Controller. */
	UFUNCTION(BlueprintGetter, Category = Components, Meta = (DisplayName = "Get Flashlight Controller"))
	FORCEINLINE UPlayerFlashlightController* GetFlashlightController() const {return FlashlightController; }

	/** Returns the PlayerCharacterMovementComponent. */
	UFUNCTION(BlueprintGetter, Category = Components, Meta = (DisplayName = "Get Player Character Movement Component"))
	FORCEINLINE UPlayerCharacterMovementComponent* GetPlayerCharacterMovement() const {return PlayerCharacterMovement; }

	/** Returns the Player Audio Controller. */
	UFUNCTION(BlueprintGetter, Category = Components, Meta = (DisplayName = "Get Player Audio Controller"))
	FORCEINLINE UPlayerAudioController* GetAudioController() const {return AudioController; }

	/** Returns the Player VFX Controller*/
	UFUNCTION(BlueprintGetter, Category = Components, Meta = (DisplayName = "Get Player VFX Controller"))
	FORCEINLINE UPlayerVFXController* GetVFXController() const {return VFXController; }

	/** Returns whether the player is currently sprinting. */
	UFUNCTION(BlueprintGetter = GetIsSprinting, Category = Locomotion, Meta = (DisplayName = "Is Sprinting"))
	FORCEINLINE bool GetIsSprinting() const {return IsSprinting; }

	/** Sets whether the player is currently sprinting. */
	UFUNCTION(BlueprintCallable, Category = Locomotion, Meta = (DisplayName = "Set IsSprinting"))
	void SetIsSprinting(bool Value, APlayerController* PlayerController);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Specify whether the character is currently jumping. */
	UFUNCTION(BlueprintCallable, Category = Locomotion, Meta = (DisplayName = "Set IsJumping", CompactNodeTitle = "Is Jumping"))
	void SetIsJumping(bool Value);

private:
	/** Checks whether the player has landed after falling. */
	bool IsPlayerLanding();

#if WITH_EDITOR
	/** Checks whether an object is a blueprint derived class or not. */
	static bool IsBlueprintClass(const UObject* Object)
	{return static_cast<bool>(Object->GetClass()->ClassFlags & CLASS_CompiledFromBlueprint);}
#endif
	
};


