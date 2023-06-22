// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Nino Saglia & Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PlayerGrabComponent.generated.h"

class UCameraComponent;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrabbedObjectReleasedDelegate, const AActor*, GrabbedActor);

UCLASS(NotBlueprintable, BlueprintType, ClassGroup = "PlayerCharacter", Within = "PlayerCharacter",
	Meta = (DisplayName = "Player Grab Component", ShortToolTip = "Component for grabbing physics objects."))
class UPlayerGrabComponent : public UPhysicsHandleComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogGrabComponent, Log, All)

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates", Meta = (DisplayName = "On Physics Grab Component Released"))
	FOnGrabbedObjectReleasedDelegate OnGrabbedObjectReleased;
	
	/** Pointer to the configuration data asset instance for this component. */
	UPROPERTY()
	UPlayerGrabConfiguration* Configuration;

	/** If true, the grab component is currently in rotation mode. Any view input will be used to rotate the object. */
	UPROPERTY(BlueprintGetter = GetIsRotationModeActive)
	bool RotationMode;

	/** A multiplier used to change the rotation speed of the camera when grabbing an object.*/
	UPROPERTY()
	float CameraRotationMultiplier{1.0f};

private:
	/** Pointer to the camera component of the player. */
	UPROPERTY()
	UCameraComponent* Camera;
	
	/** If true, the grab component is currently priming an object for throwing. */
	UPROPERTY(BlueprintGetter = GetIsPrimingThrow)
	bool IsPrimingThrow{false};

	UPROPERTY(BlueprintGetter = GetWillThrowOnRelease)
	bool WillThrowOnRelease;
	
	UPROPERTY()
	bool WillReleaseOnEndInteraction;
	
	UPROPERTY()
	FVector TargetLocation;
	
	UPROPERTY()
	float CurrentZoomLevel;

	float PreviousZoomLevel;
	
	UPROPERTY()
	float CurrentZoomAxisValue;

	UPROPERTY()
	float CurrentRotationZoomAxisValue;
	
	UPROPERTY()
	FVector LastLocation;
	
	UPROPERTY()
	FQuat CameraRelativeRotation;
	
	UPROPERTY()
	FRotator RotationDifference;
	
	UPROPERTY()
	FRotator CameraRotation;

	UPROPERTY()
	FQuat CameraQuat;
	
	UPROPERTY()
	FVector RotatedHandOffset;

	UPROPERTY()
	float WillThrowOnReleaseMultiplier;

	UPROPERTY()
	float PrePrimingThrowTimer;
	
	UPROPERTY()
	float ThrowingTimeLine;

	UPROPERTY()
	FVector ReleaseLocation;

	UPROPERTY()
	FQuat MouseInputRotation{0.0,0.0,0.0,1.0};

	UPROPERTY()
	UPlayerCharacterMovementComponent* Movement;
	
	UPROPERTY()
	float GrabbedComponentSize;
	
	/** The velocity the object wil be thrown in. (Used to calculate the thrwoing trajecory) */
	UPROPERTY()
	FVector ThrowVelocity;
	
	UFUNCTION()
	void UpdateThrowTimer(float DeltaTime);

	void VisualizeProjectilePath(AActor* ProjectileActor, FVector StartLocation, FVector LaunchVelocity);
	
	
public:
	UPlayerGrabComponent();
	
	/** Grabs an actor. */
	UFUNCTION(BlueprintCallable)
	void GrabActor(AActor* ActorToGrab);

	/** Called when you release the object or place the object. */
	UFUNCTION(BlueprintCallable)
	void ReleaseObject();

	/** The third interaction which is currenty rotating the object using mouse input. */
	void BeginTetriaryInteraction();
	
	void EndTetriaryInteraction();

	/** Called to start charging a throw. */
	UFUNCTION(BlueprintCallable)
	void BeginPrimingThrow();

	/** Called to execute a throw during the priming of a throw. */
	UFUNCTION(BlueprintCallable)
	void PerformThrow(bool OnlyPreviewTrajectory);
	
	/** Gets triggered when the zoom axis value gets changed. */
	void UpdateZoomAxisValue(float ZoomAxis);

	/** Updates on tick when you are manually rotating the object. */
	void UpdateMouseImputRotation(FVector2d MouseDelta);

protected:
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

private:
	void UpdateTargetLocationWithRotation(float DeltaTime);

	void UpdateRotatedHandOffset(FRotator& Rotation, FVector& HandOffset);

	void UpdatePhysicsHandle();
	
	void StopPrimingThrow();

public:
	/** Returns the actor that is currently being grabbed. */
	UFUNCTION(BlueprintCallable, Category = "Player Physics Grab", Meta = (DisplayName = "Get Current Grabbed Actor"))
	FORCEINLINE AActor* GetGrabbedActor() const
	{
		if (const UPrimitiveComponent* Component {GetGrabbedComponent()}) { return Component->GetOwner(); }
		return nullptr; 
	}

	/** Returns whether the grab component is currently in rotation mode or not. */
	UFUNCTION(BlueprintGetter, Category = "GrabComponent", Meta = (DisplayName = "Is Rotation Mode Active"))
	FORCEINLINE bool GetIsRotationModeActive() const { return RotationMode; }

	/** Returns whether the grab component is priming throw on an object or not. */
	UFUNCTION(BlueprintGetter, Category = "GrabComponent", Meta = (DisplayName = "Is Priming Throw"))
	FORCEINLINE bool GetIsPrimingThrow() const { return IsPrimingThrow; }
	
	/** Returns whether the grab component will throw an object on release or not. */
	UFUNCTION(BlueprintGetter, Category = "GrabComponent", Meta = (DisplayName = "Is Priming Throw"))
	FORCEINLINE bool GetWillThrowOnRelease() const { return WillThrowOnRelease; }
};

/** Configuration asset to fine tune all variables within the physics grab component*/
UCLASS(BlueprintType, ClassGroup = "PlayerCharacter")
class FIRSTPERSONCHARACTER_API UPlayerGrabConfiguration : public UDataAsset
{
	GENERATED_BODY()

public:
	/** The location where the holding hand should be relative to the physics grab component.*/
	UPROPERTY(EditDefaultsOnly,Category = "Player Physics Grab")
	FVector RelativeHoldingHandLocation{70.0f,60.0f, -30.0f};

	/** The distance on which the player will let the prop go*/
	UPROPERTY(EditDefaultsOnly,Category = "Player Physics Grab")
	float LetGoDistance{150.0f};

	/** The minimum zoom level in UE units. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Grab")
	float MinZoomLevel{0.0f};

	/** The maximum zoom level in UE units.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Grab")
	float MaxZoomLevel{200.0f};

	/** The amount that the rotation speed decreases when holding objects at a distance.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Grab")
	float CameraRotationDecreasingStrength{0.3f};

	/** The distance where the object will move towards the hand location.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Grab")
	float BeginHandOffsetDistance{50.0f};

	/**The speed at which you can zoom.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Grab")
	float ZoomSpeed{1500.0f};

	/** The speed at which the object will return to your hand depending on how fast you walk.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Grab")
	float WalkingRetunZoomSpeed{1000.0f};

	// ... Throw variables ... 

	/** The time it takes to start priming the throw.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Throw")
	float PrePrimingThrowDelayTime{0.1f};

	/** The curve to set the minimum and maximum strength the player can throw.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Throw")
	UCurveFloat* ThrowingStrengthCure;
	
	/** The time it will take to fully charge a throw.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Throw")
	float ThrowChargeTime{1.3f};
	
	/** The zoom level for the throw.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Throw")
	float ThrowingZoomLevel{0.0f};

	/** The speed at which the items goes to the throwing zoom level*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Throw")
	float ToThrowingZoomSpeed{1000.0f};

	/**The distance the object will back up when you charge the throw.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Throw")
	FVector ThrowingBackupVector{-10.0,0.0,0.0};
	/**The strength of the shaking when charging a throw.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Physics Throw")
	float ThrowingShakeSize{0.07f};

	/** Linear damping of the handle spring. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Handle|Max Zoom", Meta = (DisplayName = "Linear Damping"))
	float MaxZoomLinearDamping {200.0f};

	/** Linear stiffness of the handle spring */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Handle|Max Zoom", Meta = (DisplayName = "Linear Stiffness"))
	float MaxZoomLinearStiffness {750.0f};

	/** Angular damping of the handle spring */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Handle|Max Zoom", Meta = (DisplayName = "Angular Damping"))
	float MaxZoomAngularDamping {500.0f};

	/** Angular stiffness of the handle spring */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Handle|Max Zoom", Meta = (DisplayName = "Angular Stiffness"))
	float MaxZoomAngularStiffness {1500.0f};

	/** How quickly we interpolate the physics target transform */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Handle|Max Zoom", Meta = (DisplayName = "Interpolation Speed"))
	float MaxZoomInterpolationSpeed {50.0f};

	/** Linear damping of the handle spring. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Handle|Min Zoom", Meta = (DisplayName = "Linear Damping"))
	float MinZoomLinearDamping {200.0f};

	/** Linear stiffness of the handle spring */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Handle|Min Zoom", Meta = (DisplayName = "Linear Stiffness"))
	float MinZoomLinearStiffness {750.0f};

	/** Angular damping of the handle spring */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Handle|Min Zoom", Meta = (DisplayName = "Angular Damping"))
	float MinZoomAngularDamping {500.0f};

	/** Angular stiffness of the handle spring */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Handle|Min Zoom", Meta = (DisplayName = "Angular Stiffness"))
	float MinZoomAngularStiffness {1500.0f};

	/** How quickly we interpolate the physics target transform */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Handle|Min Zoom", Meta = (DisplayName = "Interpolation Speed"))
	float MinZoomInterpolationSpeed {50.0f};
};