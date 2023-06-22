// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PlayerFootCollisionComponent.generated.h"

/** Component that pushes physics objects aside. We attach these components to the feet of the player character.
 *	This helps us to prevent the physicsbodies from being stepped on or glitching out when the capsule component of the player character overlaps.*/
UCLASS(NotBlueprintable, BlueprintType, ClassGroup = "PlayerCharacter", Within = "PlayerCharacter", Meta = (DisplayName = "Foot Collision Component",
		ShortToolTip = "Collision component that pushes physics actors away from the player's feet."))
class FIRSTPERSONCHARACTER_API UPlayerFootCollisionComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	/** The collision component that is added to the owner when this scene component begins play. */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* CollisionSphere;
	
	/** The radius of the sphere. */
	UPROPERTY(EditAnywhere)
	float SphereRadius {30.0f};
	
	/** The maximum push strength of the collision component. */
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Base Push Strength"))
	float PushStrength {20.0f};

	/** The lateral offset factor for objects that are directly in front of the player.
	 *	This allows the character to more easily push aside small objects when walking. */
	UPROPERTY(EditAnywhere)
	float LateralOffsetFactor {0.5f};

	/** If true, we scale the push strength to the weight of the object. */
	UPROPERTY(EditAnywhere)
	bool ScaleForceByMass {true};

	UPROPERTY()
	ACharacter* Character;

public:
	UPlayerFootCollisionComponent();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
