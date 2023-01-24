// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCameraConfiguration.h"
#include "Components/ActorComponent.h"
#include "UObject/WeakObjectPtr.h"
#include "PlayerCameraController.generated.h"


class APlayerCharacterController;
UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	FPlayerCameraConfiguration CameraConfiguration {FPlayerCameraConfiguration()};

	/** The default head socket location from the skeletal mesh of the PlayerCharacterPawn. */
	UPROPERTY()
	FVector HeadSocketLocation {FVector()};
	

	
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
	
	/** Updates the camera's field of view according to the Players movement. */
	UFUNCTION()
	void UpdateCameraFieldOfView();
};
