// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerGroundMovementType : uint8
{
	Idle				UMETA(DisplayName = "Idle"),
	Walking				UMETA(DisplayName = "Walking"),
	Sprinting			UMETA(DisplayName = "Sprinting"),
};

UENUM(BlueprintType)
enum class EPlayerLandingType : uint8
{
	Soft				UMETA(DisplayName = "Soft Landing"),
	Hard				UMETA(DisplayName = "Hard Landing"),
	Heavy				UMETA(DisplayName = "Heavy Landing"),
};

UENUM(BlueprintType)
enum class EPlayerLocomotionEvent : uint8
{
	Jump				UMETA(DisplayName = "Jump"),
	Fall				UMETA(DisplayName = "Fall"),
	CrouchStart			UMETA(DisplayName = "Start Crouching"),
	CrouchEnd			UMETA(DisplayName = "Stop Crouching"),
	SprintStart			UMETA(DisplayName = "Start Sprinting"),
	SprintEnd			UMETA(DisplayName = "Stop Sprinting")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLocomotionEventDelegate, EPlayerLocomotionEvent, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLandingDelegate, EPlayerLandingType, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpDelegate);

UCLASS(ClassGroup = "PlayerCharacter", Meta = (DisplayName = "Player Character Movement Component"))
class FIRSTPERSONCHARACTER_API UPlayerCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// DELEGATES
	/** Delegate that is called when a locomotion event occurs. */
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "Locomotion Event"))
	FLocomotionEventDelegate OnLocomotionEvent;

	/** Delegate that is called when the player character jumps. */
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "Jump Event"))
	FJumpDelegate OnJump;
	
	/** Delegate that is called when the player character lands. */
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "Landing Event"))
	FLandingDelegate OnLanding;

private:
	/** When true, the player is currently sprinting. */
	UPROPERTY(BlueprintGetter = GetIsSprinting)
	bool IsSprinting {false};

	/** When true, the player is currently in the process of jumping. */
	UPROPERTY(BlueprintGetter = GetIsJumping)
	bool IsJumping {false};

public:
	UPlayerCharacterMovementComponent();
	
	virtual bool DoJump(bool bReplayingMoves) override;
	
	/** Checks and returns the current player ground movement type.
	 *	@Return An enumeration describing the current ground movement type of the movement component.
	 */
	UFUNCTION(BlueprintPure)
	EPlayerGroundMovementType GetGroundMovementType() const;

	/** Sets whether the character movement component is sprinting or not. */
	UFUNCTION()
	void SetIsSprinting(const bool Value);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;

public:
	
	UFUNCTION(BlueprintGetter)
	FORCEINLINE bool GetIsSprinting() const {return IsSprinting; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE bool GetIsJumping() const {return IsJumping; }
};
