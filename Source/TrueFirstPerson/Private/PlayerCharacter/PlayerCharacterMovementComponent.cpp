// Copyright 2023 Tim Verberne


#include "PlayerCharacterMovementComponent.h"

bool UPlayerCharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	LocomotionEventDelegate.Broadcast(JUMP);
	return Super::DoJump(bReplayingMoves);
}

void UPlayerCharacterMovementComponent::ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations)
{
	if(Velocity.Z < -1300)
	{
		if(Velocity.Z < -1600)
		{
			LocomotionEventDelegate.Broadcast(LANDINGHEAVY);
		}
		else
		{
			LocomotionEventDelegate.Broadcast(LANDINGHARD);
		}
	}
	else
	{
		LocomotionEventDelegate.Broadcast(LANDINGSOFT);
	}
	Super::ProcessLanded(Hit, remainingTime, Iterations);
}

/** Checks the current movement state and returns a corresponding enumeration value. */
EPlayerGroundMovementType UPlayerCharacterMovementComponent::GetGroundMovementType() const
{
	if(IsSprinting)
	{
		return EPlayerGroundMovementType::Sprinting;
	}
	if(IsMovingOnGround() && Velocity.SquaredLength() >= 25)
	{
		return EPlayerGroundMovementType::Walking;
	}
	return EPlayerGroundMovementType::Idle;
}

// Called by the player controller.
void UPlayerCharacterMovementComponent::SetIsSprinting(const bool Value, const APlayerController* Controller)
{
	if(!PawnOwner || IsSprinting == Value)
	{
		return;
	}
	if(PawnOwner->GetController() == Controller)
	{
		IsSprinting = Value;
	}
	if(Value)
	{
		LocomotionEventDelegate.Broadcast(SPRINTSTART);
	}
	else
	{
		LocomotionEventDelegate.Broadcast(SPRINTEND);
	}
	
	
}
