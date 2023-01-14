// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LandingIntensity.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** If true, the character is currently sprinting. */
	UPROPERTY(BlueprintReadOnly, Category = Locomotion, Meta = (DisplayName = "Is Sprinting"))
	bool IsSprinting;
	
	/** If true, the character is currently jumping. We assume the character is jumping if the character has not left the ground yet, but the jump action is triggered. */
	UPROPERTY(BlueprintReadOnly, Category = Locomotion, Meta = (DisplayName = "Is Jumping"))
	bool IsJumping;


private:
	/** Checks whether the player has landed after falling. */
	bool IsPlayerLanding();

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Specify whether the character is currently jumping. */
	UFUNCTION(BlueprintCallable, Category = Locomotion, Meta = (DisplayName = "Set IsJumping", CompactNodeTitle = "Is Jumping"))
	void SetIsJumping(bool Value);

	/** Specify whether the character is currently sprinting. */
	UFUNCTION(BlueprintCallable, Category = Locomotion, Meta = (DisplayName = "Set IsSprinting", CompactNodeTitle = "Is Sprinting"))
	void SetIsSprinting(bool Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

private: 
	
};
