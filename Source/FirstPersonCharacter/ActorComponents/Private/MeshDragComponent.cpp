// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.


#include "MeshDragComponent.h"

bool UMeshDragComponent::BeginDrag_Implementation(const AActor* Interactor)
{
	return IDraggableObject::BeginDrag_Implementation(Interactor);
}

bool UMeshDragComponent::EndDrag_Implementation(const AActor* Interactor)
{
	return IDraggableObject::EndDrag_Implementation(Interactor);
}
