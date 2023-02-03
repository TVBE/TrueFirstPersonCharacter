// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCameraConfiguration.h"
#include "Components/ActorComponent.h"
#include "UObject/WeakObjectPtr.h"
#include "PlayerCameraController.generated.h"

/** UPlayerCameraController is an Actor Component responsible for managing the player camera's behavior, such as camera shakes and other effects.
 *	This class provides a simple and convenient way for designers to customize the camera's behavior and add special effects to the player's view.
 *	@Brief ActorComponent for managing player camera behavior.
 */
class APlayerCharacterController;
UCLASS(Blueprintable, ClassGroup=(PlayerCharacter), meta=(BlueprintSpawnableComponent) )
class UPlayerCameraController : public UActorComponent
{
	GENERATED_BODY()

private:
	/** Reference to the PlayerCharacter. */
	UPROPERTY()
	class APlayerCharacter* PlayerCharacter;

	/** Reference to PlayerCharacterController. */
	UPROPERTY()
	class APlayerCharacterController* PlayerCharacterController;
	
	/** When set to true, the player does not have full rotational control over the camera's orientation. */
	UPROPERTY(BlueprintReadOnly, Category = Animation, Meta = (DisplayName = "Lock Camera To Animation", AllowPrivateAccess = "true"))
	bool IsCameraLockedToAnimation {false};

	/** The camera configuration. */
	UPROPERTY(BlueprintReadOnly, Category = Configuration, Meta = (DisplayName = "Camera Configuration", AllowPrivateAccess = "true"))
	FPlayerCameraConfiguration CameraConfiguration;
	
	/** The default head socket rotation from the skeletal mesh of the PlayerCharacterPawn. */
	UPROPERTY()
	FTransform HeadSocketTransform {FTransform()};

	/** Interpolated head socket rotation. */
	UPROPERTY()
	FRotator InterpolatedHeadSocketRotation {FRotator()};

	/** The roll offset value of the camera shake rotation. */
	UPROPERTY()
	double CameraShakeRoll {0.0};

	/** The roll offset value of the camera lean rotation. */
	UPROPERTY()
	double CameraLeanRoll {0.0};
	
	// Camera shakes
	
	
public:	
	// Sets default values for this component's properties
	UPlayerCameraController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/** Tries to get the owning pawn's player controller as PlayerCharacterController. */
	UFUNCTION()
	void HandleCharacterControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);

	/** Updates the camera relative location. */
	UFUNCTION()
	void UpdateCameraLocation();

	/** Updates the camera world rotation*/
	void UpdateCameraRotation();

	/** Returns a rotation offset for the camera to simulate the camera shaking while moving. */
	FRotator GetCameraSwayRotation();

	/** Returns a rotation offset for the camera when the player rotates while sprinting. Used to simulate leaning when running into bends. */
	FRotator GetCameraCentripetalRotation();

	/** Returns a scaled head socket delta rotation from the skeletal mesh of the PlayerCharacterPawn. */
	FRotator GetScaledHeadSocketDeltaRotation();
	
	/** Updates the camera's field of view according to the Players movement. */
	UFUNCTION()
	void UpdateCameraFieldOfView();
};
