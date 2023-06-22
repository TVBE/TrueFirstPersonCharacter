// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "GrabbableObjectInterface.h"
#include "MeshInteractionComponent.h"
#include "MeshGrabComponent.generated.h"

UCLASS(NotBlueprintable, BlueprintType, ClassGroup = "Interaction", Meta = (BlueprintSpawnableComponent,
	DisplayName = "Mesh Grab Component", ShortToolTip = "Component that allows the player to grab and throw static mesh and components."))
class UMeshGrabComponent : public UMeshInteractionComponent, public IGrabbableObject
{
	GENERATED_BODY()

private:
	bool OverrideInventoryAutoConfig {false};

public:
	/** IGrabbableObject interface functions. */
	FORCEINLINE bool BeginGrab_Implementation(const AActor* Interactor) override { return false; }
	FORCEINLINE bool EndGrab_Implementation(const AActor* Interactor) override { return false; }
	FORCEINLINE EGrabType GetGrabType_Implementation() const override { return EGrabType::OneHanded; }

private:
	virtual void OnRegister() override;
};
