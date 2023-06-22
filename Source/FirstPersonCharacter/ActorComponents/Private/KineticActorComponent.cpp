// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "KineticActorComponent.h"

#include "MeshInteractionComponent.h"
#include "PhysicsEngine/BodyInstance.h"

UKineticActorComponent::UKineticActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UKineticActorComponent::OnRegister()
{
	Super::OnRegister();
	
	IsGrabbed = true;

	if (const AActor* Actor {GetOwner()})
	{
		if (const UMeshInteractionComponent* MeshInteractionComponent {Actor->FindComponentByClass<UMeshInteractionComponent>()})
		{
			Mesh = Cast<UStaticMeshComponent>(MeshInteractionComponent->GetAttachParent());
			if (Mesh)
			if (Mesh->IsSimulatingPhysics())
			{
					Mesh->BodyInstance.bUseCCD = true;
				
					Mesh->SetNotifyRigidBodyCollision(false);
				
					if (!Mesh->BodyInstance.bGenerateWakeEvents)
					{
						Mesh->BodyInstance.bGenerateWakeEvents = true;
						DisableGenerateWakeEventsOnSleep = true;
					}
				
					OriginalSleepFamily = Mesh->BodyInstance.SleepFamily;
					OriginalSleepThreshold = Mesh->BodyInstance.CustomSleepThresholdMultiplier;
				
					Mesh->BodyInstance.SleepFamily = ESleepFamily::Custom;
					Mesh->BodyInstance.CustomSleepThresholdMultiplier = 1000.0f;
			}
		}
	}
}

void UKineticActorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Mesh)
	{
		Mesh->OnComponentSleep.AddDynamic(this, &UKineticActorComponent::HandleActorSleep);
	}
	
	if (const UWorld* World {GetWorld()})
	{
		World->GetTimerManager().SetTimer(CollisionHitEventEnableTimerHandle, this,
			&UKineticActorComponent::EnableNotifyRigidBodyCollisionOnOwner, CollisionHitEventEnableDelay, false);
	}

	if (Mesh && !Mesh->IsAnyRigidBodyAwake())
	{
		Mesh->WakeAllRigidBodies();
	}
}

void UKineticActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/** Temporary implementation to prevent the object from sleeping on release. */
	// if (Mesh && !Mesh->IsAnyRigidBodyAwake())
	// {
	// 	Mesh->WakeAllRigidBodies();
	// }
}

void UKineticActorComponent::HandleOnOwnerGrabbed()
{
	if (!Mesh) { return; }
	
	IsGrabbed = true;

	if (Mesh->IsSimulatingPhysics())
	{
		Mesh->SetNotifyRigidBodyCollision(false);
		
		if (const UWorld* World {GetWorld()})
		{
			if (World->GetTimerManager().IsTimerActive(CollisionHitEventEnableTimerHandle))
			{
				World->GetTimerManager().ClearTimer(CollisionHitEventEnableTimerHandle);
			}

			World->GetTimerManager().SetTimer(CollisionHitEventEnableTimerHandle, this,
				&UKineticActorComponent::EnableNotifyRigidBodyCollisionOnOwner, CollisionHitEventEnableDelay, false);
		}
	}
}

void UKineticActorComponent::HandleOnOwnerReleased()
{
	if (Mesh && !Mesh->IsAnyRigidBodyAwake())
	{
		if (!Mesh->IsAnyRigidBodyAwake())
		{
			Mesh->WakeAllRigidBodies();
		}

		if (const UWorld* World {GetWorld()})
		{
			if (World->GetTimerManager().IsTimerActive(RigidBodySleepEnableTimerHandle))
			{
				World->GetTimerManager().ClearTimer(RigidBodySleepEnableTimerHandle);
			}

			World->GetTimerManager().SetTimer(RigidBodySleepEnableTimerHandle, this,
				&UKineticActorComponent::EnableRigidBodySleep, TimeToStayAwakeAfterRelease, false);
		}
		
	}
	
	IsGrabbed = false;
}

void UKineticActorComponent::EnableNotifyRigidBodyCollisionOnOwner()
{
	if (!Mesh) { return; }
	
	if (Mesh->IsSimulatingPhysics())
	{
		Mesh->SetNotifyRigidBodyCollision(true);
	}
}

void UKineticActorComponent::EnableRigidBodySleep()
{
	if (Mesh)
	{
		Mesh->BodyInstance.SleepFamily = OriginalSleepFamily;
		Mesh->BodyInstance.CustomSleepThresholdMultiplier = OriginalSleepThreshold;
	}
}

void UKineticActorComponent::HandleActorSleep(UPrimitiveComponent* Component, FName BoneName)
{
	if (!Mesh || IsGrabbed) { return; }
	Mesh->OnComponentSleep.RemoveDynamic(this, &UKineticActorComponent::HandleActorSleep);

	if (DisableGenerateWakeEventsOnSleep)
	{
		Mesh->BodyInstance.bGenerateWakeEvents = false;
	}
	
	Mesh->BodyInstance.bUseCCD = false;
	
	DestroyComponent();
}




