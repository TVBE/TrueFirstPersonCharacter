// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"



const bool APlayerCharacterController::CanMove()
{
	return true; // Temp
}

const bool APlayerCharacterController::CanJump()
{
	const float JumpThreshold {80};
	const float Clearance {GetClearanceAbovePawn()};
	return (Clearance > JumpThreshold || Clearance == -1.f);
}

const bool APlayerCharacterController::CanSprint()
{
	return true; // Temp
}

const bool APlayerCharacterController::CanCrouch()
{
	return true; // Temp
}

const bool APlayerCharacterController::CanInteract()
{
	return false; // Temp
}

const bool APlayerCharacterController::CanStandUp()
{
	return true; // Temp
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

const float APlayerCharacterController::GetClearanceAbovePawn()
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

const FHitResult APlayerCharacterController::GetCameraLookAtQuery()
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


