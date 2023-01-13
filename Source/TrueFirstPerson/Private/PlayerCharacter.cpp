// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Math/Vector.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	UpdateMovementStatus();
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool APlayerCharacter::IsPlayerMoving()
{
	bool Moving {this->GetVelocity().IsNearlyZero(0.1)};
	return Moving;
}

// Called every frame.
void APlayerCharacter::UpdateMovementStatus()
{
	IsMoving = IsPlayerMoving();
}

void APlayerCharacter::SetIsJumping(bool Value)
{
	IsJumping = Value;
}

void APlayerCharacter::SetIsSprinting(bool Value)
{
	IsSprinting = Value;
}













