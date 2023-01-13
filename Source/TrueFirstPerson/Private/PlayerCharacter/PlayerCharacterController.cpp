// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "PlayerFlashlightComponent.h"
#include "GameFramework/PawnMovementComponent.h"


void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<APlayerCharacter>(this->GetPawn());
}

bool APlayerCharacterController::CanRotate()
{
	return true;
}

bool APlayerCharacterController::CanMove()
{
	return true; // Temp
}

bool APlayerCharacterController::CanJump()
{
	constexpr float RequiredClearance {80};
	const float Clearance {GetClearanceAbovePawn()};
	return ((Clearance > RequiredClearance || Clearance == -1.f) && CharacterConfiguration.IsJumpingEnabled);
}

bool APlayerCharacterController::CanSprint()
{
	return (CharacterConfiguration.IsSprintingEnabled);
}

bool APlayerCharacterController::CanCrouch()
{
	return true; // Temp
}

bool APlayerCharacterController::CanInteract()
{
	return false; // Temp
}

bool APlayerCharacterController::CanToggleFlashlight()
{	const UActorComponent* Component {PlayerCharacter->GetComponentByClass(UPlayerFlashlightComponent::StaticClass())};
	if(Component != nullptr)
	{
		return true;
	};
	return false;
}


bool APlayerCharacterController::CanStandUp()
{
	constexpr float RequiredClearance {100};
	const float Clearance {GetClearanceAbovePawn()};
	return (Clearance > RequiredClearance || Clearance == -1.f);
}

void APlayerCharacterController::StartSprinting()
{
	// Temp
}

void APlayerCharacterController::StopSprinting()
{
	// Temp
}

void APlayerCharacterController::StartCrouching()
{
	// Temp
}

void APlayerCharacterController::StopCrouching()
{
	// Temp
}

float APlayerCharacterController::GetClearanceAbovePawn()
{
	const AActor* Actor {this->GetPawn()};
	const FVector Start {Actor->GetActorLocation()};
	const FVector End {Start + FVector(0.f, 0.f, 500.f)};
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	if (Actor->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		return HitResult.Distance;
	}
	
	return -1.f;
}

FHitResult APlayerCharacterController::GetCameraLookAtQuery()
{
	const float TraceLength {250.f};
	const FVector Start {this->PlayerCameraManager->GetCameraLocation()};
	const FVector End {Start + this->PlayerCameraManager->GetActorForwardVector() * TraceLength};
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	if (this->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		return HitResult;
	}
	return FHitResult();
}


