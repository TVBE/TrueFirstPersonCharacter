// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mouse.generated.h"

class UMeshGrabComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseMovementDelegate, FVector2D, Vector);

/** Base class for desktop mouse actor.
 *	This class is expected to be extended and fully implemented in a blueprint derived class. */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "Desktop", Meta = (DisplayName = "Mouse"))
class AMouse : public AActor
{
	GENERATED_BODY()

public:
	/** Delegate called when the mouse registers movement. */
	UPROPERTY(BlueprintAssignable, Category = "Delegates", Meta = (DisplayName = "On Mouse Movement"))
	FOnMouseMovementDelegate OnMouseMovement;
	
protected:
	/** The static mesh component for the mouse. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", Meta = (DisplayName = "Static Mesh"))
	UStaticMeshComponent* MouseMesh;

	/** The mesh interaction component for the mouse. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UMeshGrabComponent* GrabComponent;

public:	
	AMouse();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void PostInitProperties() override;
	virtual void BeginPlay() override;

	/** Handles the mesh waking up. */
	UFUNCTION()
	void HandleOnMeshWake(UPrimitiveComponent* WakingComponent, FName Name);

	/** Handles the mesh going to sleep. */
	UFUNCTION()
	void HandleOnMeshSleep(UPrimitiveComponent* SleepingComponent, FName Name);
};
