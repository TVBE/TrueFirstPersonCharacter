// Copyright 2023 Tim Verberne.


#include "PlayerCharacterController.h"
#include "TrueFirstPerson/TrueFirstPerson.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.h"
#include "Math/Rotator.h"
#include "PlayerFlashlightComponent.h"
#include "PlayerGroundMovementType.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/PawnMovementComponent.h"
// Called on construction
APlayerCharacterController::APlayerCharacterController()
{
}

// Called when the game starts.
void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	// Get a casted reference to the PlayerCharacter
	if(GetPawn() == nullptr)
	{
		UE_LOG(LogPlayerCharacterController, Warning, TEXT("PlayerCharacterController is not assigned to a pawn."));
		return;
	}
	PlayerCharacter = Cast<APlayerCharacter>(this->GetPawn());
	if(PlayerCharacter == nullptr)
	{
		const FString PawnName {UKismetSystemLibrary::GetDisplayName(GetPawn())};
		UE_LOG(LogPlayerCharacterController, Warning, TEXT("PlayerCharacterController expected a Pawn of type PlayerCharacter, but got assigned to an instance of %s instead"), *PawnName);
	}
	
	// Restrict the CameraManager view pitch.
	if(PlayerCameraManager != nullptr)
	{
		PlayerCameraManager->ViewPitchMax = CharacterConfiguration.MaximumViewPitch;
		PlayerCameraManager->ViewPitchMin = CharacterConfiguration.MinimumViewPitch;
	}
	
}

// Called when the controller is constructed.
void APlayerCharacterController::SetupInputComponent()
{
	Super :: SetupInputComponent ();

	InputComponent->BindAxis(TEXT("Gamepad Horizontal Rotation"), this, &APlayerCharacterController::HandleHorizontalRotation);
	InputComponent->BindAxis(TEXT("Mouse Horizontal Rotation"), this,	&APlayerCharacterController::HandleHorizontalRotation);
	InputComponent->BindAxis(TEXT("Gamepad Vertical Rotation"), this,  &APlayerCharacterController::HandleVerticalRotation);
	InputComponent->BindAxis(TEXT("Mouse Vertical Rotation"), this, &APlayerCharacterController::HandleVerticalRotation);
	InputComponent->BindAxis(TEXT("Move Longitudinal"), this, &APlayerCharacterController::HandleLongitudinalMovementInput);
	InputComponent->BindAxis(TEXT("Move Lateral"),this, &APlayerCharacterController::HandleLateralMovementInput);

	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacterController::HandleJumpActionPressed);
	InputComponent->BindAction(TEXT("Sprint"),IE_Pressed, this, &APlayerCharacterController::HandleSprintActionPressed);
	InputComponent->BindAction(TEXT("Sprint"),IE_Released, this, &APlayerCharacterController::HandleSprintActionReleased);
	InputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerCharacterController::HandleCrouchActionPressed);
	InputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APlayerCharacterController::HandleCrouchActionReleased);
	InputComponent->BindAction(TEXT("ToggleFlashlight"),IE_Pressed, this, &APlayerCharacterController::HandleFlashlightActionPressed);
}

void APlayerCharacterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdatePendingActions();
}


void APlayerCharacterController::HandleHorizontalRotation(float Value)
{
	if(CanRotate())
	{
		AddYawInput(Value * CharacterConfiguration.RotationRate * GetWorld()->DeltaTimeSeconds);
	}
}

void APlayerCharacterController::HandleVerticalRotation(float Value)
{
	if(CanRotate())
	{
		AddPitchInput(Value * CharacterConfiguration.RotationRate * GetWorld()->DeltaTimeSeconds);
	}
}

void APlayerCharacterController::HandleLongitudinalMovementInput(float Value)
{
	if(CanMove())
	{
		const FRotator Rotation {FRotator(0, GetControlRotation().Yaw, 0)};
		GetCharacter()->AddMovementInput((Rotation.Vector()), Value);
	}
}

void APlayerCharacterController::HandleLateralMovementInput(float Value)
{
	if(CanMove())
	{
		const FRotator Rotation {FRotator(0, GetControlRotation().Yaw+90, 0)};
		GetCharacter()->AddMovementInput((Rotation.Vector()), Value);
	}
}

void APlayerCharacterController::HandleJumpActionPressed()
{
	if(CanJump())
	{
		float Clearance = GetClearanceAbovePawn();
		if(Clearance <= 175 && Clearance != -1.f)
		{
			// We limit the JumpZVelocity of the player under a certain clearance to prevent the character from bumping its head into the object above.
			GetCharacter()->GetCharacterMovement()->JumpZVelocity = Clearance * 4.25;
		}
		else
		{
			GetCharacter()->GetCharacterMovement()->JumpZVelocity = CharacterConfiguration.JumpVelocity;
		}
		GetCharacter()->Jump();
	}
}

void APlayerCharacterController::HandleSprintActionPressed()
{
	IsSprintPending = true;
	if(CanSprint())
	{
		StartSprinting();
	}
}


void APlayerCharacterController::HandleSprintActionReleased()
{
	IsSprintPending = false;
	if(IsSprinting())
	{
		StopSprinting();
	}
}

void APlayerCharacterController::HandleCrouchActionPressed()
{
	IsCrouchPending = true;

	if(CharacterConfiguration.EnableCrouchToggle)
	{
		if(!GetCharacter()->GetMovementComponent()->IsCrouching() && CanCrouch())
		{
			StartCrouching();
			return;
		}
		if(GetCharacter()->GetMovementComponent()->IsCrouching() && CanStandUp())
		{
			StopCrouching();
		}
	}
	else if(CanCrouch())
	{
		StartCrouching();
	}
}

void APlayerCharacterController::HandleCrouchActionReleased()
{
	IsCrouchPending = false;
}

void APlayerCharacterController::HandleFlashlightActionPressed()
{
	if(CanToggleFlashlight())
	{
		
	}
}

void APlayerCharacterController::UpdatePendingActions()
{
	if(IsSprintPending && !IsSprinting() && CanSprint())
	{
		if(!GetCharacter()->GetMovementComponent()->IsCrouching())
		{
			StartSprinting();
		}
		else if(GetCharacter()->GetMovementComponent()->IsCrouching() && CanStandUp())
		{
			StopCrouching();
			StartSprinting();
			IsCrouchPending = false;
		}
	}
	if(IsCrouchPending && !GetCharacter()->GetMovementComponent()->IsCrouching() && CanCrouch())
	{
			if(IsSprinting())
			{
				StopSprinting();
				IsSprintPending = false;
			}
			StartCrouching();
	}
}

bool APlayerCharacterController::GetHasMovementInput()
{
	if(InputComponent != nullptr)
	{
		return InputComponent->GetAxisValue("Move Longitudinal") || InputComponent->GetAxisValue("Move Lateral");
	}
	return 0.0;
}

float APlayerCharacterController::GetHorizontalRotationInput()
{
	if(InputComponent != nullptr)
	{
		return InputComponent->GetAxisValue("Horizontal Rotation");
	}
	return 0.0;
}

/** Checks the current movement state and returns a corresponding enumeration value. This function is designed for Blueprint usage to easily implement branching behavior. */
EPlayerGroundMovementType APlayerCharacterController::GetGroundMovementType()
{
	if(IsSprinting())
	{
		return EPlayerGroundMovementType::Sprinting;
	}
	if(GetCharacter()->GetMovementComponent()->IsMovingOnGround() && GetCharacter()->GetVelocity().SquaredLength() >= 25)
	{
		return EPlayerGroundMovementType::Walking;
	}
	return EPlayerGroundMovementType::Idle;
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
	constexpr float RequiredClearance {60};
	const float Clearance {GetClearanceAbovePawn()};
	return ((Clearance > RequiredClearance || Clearance == -1.f) && CharacterConfiguration.IsJumpingEnabled && !GetCharacter()->GetMovementComponent()->IsFalling());
}

bool APlayerCharacterController::CanSprint()
{
	return CharacterConfiguration.IsSprintingEnabled && GetCharacter()->GetMovementComponent()->IsMovingOnGround()
			&& GetInputAxisValue("Move Longitudinal") > 0.25 && FMath::Abs(GetInputAxisValue("Move Lateral")) < 0.5;
}

bool APlayerCharacterController::CanCrouch()
{
	return CharacterConfiguration.IsCrouchingEnabled;
}

bool APlayerCharacterController::CanInteract()
{
	return false; // Temp
}

/** Checks whether the MovementComponent of the character is equal to the Sprinting Speed defined in the Character Configuration. */
bool APlayerCharacterController::IsSprinting()
{
	return GetCharacter()->GetMovementComponent()->GetMaxSpeed() == CharacterConfiguration.SprintSpeed && GetCharacter()->GetMovementComponent()->IsMovingOnGround();
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
	return (Clearance > RequiredClearance || Clearance == -1.f && GetCharacter()->GetMovementComponent()->IsCrouching());
}

void APlayerCharacterController::StartSprinting()
{
	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = CharacterConfiguration.SprintSpeed;
}

void APlayerCharacterController::StopSprinting()
{
	GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = CharacterConfiguration.WalkSpeed;
}

void APlayerCharacterController::StartCrouching()
{
	GetCharacter()->Crouch();
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
		return HitResult.Distance - GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		// We subtract the capsule collision half height as this is the distance between the center of the SkeletalMesh and the top of the head.
	}
	
	return -1.f; // We return -1 if no hit result is produced by the collision query. This means that there is more than 500 units of clearance above the character.
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


