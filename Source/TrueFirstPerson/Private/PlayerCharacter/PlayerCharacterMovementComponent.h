// Copyright 2023 Tim Verberne

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
enum EPlayerLocomotionEvent
{
	JUMP				UMETA(DisplayName = "Jump"),
	FALL				UMETA(DisplayName = "Fall"),
	LANDINGSOFT			UMETA(DisplayName = "Soft Landing"),
	LANDINGHARD			UMETA(DisplayName = "Hard Landing"),
	LANDINGHEAVY		UMETA(DisplayName = "Heavy Landing"),
	CROUCHSTART			UMETA(DisplayName = "Start Crouching"),
	CHROUCHEND			UMETA(DisplayName = "Stop Crouching"),
	SPRINTSTART			UMETA(DisplayName = "Start Sprinting"),
	SPRINTEND			UMETA(DisplayName = "Stop Sprinting")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLocomotionEvent, EPlayerLocomotionEvent, Value);

UCLASS()
class UPlayerCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "Locomotion Event"))
	FLocomotionEvent LocomotionEventDelegate;

private:
	/** When true, the player is currently sprinting. */
	UPROPERTY(BlueprintGetter = GetIsSprinting, Category = Default, Meta = (Displayname = "Is Sprinting"))
	bool IsSprinting {false};

	/** When true, the player is currently in the process of jumping. */
	UPROPERTY(BlueprintGetter = GetIsJumping, Category = Default, Meta = (DisplayName = "Is Jumping"))
	bool IsJumping {false};

public:
	virtual bool DoJump(bool bReplayingMoves) override;
	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;
	
	UFUNCTION(BlueprintGetter, Category = Default, Meta = (DisplayName = "Is Sprinting"))
	FORCEINLINE bool GetIsSprinting() const {return IsSprinting; }

	UFUNCTION(BlueprintGetter, Category = Default, Meta = (DisplayName = "Is Jumping"))
	FORCEINLINE bool GetIsJumping() const {return IsJumping; }

	/** Checks and returns the current player ground movement type.
	 *	@Return An enumeration describing the current ground movement type of the movement component.
	 */
	UFUNCTION(BlueprintPure, Category = Locomotion, Meta = (DisplayName = "Get Ground Movement Type"))
	EPlayerGroundMovementType GetGroundMovementType() const;

	/** Sets whether the character movement component is sprinting or not. */
	UFUNCTION(Category = Locomotion, Meta = (Displayname = "Set Is Sprinting "))
	void SetIsSprinting(const bool Value, const APlayerController* Controller);
};
