// Copyright 2023 Tim Verberne


#include "PlayerCharacterMovementComponent.h"

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
	if(!PawnOwner)
	{
		return;
	}
	if(PawnOwner->GetController() == Controller)
	{
		IsSprinting = Value;
	}
}
