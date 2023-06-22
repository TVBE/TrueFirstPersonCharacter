// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObjectInterface.h"
#include "MeshInteractionComponent.generated.h"

/** NOTE: This class should be treated as abstract. We haven't marked it as abtract since some old levels contain actors with a serialized instance of this component. */
UCLASS(NotBlueprintable, BlueprintType, ClassGroup = "Interaction",
	Meta = (DisplayName = "Mesh Interaction Component", ShortToolTip = "Base class for component that adds interactability to static mesh actors."))
class UMeshInteractionComponent : public USceneComponent, public IInteractableObject  
{
	GENERATED_BODY()
	
public:
	/** IInteractableObject interface functions. */
	FORCEINLINE FVector GetInteractionWidgetLocation_Implementation() const override { return GetComponentLocation(); }
	
protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
};
