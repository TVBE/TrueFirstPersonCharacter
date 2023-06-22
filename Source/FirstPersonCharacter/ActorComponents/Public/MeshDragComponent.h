// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "DraggableObjectInterface.h"
#include "MeshInteractionComponent.h"
#include "MeshDragComponent.generated.h"

UCLASS(NotBlueprintable, BlueprintType, ClassGroup = "Interaction", Meta = (BlueprintSpawnableComponent,
	DisplayName = "Mesh Drag Component", ShortToolTip = "Component that allows the player drag and push heavy objects."))
class UMeshDragComponent : public UMeshInteractionComponent, public IDraggableObject 
{
	GENERATED_BODY()

public:
	bool BeginDrag_Implementation(const AActor* Interactor) override;
	bool EndDrag_Implementation(const AActor* Interactor) override;
};
