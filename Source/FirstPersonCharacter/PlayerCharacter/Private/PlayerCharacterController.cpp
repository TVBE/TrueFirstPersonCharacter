// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne..

#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "PlayerFlashlightComponent.h"
#include "PlayerCameraController.h"
#include "PlayerInteractionComponent.h"
#include "FirstPersonCharacterWorldSubystem.h"
#include "PlayerGrabComponent.h"
#include "PlayerDragComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Math/Rotator.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PawnMovementComponent.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacterController)

/** Called on construction. */
APlayerCharacterController::APlayerCharacterController()
{
}

/** Called when the game starts. */
void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	/** Get a pointer to the PlayerCharacter instance this controller is controlling. */
	if (!GetPawn())
	{
		UE_LOG(LogPlayerCharacterController, Warning, TEXT("PlayerCharacterController is not assigned to a pawn."));
		return;
	}
	PlayerCharacter = Cast<APlayerCharacter>(this->GetPawn());
	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetCharacterConfiguration())
		{
			CharacterConfiguration = PlayerCharacter->GetCharacterConfiguration();	
		}
	}
	else
	{
		const FString PawnName {UKismetSystemLibrary::GetDisplayName(GetPawn())};
		UE_LOG(LogPlayerCharacterController, Warning, TEXT("PlayerCharacterController expected a Pawn of type PlayerCharacter, but got assigned to an instance of %s instead"), *PawnName);
	}
}

/** Called when the controller possesses a pawn. */
void APlayerCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!InPawn) { return; }
	
	/** Registers the controller to the player character subsystem. */
	if (const UWorld* World {GetWorld()})
	{
		if (UFirstPersonCharacterWorldSubsystem* PlayerSubsystem {World->GetSubsystem<UFirstPersonCharacterWorldSubsystem>()})
		{
			PlayerSubsystem->RegisterPlayerController(this);
		}
	}
	
	if (const UPlayerCameraController* CameraController {Cast<UPlayerCameraController>(InPawn->FindComponentByClass(UPlayerCameraController::StaticClass()))})
	{
		if (const UPlayerCameraConfiguration* Configuration {CameraController->GetConfiguration()})
		{
			PlayerCameraManager->ViewPitchMax = Configuration->MaximumViewPitch;
			PlayerCameraManager->ViewPitchMin = Configuration->MinimumViewPitch;
		}
	}
	
	InteractionComponent = Cast<UPlayerInteractionComponent>(InPawn->FindComponentByClass(UPlayerInteractionComponent::StaticClass()));
}

/** Called when the controller is constructed. */
void APlayerCharacterController::SetupInputComponent()
{
	Super :: SetupInputComponent ();

	InputComponent->BindAxis(TEXT("Horizontal Rotation"), this, &APlayerCharacterController::HandleHorizontalRotation);
	InputComponent->BindAxis(TEXT("Vertical Rotation"), this,  &APlayerCharacterController::HandleVerticalRotation);
	InputComponent->BindAxis(TEXT("Move Longitudinal"), this, &APlayerCharacterController::HandleLongitudinalMovementInput);
	InputComponent->BindAxis(TEXT("Move Lateral"),this, &APlayerCharacterController::HandleLateralMovementInput);
	InputComponent->BindAxis(TEXT("ZoomAxis"), this, &APlayerCharacterController::HandleZoomDirectionInput);

	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacterController::HandleJumpActionPressed);
	
	InputComponent->BindAction(TEXT("Sprint"),IE_Pressed, this, &APlayerCharacterController::HandleSprintActionPressed);
	InputComponent->BindAction(TEXT("Sprint"),IE_Released, this, &APlayerCharacterController::HandleSprintActionReleased);
	
	InputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerCharacterController::HandleCrouchActionPressed);
	InputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APlayerCharacterController::HandleCrouchActionReleased);
	
	InputComponent->BindAction(TEXT("ToggleFlashlight"),IE_Pressed, this, &APlayerCharacterController::HandleFlashlightActionPressed);
	
	InputComponent->BindAction(TEXT("PrimaryAction"), IE_Pressed, this, &APlayerCharacterController::HandlePrimaryActionPressed);
	InputComponent->BindAction(TEXT("PrimaryAction"), IE_Released, this, &APlayerCharacterController::HandlePrimaryActionReleased);

	InputComponent->BindAction(TEXT("SecondaryAction"), IE_Pressed, this, &APlayerCharacterController::HandleSecondaryActionPressed);
	InputComponent->BindAction(TEXT("SecondaryAction"), IE_Released, this, &APlayerCharacterController::HandleSecondaryActionReleased);

	InputComponent->BindAction(TEXT("TertiaryAction"), IE_Pressed, this, &APlayerCharacterController::HandleTertiaryActionPressed);
	InputComponent->BindAction(TEXT("TertiaryAction"), IE_Released, this, &APlayerCharacterController::HandleTertiaryActionReleased);
}

void APlayerCharacterController::HandleHorizontalRotation(float Value)
{
	if (!CanProcessRotationInput) { return; }
	
	if (InteractionComponent && InteractionComponent->GetIsTertiaryInteractionActive())
	{
		if (InteractionComponent->GetIsTertiaryInteractionActive())
		{
			InteractionComponent->AddYawInput(Value);
			InputRotation.X = 0;
			return;
		}
	}
	InputRotation.X = Value * CharacterConfiguration->RotationRate * 0.015;
}

void APlayerCharacterController::HandleVerticalRotation(float Value)
{
	if (!CanProcessRotationInput) { return; }
	
	if (InteractionComponent && InteractionComponent->GetIsTertiaryInteractionActive())
	{
		if (InteractionComponent->GetIsTertiaryInteractionActive())
		{
			InteractionComponent->AddPitchInput(Value);
			InputRotation.Y = 0;
			return;
		}
	}
	InputRotation.Y = Value * CharacterConfiguration->RotationRate * 0.015;
}

void APlayerCharacterController::HandleLongitudinalMovementInput(float Value)
{
	if (!CanProcessMovementInput) { return; }
		const FRotator Rotation {FRotator(0, GetControlRotation().Yaw, 0)};
		GetCharacter()->AddMovementInput((Rotation.Vector()), Value);
}

void APlayerCharacterController::HandleLateralMovementInput(float Value)
{
	if (!CanProcessMovementInput) { return; }
		const FRotator Rotation {FRotator(0, GetControlRotation().Yaw+90, 0)};
		GetCharacter()->AddMovementInput((Rotation.Vector()), Value);
}
void APlayerCharacterController::HandleZoomDirectionInput(float Value)
{
	if (InteractionComponent)
	{
		InteractionComponent->AddScrollInput(Value);
	}
}

void APlayerCharacterController::HandleJumpActionPressed()
{
	if (!CanProcessMovementInput) { return; }
	if (PlayerCharacter && PlayerCharacter->CanJump())
	{
		const float Clearance = PlayerCharacter->GetClearanceAbovePawn();
		if (Clearance <= 175 && Clearance != -1.f)
		{
			// We limit the JumpZVelocity of the player under a certain clearance to prevent the character from bumping its head into the object above.
			GetCharacter()->GetCharacterMovement()->JumpZVelocity = Clearance * 4.25;
		}
		else
		{
			GetCharacter()->GetCharacterMovement()->JumpZVelocity = CharacterConfiguration->JumpVelocity;
		}
		GetCharacter()->Jump();
	}
}

void APlayerCharacterController::HandleSprintActionPressed()
{
	if (!CanProcessMovementInput) { return; }
	IsSprintPending = true;
	if (CanCharacterSprint())
	{
		PlayerCharacter->StartSprinting();
	}
}

void APlayerCharacterController::HandleSprintActionReleased()
{
	if (!CanProcessMovementInput) { return; }
	IsSprintPending = false;
	PlayerCharacter->StopSprinting();
}

void APlayerCharacterController::HandleCrouchActionPressed()
{
	if (!CanProcessMovementInput) { return; }
	
	if (CharacterConfiguration->EnableCrouchToggle)
	{
		if (!PlayerCharacter->bIsCrouched && PlayerCharacter->CanCrouch())
		{
			PlayerCharacter->Crouch(false);
			IsCrouchPending = true;
			return;
		}
		if (PlayerCharacter->bIsCrouched && PlayerCharacter->CanStandUp())
		{
			PlayerCharacter->UnCrouch(false);
			IsCrouchPending = false;
		}
	}
	else if (PlayerCharacter->CanCrouch())
	{
		PlayerCharacter->Crouch(false);
		IsCrouchPending = true;
	}
}

void APlayerCharacterController::HandleCrouchActionReleased()
{
	if (!CanProcessMovementInput || CharacterConfiguration->EnableCrouchToggle) { return; }
	IsCrouchPending = false;
	
	if (PlayerCharacter->bIsCrouched)
	{
		PlayerCharacter->UnCrouch(false);
	}
}

void APlayerCharacterController::HandleFlashlightActionPressed()
{
	if (!PlayerCharacter || !CanProcessMovementInput) { return; }
	if (UPlayerFlashlightComponent* Flashlight = PlayerCharacter->FindComponentByClass<UPlayerFlashlightComponent>())
	{
		Flashlight->SetFlashlightEnabled(!Flashlight->IsFlashlightEnabled());
	}
}

void APlayerCharacterController::HandleTertiaryActionPressed()
{
	if (InteractionComponent)
	{
		InteractionComponent->BeginTertiaryInteraction();
	}
}

void APlayerCharacterController::HandleTertiaryActionReleased()
{
	if (InteractionComponent)
	{
		InteractionComponent->EndTertiaryInteraction();
	}
}

void APlayerCharacterController::HandlePrimaryActionPressed()
{
	if (InteractionComponent)
	{
		InteractionComponent->BeginPrimaryInteraction();
	}
}

void APlayerCharacterController::HandlePrimaryActionReleased()
{
	if (InteractionComponent)
	{
		InteractionComponent->EndPrimaryInteraction();
	}
}

void APlayerCharacterController::HandleSecondaryActionPressed()
{
	if (InteractionComponent)
	{
		InteractionComponent->BeginSecondaryInteraction();
	}
}

void APlayerCharacterController::HandleSecondaryActionReleased()
{
	if (InteractionComponent)
	{
		InteractionComponent->EndSecondaryInteraction();
	}
}

void APlayerCharacterController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	CalculateRotationMultiplier(InputRotation);
	
	AddYawInput(InputRotation.X * InteractionRotationMultiplier);
	AddPitchInput(InputRotation.Y * InteractionRotationMultiplier);

	InputRotation.Zero();
	
	Super::ProcessPlayerInput(DeltaTime, bGamePaused);
}

void APlayerCharacterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (PlayerCharacter)
	{
		UpdateCurrentActions();
		UpdatePendingActions();
		UpdatePlayerControlRotation(ControlRotation, DeltaSeconds);
	}
}

void APlayerCharacterController::UpdatePlayerControlRotation(const FRotator& Rotation, const float DeltaSeconds)
{

	if (InteractionComponent && PlayerCharacter)
	{
		const UPlayerDragComponent* DragComponent {InteractionComponent->GetDragComponent()};

		if (DragComponent && DragComponent->GetGrabbedComponent())
		{
			const FVector PlayerForwardVector {PlayerCharacter->GetVelocity().GetSafeNormal()};
			const float PlayerForwardSpeed {static_cast<float>(FVector::DotProduct(PlayerForwardVector, PlayerCharacter->GetActorForwardVector()))};
			const float PlayerSidewardSpeed {static_cast<float>(FVector::CrossProduct(PlayerForwardVector, PlayerCharacter->GetActorForwardVector()).Size())};
			
			const FVector CameraLocation {PlayerCameraManager->GetCameraLocation()};
			const FVector DraggedObjectLocation {DragComponent->GetDragLocation()};
			const FVector CameraToDraggedObjectVector {(DraggedObjectLocation - CameraLocation).GetSafeNormal()};
	
			const float DistanceToDraggedObject {static_cast<float>(FVector::Distance(CameraLocation, DraggedObjectLocation))};
			const float AngleThreshold
			{static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(80, 150), FVector2D(1, 0), DistanceToDraggedObject) * 45)};
			
			const float AngleBetweenVectors {FMath::RadiansToDegrees(acosf(FVector::DotProduct(CameraToDraggedObjectVector, GetControlRotation().Vector())))};

			if (PlayerForwardSpeed <= -1 && AngleBetweenVectors > AngleThreshold)
			{
				FVector NewForwardVector {CameraToDraggedObjectVector * FMath::Cos(FMath::DegreesToRadians(AngleThreshold))
					+ GetControlRotation().Vector() * FMath::Sin(FMath::DegreesToRadians(AngleThreshold))};
				NewForwardVector.Normalize();
		
				FRotator TargetRotation {NewForwardVector.Rotation()};
				TargetRotation.Pitch = ControlRotation.Pitch;
		
				ControlRotation = FMath::RInterpTo(ControlRotation, TargetRotation, DeltaSeconds, 1.5);
			}
	
			PlayerControlRotation = ControlRotation;
			return;
		}
	}
	
	if (CharacterConfiguration && CharacterConfiguration->IsRotationSmoothingEnabled)
	{
		PlayerControlRotation = FMath::RInterpTo(PlayerControlRotation, Rotation, DeltaSeconds, CharacterConfiguration->RotationSmoothingSpeed);
	}
	else
	{
		PlayerControlRotation = Rotation;
	}
}

void APlayerCharacterController::UpdateCurrentActions()
{
	if (!CanProcessMovementInput) {return;}
	
	/** If the character is sprinting and should no longer be sprinting, stop sprinting. */
	if (!CanCharacterSprint() || !IsSprintPending)
	{
		PlayerCharacter->StopSprinting();
	}
}

void APlayerCharacterController::UpdatePendingActions()
{
	/** If there is a sprint pending and the character is no longer sprinting, start sprinting. */
	if (IsSprintPending && !PlayerCharacter->IsSprinting() && CanCharacterSprint())
	{
		if (!PlayerCharacter->bIsCrouched)
		{
			PlayerCharacter->StartSprinting();
		}
		/** If the character is crouching, stand up before sprinting. */
		else if (PlayerCharacter->bIsCrouched && PlayerCharacter->CanStandUp())
		{
			IsCrouchPending = false;
			PlayerCharacter->UnCrouch(false);
			PlayerCharacter->StartSprinting();
		}
	}
	/** If crouch is pending and the character is not crouching, start crouching. */
	if (IsCrouchPending && !PlayerCharacter->bIsCrouched && PlayerCharacter->CanCrouch())
	{
		if (PlayerCharacter->IsSprinting())
		{
			PlayerCharacter->StopSprinting();
			IsSprintPending = false;
		}
		PlayerCharacter->Crouch(false);
	}
}

void APlayerCharacterController::CalculateRotationMultiplier(const FVector2D InputDirection)
{
	if (!InteractionComponent || !CharacterConfiguration)
    {
        InteractionRotationMultiplier = 1.0f;
        return;
    }

    const UPlayerGrabComponent* GrabComponent {InteractionComponent->GetGrabComponent()};
    const UPlayerDragComponent* DragComponent {InteractionComponent->GetDragComponent()};

    if (GrabComponent->GetGrabbedComponent() || DragComponent->GetGrabbedComponent())
    {
	    float RotationMultiplier {1.0f};

    	if (const UPrimitiveComponent* PrimitiveComponent {GrabComponent->GetGrabbedComponent() ? GrabComponent->GetGrabbedComponent() : DragComponent->GetGrabbedComponent()})
    	{
    		const float Mass {PrimitiveComponent->GetMass()};
    		const FBoxSphereBounds Bounds {PrimitiveComponent->CalcBounds(PrimitiveComponent->GetComponentTransform())};
    		const float BoundingBoxSize {static_cast<float>(Bounds.GetBox().GetVolume())};

    		const float MassRotationMultiplier {static_cast<float>(FMath::GetMappedRangeValueClamped
				(CharacterConfiguration->InteractionRotationWeightRange, CharacterConfiguration->InteractionRotationWeightScalars, Mass))};

    		const float BoundsRotationMultiplier {static_cast<float>(FMath::GetMappedRangeValueClamped
				(CharacterConfiguration->InteractionRotationSizeRange, CharacterConfiguration->InteractionRotationSizeScalars, BoundingBoxSize))};

    		float DistanceMultiplier;

    		if (GrabComponent->GetGrabbedComponent())
    		{
    			if (const UCameraComponent* CameraComponent {PlayerCharacter->GetCamera()})
    			{
    				const float Distance {static_cast<float>(FVector::Dist(PrimitiveComponent->GetComponentLocation(), CameraComponent->GetComponentLocation()))};
    				
    				DistanceMultiplier = FMath::GetMappedRangeValueClamped
					(CharacterConfiguration->InteractionRotationDistanceRange, CharacterConfiguration->InteractionRotationDistanceScalars, Distance);

    				RotationMultiplier *= FMath::Clamp(MassRotationMultiplier * BoundsRotationMultiplier, CharacterConfiguration->InteractionRotationFloor, 1.0) * DistanceMultiplier;
    			}
    		}
    		else if (DragComponent->GetGrabbedComponent())
    		{
    			if (const UCameraComponent* CameraComponent {PlayerCharacter->GetCamera()})
    			{
    				const FVector CameraLocation {CameraComponent->GetComponentLocation()};
    				const FVector CameraForwardVector {CameraComponent->GetForwardVector()};
    				const FVector DragLocation {DragComponent->GetDragLocation()};
    				const FVector DistanceToComponent {DragLocation - CameraLocation};

    				const float Angle {static_cast<float>(FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(CameraForwardVector, DistanceToComponent.GetSafeNormal()))))};

    				DistanceMultiplier = FMath::GetMappedRangeValueClamped(
						CharacterConfiguration->InteractionRotationOffsetRange, CharacterConfiguration->InteractionRotationOffsetScalars, Angle);

    				const FVector CameraToComponentDirection {DragComponent->GetDragLocation() - PlayerCameraManager->GetCameraLocation()};
    				const FVector InputDirection3D {FVector(InputDirection, 0.0f)};
    				const float DotProduct {static_cast<float>(FVector::DotProduct(InputDirection3D, CameraToComponentDirection.GetSafeNormal()))};

    				constexpr float DragMultiplier {0.2};

    				if (DotProduct > 0.0f)
    				{
    					RotationMultiplier *= FMath::Clamp(MassRotationMultiplier * BoundsRotationMultiplier, CharacterConfiguration->InteractionRotationFloor, 1.0) * DragMultiplier;
    				}
    				else
    				{
    					RotationMultiplier *= FMath::Clamp(MassRotationMultiplier * BoundsRotationMultiplier, CharacterConfiguration->InteractionRotationFloor, 1.0) * DistanceMultiplier * DragMultiplier;
    				}
    			}
    		}
    	}
    	InteractionRotationMultiplier = RotationMultiplier;
    	return;
    }
	InteractionRotationMultiplier = 1.0f;
}

bool APlayerCharacterController::GetHasMovementInput() const
{
	if (InputComponent != nullptr && InteractionComponent && !InteractionComponent->GetIsTertiaryInteractionActive())
	{
		return InputComponent->GetAxisValue("Move Longitudinal") || InputComponent->GetAxisValue("Move Lateral");
	}
	return 0.0;
}

float APlayerCharacterController::GetHorizontalRotationInput() const
{
	if (InputComponent != nullptr && InteractionComponent && !InteractionComponent->GetIsTertiaryInteractionActive())
	{
		return InputComponent->GetAxisValue("Horizontal Rotation");
	}
	return 0.0;
}

void APlayerCharacterController::SetCanProcessMovementInput(const bool Value)
{
		CanProcessMovementInput = Value;
}

void APlayerCharacterController::SetCanProcessRotationInput(const bool Value)
{
		CanProcessRotationInput = Value;
}

bool APlayerCharacterController::SetPlayerMovementInputLock(const bool Value)
{
	MovementInputLockCount += Value ? 1 : -1;
	const bool CanProcessInput {!MovementInputLockCount};
	if (GetCanProcessMovementInput() != CanProcessInput)
	{
		SetCanProcessMovementInput(CanProcessInput);
		OnMovementInputLockChanged.Broadcast(CanProcessInput);
	}
	return CanProcessInput;
}

bool APlayerCharacterController::SetPlayerRotationInputLock(const bool Value)
{
	RotationInputLockCount += Value ? 1 : -1;
	const bool CanProcessInput {!RotationInputLockCount};
	if (GetCanProcessRotationInput() != CanProcessInput)
	{
		SetCanProcessRotationInput(CanProcessInput);
		OnRotationInputLockChanged.Broadcast(CanProcessInput);
	}
	return CanProcessInput;
}

void APlayerCharacterController::FadePlayerCameraFromBlack(const float Duration)
{
	if (PlayerCharacter && PlayerCharacter->GetCameraController())
	{
		PlayerCharacter->GetCameraController()->FadeFromBlack(Duration);
	}
}

bool APlayerCharacterController::CanCharacterSprint() const
{
	return CharacterConfiguration->IsSprintingEnabled && GetCharacter()->GetMovementComponent()->IsMovingOnGround() && !GetCharacter()->bIsCrouched
			&& GetInputAxisValue("Move Longitudinal") > 0.5 && FMath::Abs(GetInputAxisValue("Move Lateral")) <= GetInputAxisValue("Move Longitudinal");
}

FHitResult APlayerCharacterController::GetCameraLookAtQuery() const
{
	constexpr float TraceLength {250.f};
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

void APlayerCharacterController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UWorld* World {GetWorld()})
	{
		if (UFirstPersonCharacterWorldSubsystem* Subsystem {World->GetSubsystem<UFirstPersonCharacterWorldSubsystem>()})
		{
			Subsystem->UnregisterPlayerController(this);
		}
	}
	Super::EndPlay(EndPlayReason);
}

UPlayerInteractionComponent* APlayerCharacterController::SearchForPlayerInteractionComponent()
{
	if (!InteractionComponent)
	{
		if(GetPawn())
		{
			InteractionComponent = Cast<UPlayerInteractionComponent>(GetPawn()->FindComponentByClass(UPlayerInteractionComponent::StaticClass()));
		}
	}
	return InteractionComponent;
}



