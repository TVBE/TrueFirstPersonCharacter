// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "MeshGrabComponent.h"

#include "Engine/StaticMeshActor.h"

inline bool DetermineInventoryAddibility(const UActorComponent* Component)
{
	if (!Component) { return false; }
	if (const AStaticMeshActor* MeshActor {Cast<AStaticMeshActor>(Component->GetOwner())})
	{
		if (const UStaticMeshComponent* MeshComponent {MeshActor->GetStaticMeshComponent()})
		{
			const float Mass {MeshComponent->GetMass()};
			if (Mass > 10.0f) { return false; }

			const FBoxSphereBounds BoxSphereBounds {MeshComponent->GetStaticMesh()->GetBounds()};
			const FVector BoxExtent {BoxSphereBounds.BoxExtent};
			const float Volume {static_cast<float>(BoxExtent.X * BoxExtent.Y * BoxExtent.Z * 8.0f)};

			if (Volume > 1000000.0f) { return false; }
			return true;
		}
	}
	return false;
}

void UMeshGrabComponent::OnRegister()
{
	Super::OnRegister();

	//if (!OverrideInventoryAutoConfig)
	//{
	//	IsInventoryAddable = DetermineInventoryAddibility(this);
	//}
}

