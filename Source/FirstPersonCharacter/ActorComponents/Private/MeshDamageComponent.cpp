// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "MeshDamageComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"

UMeshDamageComponent::UMeshDamageComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}

void UMeshDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UStaticMeshComponent* Component {Cast<UStaticMeshComponent>(GetAttachParent())})
	{
		Component->OnComponentHit.AddDynamic(this, &UMeshDamageComponent::OnStaticMeshComponentHit);

		if (const FBodyInstance* BodyInstance = Component->GetBodyInstance())
		{
			if (BodyInstance->bNotifyRigidBodyCollision)
			{
				Component->SetNotifyRigidBodyCollision(true);
			}
		}
		
		MeshComponent = Component;
	}
}

void UMeshDamageComponent::ResetDamage()
{
	DamageLevel = 0.0f;
}

void UMeshDamageComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UStaticMeshComponent* Component {Cast<UStaticMeshComponent>(GetAttachParent())})
	{
		Component->OnComponentHit.RemoveDynamic(this, &UMeshDamageComponent::OnStaticMeshComponentHit);
		
		MeshComponent = Component;
	}
}

void UMeshDamageComponent::OnStaticMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<ACharacter>(OtherActor))
	{
		return;
	}
	
	if ((!OtherComp->IsSimulatingPhysics() && !MeshComponent->IsSimulatingPhysics()) || IsCooldownActive)
	{
		return;
	}
	
	const float ImpulseForce {static_cast<float>(NormalImpulse.Size())};
	
	if (ImpulseForce >= ImpulseForceThreshold)
	{
		AddDamage(ImpulseForce);
		StartCooldown();
	}
}

void UMeshDamageComponent::AddDamage(const float Value)
{
	if (DamageLevel + Value >= Threshold)
	{
		DamageLevel = Threshold;
	}
	else
	{
		DamageLevel += Value;
	}
	UpdateDamagePercentage();
}

void UMeshDamageComponent::SetDamageLevel(const float Value)
{
	DamageLevel = FMath::Clamp(Value, 0.0f, Threshold);
	UpdateDamagePercentage();
}

void UMeshDamageComponent::UpdateDamagePercentage()
{
	const float OldDamagePercentage {DamagePercentage};
	DamagePercentage = (DamageLevel / Threshold) * 100.0f;
	
	if (DamagePercentage != OldDamagePercentage)
	{
		OnDamaged.Broadcast(DamagePercentage);
	}

	if (DamagePercentage == 100)
	{
		if (!IsThresholdReached)
		{
			OnDamageThresholdReached.Broadcast();
			HandleDamageThresholdReached();
		}
		IsThresholdReached = true;
	}
}

void UMeshDamageComponent::HandleDamageThresholdReached()
{
	if (AActor* Owner {GetOwner()})
	{
		if (PlayAudioEffects)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, DestructionSound, GetComponentLocation());
		}
		
		if (PlayParticleEffects)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DestructionNiagaraSystem, GetComponentLocation(), GetComponentRotation());
		}
		
		if (DestroyOwnerOnThresholdReached)
		{
			Owner->Destroy();
		}
	}
}

void UMeshDamageComponent::StartCooldown()
{
	IsCooldownActive = true;
	
	if (const UWorld* World = GetWorld())
	{
		if (World->GetTimerManager().TimerExists(CooldownTimerHandle))
		{
			World->GetTimerManager().ClearTimer(CooldownTimerHandle);
			
		}

		constexpr float CooldownTime {0.5};
		
		World->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UMeshDamageComponent::HandleCooldownFinished, CooldownTime, false);
	}
}

void UMeshDamageComponent::HandleCooldownFinished()
{
	IsCooldownActive = false;
}



