// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "MeshInteractionComponent.h"
#include "CollisionQueryParams.h"
#include "Engine/StaticMeshActor.h"

void UMeshInteractionComponent::OnRegister()
{
	Super::OnRegister();
	
	if (UStaticMeshComponent* MeshComponent {Cast<UStaticMeshComponent>(GetAttachParent())})
	{
		MeshComponent->SetMobility(EComponentMobility::Movable);
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->PutRigidBodyToSleep();
		MeshComponent->SetCollisionProfileName(TEXT("InteractableMesh"));
		
		MeshComponent->SetGenerateOverlapEvents(true);

		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void UMeshInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UStaticMeshComponent* MeshComponent {Cast<UStaticMeshComponent>(GetAttachParent())})
	{
		const float Mass {MeshComponent->GetMass()};

		constexpr float MinMass {12.0f};
		
		if (Mass > MinMass)
		{
			MeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
		}
		else
		{
			MeshComponent->SetCollisionObjectType(ECC_GameTraceChannel2);
			MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			
			MeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

			MeshComponent->SetNotifyRigidBodyCollision(true);
		}
		
		MeshComponent->PutRigidBodyToSleep();
	}
}
