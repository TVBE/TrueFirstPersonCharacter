// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Nino Saglia & Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PlayerDragComponent.generated.h"

class UPlayerInteractionComponent;
class UCameraComponent;
class APlayerCharacter;

UCLASS(NotBlueprintable, BlueprintType, ClassGroup = "PlayerCharacter", Within = "PlayerCharacter",
	Meta = (DisplayName = "Player Drag Component", ShortToolTip = "Component for dragging physics objects."))
class FIRSTPERSONCHARACTER_API UPlayerDragComponent : public UPhysicsHandleComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogDragComponent, Log, All)
	
	/** TODO: Abide to standard C++ formatting.
	 *	1. Properties
	 *	2. Constructor
	 *	3. Methods
	 */
	
protected:
	void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY()
	UPlayerInteractionComponent* InteractionComponent;
	
	/** Returns the actor that is currently being grabbed. */
	UFUNCTION(BlueprintCallable, Category = "Player Physics Grab", Meta = (DisplayName = "Get Current Grabbed Actor"))
	FORCEINLINE AActor* GetDraggedActor() const
	{
		if (const UPrimitiveComponent* Component {GetGrabbedComponent()}) { return Component->GetOwner(); }
		return nullptr; 
	}
	
	UPROPERTY(EditDefaultsOnly, Category = "Drag Configuration")
	UPlayerDragConfiguration* Configuration;
	
	/** A multiplier used to change the rotation speed of the camera when dragging an object.*/
	UPROPERTY()
	float CameraRotationMultiplier{1.0f};

	UFUNCTION(BlueprintCallable, Category = "Drag")
	void DragActorAtLocation(AActor* ActorToDrag, const FVector& Location);

	UFUNCTION(BlueprintCallable, Category = "Drag")
	void ReleaseActor();
	
	/**Not used: Was used to change the distance of the drag location.*/
	UFUNCTION(BlueprintCallable, Category = "Zoom")
	void UpdateZoomAxisValue(float ZoomAxis);

private:
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	void UpdateTargetLocation(float DeltaTime);

	void UpdateCameraRotationSpeed(float DeltaTime);

	void UpdateLocalConstraint();
	
	UFUNCTION()
	void ApplyToPhysicsHandle();
	
	bool ApplyForceOnCenterMass{false};
	
	UPROPERTY()
	class UPlayerCharacterMovementComponent* Movement;

	float Gravity;

	FVector DraggedLocationOffset;

	float TargetLocationZ;


	float DraggedComponentSize;

	/** Locations used to set the target location of the physicshandle: handle.*/
	FVector TargetLocation;
	FVector GrabOffset{0.0,0.0,0.0};
	
	float CurrentZoomLevel;
	
	float CurrentZoomAxisValue;

public:
	/** Returns the location the drag component is dragging the mesh from. */
	FVector GetDragLocation() const;
};
/** Configuration asset to fine tune all variables within the drag component*/
UCLASS(BlueprintType, ClassGroup = "PlayerCharacter")
class FIRSTPERSONCHARACTER_API UPlayerDragConfiguration : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab Settings")
	float LetGoDistance{300.f};


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom Settings")
	float ZoomSpeed{0.0f};


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom Settings")
	float MinZoomLevel{0.0f};


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom Settings")
	float MaxZoomLevel{1000.f};

	/** The amount that the rotation speed decreases when dragging objects.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player Physics Grab")
	float CameraRotationDecreasingStrength{0.8f};
	
	
	// ... PhysicsHandleSettings ... 
	
	/** Linear damping of the handle spring. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PhysicsHandle")
	float LinearDamping{60.0f};

	/** Linear stiffness of the handle spring */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PhysicsHandle")
	float LinearStiffness{40.0f};

	/** Angular damping of the handle spring */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PhysicsHandle")
	float AngularDamping{0.0f};

	/** Angular stiffness of the handle spring */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PhysicsHandle")
	float AngularStiffness{0.0f};

	/** How quickly we interpolate the physics target transform */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PhysicsHandle")
	float InterpolationSpeed{300.0f};
};
