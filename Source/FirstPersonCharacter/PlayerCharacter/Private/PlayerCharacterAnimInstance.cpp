// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "PlayerCharacterMovementComponent.h"

#include "KismetAnimationLibrary.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	if(GetSkelMeshComponent() && GetSkelMeshComponent()->GetOwner())
	{
		PlayerCharacter = Cast<APlayerCharacter>(GetSkelMeshComponent()->GetOwner());
	}
	Super::NativeInitializeAnimation();
}

void UPlayerCharacterAnimInstance::NativeBeginPlay()
{
	if(!PlayerCharacter && GetSkelMeshComponent() && GetSkelMeshComponent()->GetOwner())
	{
		PlayerCharacter = Cast<APlayerCharacter>(GetSkelMeshComponent()->GetOwner());
	}
	Super::NativeBeginPlay();
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (PlayerCharacter)
	{
		const APlayerCharacterController* Controller {PlayerCharacter->GetPlayerCharacterController()};
		const UPlayerCharacterMovementComponent* CharacterMovement {PlayerCharacter->GetPlayerCharacterMovement()};
		if (Controller && CharacterMovement)
		{
			CheckMovementState(*PlayerCharacter, *Controller, *CharacterMovement);
			
		}

		Direction = GetDirection(*PlayerCharacter);
		Speed = GetSpeed(*PlayerCharacter, *CharacterMovement);

		/** Reset fall timer if the player is no longer falling. */
		if (IsFalling ^ CharacterMovement->IsFalling() && !IsFalling)
		{
			FallTime = 0.0f;
		}

		/** Check whether the player is falling. */
		IsFalling = CharacterMovement->IsFalling();
		IsAirborne = CharacterMovement->IsFalling() || CharacterMovement->GetIsJumping();

		/** Update fall time if the player is falling. */
		if (IsFalling)
		{
			UpdateFallTime(DeltaSeconds);
		}
		
		CheckTurnInplaceConditions(*PlayerCharacter);
	}
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPlayerCharacterAnimInstance::GetStepData(FStepData& StepData, const FVector Location)
{
	StepData.Location = Location;

	if (GetSkelMeshComponent() && GetSkelMeshComponent()->GetOwner())
	{
		StepData.Velocity = GetSkelMeshComponent()->GetOwner()->GetVelocity().Length();
	}
		
	FHitResult HitResult;
	FVector TraceStart = Location;
	FVector TraceEnd = Location - FVector(0, 0, 50);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwningActor());
	Params.bTraceComplex = false;
	Params.bReturnPhysicalMaterial = true;

	if (GetWorld() && GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		StepData.Object = HitResult.GetActor();
		StepData.PhysicalMaterial = HitResult.PhysMaterial.Get();
	}
}

FStepData UPlayerCharacterAnimInstance::GetFootstepData(const ELeftRight Foot)
{
	FStepData StepData {FStepData()};
	const FName Socket {Foot == ELeftRight::Left ? "foot_l_socket" : "foot_r_socket"};
	if (GetSkelMeshComponent() && GetSkelMeshComponent()->GetOwner())
	{
		FVector Location {GetSkelMeshComponent()->GetSocketLocation(Socket)};
		GetStepData(StepData, Location);
	}
	return StepData;
}

FStepData UPlayerCharacterAnimInstance::GetHandstepData(const ELeftRight Hand)
{
	FStepData StepData {FStepData()};
	const FName Socket {Hand == ELeftRight::Left ? "hand_l_socket" : "hand_r_socket"};
	if (GetSkelMeshComponent() && GetSkelMeshComponent()->GetOwner())
	{
		FVector Location {GetSkelMeshComponent()->GetSocketLocation(Socket)};
		GetStepData(StepData, Location);
	}
	return StepData;
}

/** Check the movement state of the player character, and update animation variables accordingly. */
void UPlayerCharacterAnimInstance::CheckMovementState(const APlayerCharacter& Character, const APlayerCharacterController& Controller, const UPlayerCharacterMovementComponent& CharacterMovement)
{
	IsMovementPending = Controller.GetHasMovementInput();
	IsMoving = IsMovementPending && (CharacterMovement.IsMovingOnGround() || CharacterMovement.IsFalling());
	IsCrouching = CharacterMovement.IsCrouching();

	DoSprintSop = !IsMovementPending && Speed > 275 && (Direction >= -20 && Direction <= 20);
}

/** Check if the player character is turning in place, and update animation variables accordingly. */
void UPlayerCharacterAnimInstance::CheckTurnInplaceConditions(const APlayerCharacter& Character)
{
	if (Character.GetIsTurningInPlace())
	{
		/** Determine which direction the character is turning. */
		if (Character.GetYawDelta() > 0)
		{
			IsTurningRight = true;
			IsTurningLeft = false;
		}
		else
		{
			IsTurningRight = false;
			IsTurningLeft = true;
		}
		TurnSpeed = FMath::Clamp(0.1f * abs(Character.GetYawDelta()), 0.0f, 1.0f);
	}
	else
	{
		IsTurningLeft = false;
		IsTurningRight = false;
		TurnSpeed = 0.0f;
	}
}

/** Get the character's movement direction. */
float UPlayerCharacterAnimInstance::GetDirection(const APlayerCharacter& Character)
{
	const float UnmappedDirection {UKismetAnimationLibrary::CalculateDirection(Character.GetVelocity(), Character.GetActorRotation())};
	return FMath::GetMappedRangeValueClamped(FVector2D(-171.5, 171.5), FVector2D(-180, 180), UnmappedDirection);
}

/** Get the character's speed based on its movement input vector.*/
float UPlayerCharacterAnimInstance::GetSpeed(const APlayerCharacter& Character, const UPlayerCharacterMovementComponent& CharacterMovement)
{
	if (!CharacterMovement.GetLastInputVector().IsNearlyZero())
	{
		return Character.GetVelocity().Size2D();
	}
	return 0.0f;
}
/** Update the time that the character has been falling, and calculate a normalized alpha value. */
void UPlayerCharacterAnimInstance::UpdateFallTime(const float DeltaTime)
{
	FallTime += DeltaTime;
	VerticalAlpha = FMath::Clamp(FallTime, 0, 1);
}
