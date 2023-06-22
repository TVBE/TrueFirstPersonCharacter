// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "..\Public\MeshCollisionTriggerComponent.h"

#include "Components/ArrowComponent.h"

UMeshCollisionTriggerComponent::UMeshCollisionTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMeshCollisionTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UStaticMeshComponent* Component {Cast<UStaticMeshComponent>(GetAttachParent())})
	{
		Component->OnComponentHit.AddDynamic(this, &UMeshCollisionTriggerComponent::OnStaticMeshComponentHit);
		Component->SetGenerateOverlapEvents(false);
		Component->SetNotifyRigidBodyCollision(true);

		MeshComponent = Component;
	}
}

void UMeshCollisionTriggerComponent::OnStaticMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherComp->IsSimulatingPhysics())
	{
		return;
	}

	const FVector HitDirection {Hit.ImpactNormal.GetSafeNormal()};
	const float Angle {static_cast<float>(FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(HitDirection, -GetComponentRotation().Vector()))))};

	const FVector OtherVelocity {OtherComp->GetComponentVelocity()};
	const FVector RelativeVelocity {OtherVelocity - MeshComponent->GetComponentVelocity()};

	if (NormalImpulse.Size() >= ImpulseForceThreshold && (!RestrictCollisionAngle || Angle <= MaxAllowedAngle))
	{
		OnCollisionTrigger.Broadcast();
		if (IsTriggerLimitEnabled)
		{
			Triggers++;
			if (Triggers >= TriggerLimit)
			{
				OnCollisionTriggerLimitReached.Broadcast();
				if (DestroyAfterTriggerLimitReached)
				{
					DestroyComponent();
				}
			}
		}
	}
}

void UMeshCollisionTriggerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UStaticMeshComponent* Component {Cast<UStaticMeshComponent>(GetAttachParent())})
	{
		Component->OnComponentHit.RemoveDynamic(this, &UMeshCollisionTriggerComponent::OnStaticMeshComponentHit);
		
		MeshComponent = Component;
	}
}




