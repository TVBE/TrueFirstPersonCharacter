// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "PlayerFootCollisionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

#if WITH_REACOUSTIC
#include "Reacoustic/Public/ReacousticComponent.h"
#endif

UPlayerFootCollisionComponent::UPlayerFootCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerFootCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	
	CollisionSphere = Cast<USphereComponent>(GetOwner()->AddComponentByClass(USphereComponent::StaticClass(),
		false, this->GetRelativeTransform(), true));
	if (CollisionSphere)
	{
		CollisionSphere->SetupAttachment(this);
		CollisionSphere->InitSphereRadius(SphereRadius);
		
		CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);

		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UPlayerFootCollisionComponent::OnOverlapBegin);

		CollisionSphere->RegisterComponent();
	}
}

void UPlayerFootCollisionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector ImpulseDirection = (OtherComp->GetComponentLocation() - GetComponentLocation()).GetSafeNormal();

	const FVector PlayerForwardVector {Character->GetActorForwardVector()};

	if (const float DotProduct {static_cast<float>(FVector::DotProduct(ImpulseDirection, PlayerForwardVector))}; DotProduct > 0.0f)
	{
		const FVector PlayerRightVector {Character->GetActorRightVector()};
		
		if (const float LateralDotProduct {static_cast<float>(FVector::DotProduct(ImpulseDirection, PlayerRightVector))}; LateralDotProduct < 0.0f)
			{
			ImpulseDirection -= PlayerRightVector * LateralOffsetFactor;
			}
		else 
			{
			ImpulseDirection += PlayerRightVector * LateralOffsetFactor;
			}
		ImpulseDirection.Normalize();
	}

	ImpulseDirection.Z = FMath::Max(0.0f, ImpulseDirection.Z);
	float ImpulseStrength;
	if (ScaleForceByMass)
	{
		const float ObjectMass {OtherComp->GetMass()};
		constexpr float MassExponent { 0.5f }; 
		const float ScaledMass = FMath::Pow(ObjectMass, MassExponent);
		ImpulseStrength = PushStrength * ScaledMass;
	}
	else
	{
		ImpulseStrength = PushStrength;
	}

	OtherComp->AddImpulse(ImpulseDirection * ImpulseStrength);

#if WITH_REACOUSTIC
if (ImpulseStrength > 1)
{
	if (UReacousticComponent* ReacousticComponent {Cast<UReacousticComponent>(OtherActor->GetComponentByClass(UReacousticComponent::StaticClass()))})
	{
		float PlayerVelocity {static_cast<float>(OtherActor->GetVelocity().Size())};
		
		PlayerVelocity = FMath::Clamp(PlayerVelocity, 50.0f, 300.0f);
		
		const float VelocityFactor {static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(50.0f, 300.0f), FVector2D(0.3f, 0.5f), PlayerVelocity))};
		
		const float RandomImpactValue {FMath::RandRange(VelocityFactor, 0.5f)};
		
		const float Weight {FMath::Clamp(OtherComp->GetMass(), 0.1f, 10.0f)};
		
		const float WeightFactor {static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(0.1f, 10.0f), FVector2D(0.8f, 0.2f), Weight))};
		
		const float ImpactValue {RandomImpactValue * WeightFactor};

		ReacousticComponent->TriggerManualHit(ImpactValue);
	}
}
#endif
};
