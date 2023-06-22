// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "PlayerCameraController.h"

#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "PlayerCharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

#if WITH_EDITOR
#include "Editor/UnrealEd/Public/Editor.h"
#endif

/** Sets default values for this component's properties. */
UPlayerCameraController::UPlayerCameraController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerCameraController::OnRegister()
{
	Super::OnRegister();
	
	/** Load the configuration asset. If no configuration asset is provided, construct a default instance of the configuration asset instead. */
	if (!Configuration)
	{
		Configuration = ConfigurationAsset.LoadSynchronous();
		if (!Configuration)
		{
			Configuration = NewObject<UPlayerCameraConfiguration>();
			Configuration->AddToRoot();
		}
	}
	
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (!PlayerCharacter) {return; }
	HeadSocketTransform = PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor);

#if WITH_EDITOR
	/** Perform a check to make sure that the game is being simulated in the editor to prevent an assertion error on this delegate. */
	if (GEditor && GEditor->IsPlayingSessionInEditor())
	{
		PlayerCharacter->ReceiveControllerChangedDelegate.AddDynamic(this, &UPlayerCameraController::HandleCharacterControllerChanged);
	}
#endif

#if !(WITH_EDITOR)
	PlayerCharacter->ReceiveControllerChangedDelegate.AddDynamic(this, &UPlayerCameraController::HandleCharacterControllerChanged);
#endif
	
	/** Apply the camera configuration. */
	if (!Configuration) {return; }
	Configuration->ApplyToCamera(PlayerCharacter->GetCamera());
}

/** Called after the pawn's controller has changed. */
void UPlayerCameraController::HandleCharacterControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	if (APlayerCharacterController* Controller {Cast<APlayerCharacterController>(NewController)})
	{
		PlayerCharacterController = Controller;
	}
}

/** Called when the game starts. */
void UPlayerCameraController::BeginPlay()
{
	Super::BeginPlay();
	/** Sets the starting color to black so that we can fade in the camera when the player is fully initialized. */
	if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		const FLinearColor Color(0.0f, 0.0f, 0.0f, 1.0f);
		PlayerController->PlayerCameraManager->SetManualCameraFade(1.0f, Color, false);
	}
	
	if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager)
		{
			CameraManager->ViewPitchMax = Configuration->MaximumViewPitch;
			CameraManager->ViewPitchMin = Configuration->MinimumViewPitch;
		}
	}
}


/** Called every frame. */
void UPlayerCameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (PlayerCharacter && PlayerCharacterController)
		if (UCameraComponent* Camera {PlayerCharacter->GetCamera()})
	{
		UpdateCameraRotation(Camera, DeltaTime); /** Even with camera sway and centripetal rotation disabled, we need to call this function every frame to update the actual orientation of the camera. */
		UpdateCameraLocation(Camera);
		if (Configuration->IsDynamicFOVEnabled)
		{
			UpdateCameraFieldOfView(Camera, DeltaTime);
		}
		if (Configuration->IsDynamicVignetteEnabled)
		{
			UpdateCameraVignetteIntensity(Camera, DeltaTime);
		}
		if (Configuration->IsDynamicDOFEnabled)
		{
			UpdateCameraDepthOfField(Camera, DeltaTime);
		}
	}
}

// Called by TickComponent.
void UPlayerCameraController::UpdateCameraLocation(UCameraComponent* Camera)
{
	/** Get an alpha value based on the pitch of the camera. We do not want the camera to explicitly follow the head socket if the body of the player isn't visible (e.g. looking down),
	 as this could be perceived as annoying by the user. */ 
	const double PitchAlpha
	{FMath::GetMappedRangeValueClamped(FVector2d(-30.0, -55.0), FVector2d(0.0, 1.0), Camera->GetComponentRotation().Pitch)};
	
	/** Get the delta position of the current head socket location in relation to the default location. This allows us to introduce some socket-bound headbobbing with scalable intensity. */
	const FVector SocketLocation
	{FVector(0, 0,(PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor).GetLocation() - HeadSocketTransform.GetLocation()).Z * 0.5)};
	
	FVector Result;
	/** If the player is looking forward or up, we don't need to perform any additional calculations and can set the relative location to the CameraConfiguration's default value. */
	if (PitchAlpha == 0.0)
	{
		Result = Configuration->CameraOffset + (SocketLocation * !PlayerCharacter->GetIsTurningInPlace());
	}
	else
	{
		/** Get the target location if the player is not looking down. */
		const FVector UprightCameraLocation {Configuration->CameraOffset + (SocketLocation * !PlayerCharacter->GetIsTurningInPlace())};

		/** Get the character's forward vector. */
		const FVector ForwardVector = PlayerCharacter->GetActorForwardVector();

		/** Project the character's velocity onto its forward vector to get the forward velocity. */
		const float ForwardVelocity = FVector::DotProduct(PlayerCharacter->GetVelocity(), ForwardVector);
		
		/** Calculate the target location if the player is looking down. */
		const FVector DownwardCameraLocation {PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor).GetLocation() + FVector(Configuration->CameraOffset.X * 0.625, 0, 0)
		- FVector(0, 0, (ForwardVelocity * 0.02))}; // We lower the camera slightly when the character is moving forward to simulate the body leaning forward.
		
		/** Interpolate between the two target locations depending on PitchAlpha. */
		Result = FMath::Lerp(UprightCameraLocation, DownwardCameraLocation, PitchAlpha); //NOTE: In UE 5.1 using FMath::Lerp() with two FVectors can cause semantic errors, but the code will compile and run just fine.
	}
	/** Rotate the result with the base aim rotation. */
	const FRotator ControlRotation {FRotator(0, PlayerCharacterController->GetPlayerControlRotation().Yaw, 0)};
	Result = ControlRotation.RotateVector(Result);
	
	/** Add the world location of the pawn to the result. */
	Result = Result + PlayerCharacter->GetActorLocation();
	
	Camera->SetWorldLocation(Result);
}

/** Called by TickComponent. */
void UPlayerCameraController::UpdateCameraRotation(const UCameraComponent*, const float DeltaTime)
{
	if (Configuration->IsCameraSwayEnabled)
	{
		GetCameraSwayRotation(Sway);
	}
	
	if (Configuration->IsCentripetalRotationEnabled)
	{
		GetCameraCentripetalRotation(CentripetalRotation);
	}
	
	if (!PlayerCharacter->GetIsTurningInPlace())
	{
		GetScaledHeadSocketDeltaRotation(SocketRotation, DeltaTime);
	}
	else
	{
		SocketRotation.Pitch = 0.0f;
		SocketRotation.Roll = 0.0f;
		SocketRotation.Yaw = 0.0f;
	}
	
		PlayerCharacter->GetCamera()->SetWorldRotation(Sway + CentripetalRotation + SocketRotation + PlayerCharacterController->GetPlayerControlRotation());
		PlayerCharacter->GetCamera()->SetWorldRotation(Sway + CentripetalRotation + SocketRotation + PlayerCharacterController->GetPlayerControlRotation());
}

/** Called by UpdateCameraRotation. */
void UPlayerCameraController::GetCameraSwayRotation(FRotator& Rotator)
{
	/** Get the current ground movement type from the PlayerController. */
	if (!PlayerCharacter->GetPlayerCharacterMovement()) { return; }
	
	const EPlayerGroundMovementType MovementType {PlayerCharacter->GetPlayerCharacterMovement()->GetGroundMovementType()};
	/** Get a oscillation multiplier value according to the ground movement type. */
	float IntensityMultiplier {0.0};
	switch(MovementType)
	{
	case EPlayerGroundMovementType::Idle: IntensityMultiplier = 0.1;
		break;
	case EPlayerGroundMovementType::Walking: IntensityMultiplier = 0.3;
		break;
	case EPlayerGroundMovementType::Sprinting: IntensityMultiplier = 1.65;
		break;
	}
	/** Get a mapped deviation value that scales the shake intensity and speed. Used to introduce some cyclical pseudo-random variance. */
	const double Deviation {FMath::GetMappedRangeValueClamped(FVector2d(-1.0, 1.00), FVector2d(0.75, 1.5),
					UKismetMathLibrary::Cos(UGameplayStatics::GetTimeSeconds(GetWorld()) * 2.4))};
	
	/** Calculate the target shake rotation. */
	const double TargetRollOffset {UKismetMathLibrary::Cos(UGameplayStatics::GetTimeSeconds(GetWorld()) * Deviation) * IntensityMultiplier * Deviation * Configuration->CameraSwayIntensity};
	
	/** Interpolate between the current camera roll and the target camera roll. */
	CameraShakeRoll = FMath::FInterpTo(CameraShakeRoll, TargetRollOffset, GetWorld()->GetDeltaSeconds(), 3.0);
	
	Rotator.Roll = CameraShakeRoll;
}

/** Called by UpdateCameraRotation. */
void UPlayerCameraController::GetCameraCentripetalRotation(FRotator& Rotator)
{
	const UPlayerCharacterMovementComponent* CharacterMovement {PlayerCharacter->GetPlayerCharacterMovement()};
	if (!CharacterMovement) { return; }
	if (Configuration->IsCentripetalRotationSprintOnly && !CharacterMovement->GetIsSprinting()) { return; }
	
	/** When the player is moving laterally while sprinting, we want the camera to lean into that direction. */
	const float LateralVelocityMultiplier {0.002353f * Configuration->VelocityCentripetalRotation};
	const float SprintMultiplier {Configuration->IsCentripetalRotationSprintOnly ? Configuration->CentripetalRotationNonSprintMultiplier : 1.0f};
	const FVector WorldVelocity {PlayerCharacter->GetMovementComponent()->Velocity};
	const FVector LocalVelocity {PlayerCharacter->GetActorTransform().InverseTransformVector(WorldVelocity)};
	const double LateralVelocityRoll {LocalVelocity.Y * LateralVelocityMultiplier * SprintMultiplier};
		
	/** When the player is rotating horizontally while sprinting, we want the camera to lean into that direction. */
	float HorizontalRotationRoll {0.0f};
	if (CharacterMovement->GetIsSprinting())
	{
		HorizontalRotationRoll = FMath::Clamp(PlayerCharacterController->GetHorizontalRotationInput() * Configuration->RotationCentripetalRotation,
				-Configuration->MaxCentripetalRotation, Configuration->MaxCentripetalRotation);
	}
	
	const double TargetRoll {LateralVelocityRoll + HorizontalRotationRoll};
	
	/** Interpolate the roll value. */
	CameraLeanRoll = FMath::FInterpTo(CameraLeanRoll, TargetRoll, GetWorld()->GetDeltaSeconds(), 4.f);
	Rotator.Roll = CameraLeanRoll;
}

void UPlayerCameraController::GetScaledHeadSocketDeltaRotation(FRotator& Rotator, const float DeltaTime)
{
	if (!PlayerCharacter->GetPlayerCharacterMovement()) { return; }
	
	
	/** Get the current ground movement type from the PlayerController. */
	const EPlayerGroundMovementType MovementType {PlayerCharacter->GetPlayerCharacterMovement()->GetGroundMovementType()};
	
	/** Get a oscillation multiplier value according to the ground movement type. */
	float IntensityMultiplier {0.0};
	if (!PlayerCharacter->GetMovementComponent()->IsFalling())
	{
		switch(MovementType)
		{
		case EPlayerGroundMovementType::Sprinting: IntensityMultiplier = 1.25;
			break;
		default: IntensityMultiplier = 0.5;
			break;
		}
	}
	
	/** Get the delta head socket rotation. */
	FRotator TargetHeadSocketRotation {(PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor).GetRotation()
		- HeadSocketTransform.GetRotation()) * IntensityMultiplier};

	/** Apply scalars. */
	const float XScalar {static_cast<float>(PlayerCharacter->bIsCrouched ? Configuration->CrouchedSocketRotationIntensity.X : Configuration->SocketRotationIntensity.X)};
	const float YScalar	{static_cast<float>(PlayerCharacter->bIsCrouched ? Configuration->CrouchedSocketRotationIntensity.Y : Configuration->SocketRotationIntensity.Y)};
	const float ZScalar	{static_cast<float>(PlayerCharacter->bIsCrouched ? Configuration->CrouchedSocketRotationIntensity.Z : Configuration->SocketRotationIntensity.Z)};
	
	TargetHeadSocketRotation = FRotator(TargetHeadSocketRotation.Pitch * XScalar, (TargetHeadSocketRotation.Yaw * ZScalar), (TargetHeadSocketRotation.Roll * YScalar));

	/** Interpolate the rotation value to smooth out jerky rotation changes. */
	if (const UWorld* World {GetWorld()})
	{
		InterpolatedHeadSocketRotation = FMath::RInterpTo(InterpolatedHeadSocketRotation, TargetHeadSocketRotation, DeltaTime, 4);
	}
	
	Rotator = InterpolatedHeadSocketRotation;
}

/** Called by TickComponent. */
void UPlayerCameraController::UpdateCameraFieldOfView(UCameraComponent* Camera, const float DeltaTime)
{
	if (const UPlayerCharacterConfiguration* CharacterConfiguration {PlayerCharacter->GetCharacterConfiguration()})
	{
		float TargetFOV {Configuration->DefaultFOV};
		const FVector WorldVelocity {PlayerCharacter->GetMovementComponent()->Velocity};
		const FVector LocalVelocity {PlayerCharacter->GetActorTransform().InverseTransformVector(WorldVelocity)};
		if (LocalVelocity.X > CharacterConfiguration->WalkSpeed * 1.1)
		{
			TargetFOV = FMath::GetMappedRangeValueClamped(FVector2D(CharacterConfiguration->WalkSpeed * 1.1, CharacterConfiguration->SprintSpeed),
						FVector2D(Configuration->DefaultFOV, Configuration->SprintFOV), LocalVelocity.X);
		} 

		Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime,2.f );
	}
}

void UPlayerCameraController::UpdateCameraVignetteIntensity(UCameraComponent* Camera, const float DeltaTime)
{
	if (PlayerCharacter->GetPlayerCharacterMovement())
	{
		const float TargetVignetteIntensity {PlayerCharacter->GetPlayerCharacterMovement()->GetIsSprinting()
			? Configuration->SprintVignetteIntensity : Configuration->DefaultVignetteIntensity};
		
		if (Camera->PostProcessSettings.VignetteIntensity != TargetVignetteIntensity)
		{
			constexpr float InterpolationSpeed {3};
			Camera->PostProcessSettings.VignetteIntensity =
				FMath::FInterpTo(Camera->PostProcessSettings.VignetteIntensity, TargetVignetteIntensity, DeltaTime, InterpolationSpeed);
		}
	}
}

void UPlayerCameraController::UpdateCameraDepthOfField(UCameraComponent* Camera, const float DeltaTime)
{
	float FocalDistance {GetFocalDistance(Camera)};
	FocalDistance = FMath::Clamp(FocalDistance, Configuration->MinimumFocalDistance, Configuration->MaximumFocalDistance);
	
	const float BlurFocus {static_cast<float>(FMath::GetMappedRangeValueClamped
		(FVector2D(Configuration->MinimumFocalDistance, Configuration->MaximumFocalDistance),
			FVector2D(Configuration->MacroBlurFocus,Configuration->LongShotBlurFocus),FocalDistance))};
	
	const float BlurAmount {static_cast<float>(FMath::GetMappedRangeValueClamped
		(FVector2D(Configuration->MinimumFocalDistance, Configuration->MaximumFocalDistance),
			FVector2D(Configuration->MacroBlurAmount,Configuration->LongShotBlurAmount),FocalDistance))};

	PlayerCharacter->GetCamera()->PostProcessSettings.DepthOfFieldFocalDistance =
		FMath::FInterpTo(Camera->PostProcessSettings.DepthOfFieldFocalDistance, FocalDistance, DeltaTime, Configuration->DynamicDofSpeed);
	
	PlayerCharacter->GetCamera()->PostProcessSettings.DepthOfFieldDepthBlurAmount = 
		FMath::FInterpTo(Camera->PostProcessSettings.DepthOfFieldDepthBlurAmount, BlurFocus, DeltaTime, Configuration->DynamicDofSpeed);
	
	PlayerCharacter->GetCamera()->PostProcessSettings.DepthOfFieldDepthBlurRadius =
		FMath::FInterpTo(Camera->PostProcessSettings.DepthOfFieldDepthBlurRadius, BlurAmount, DeltaTime, Configuration->DynamicDofSpeed);
}

float UPlayerCameraController::GetFocalDistance(UCameraComponent* Camera) const
{
	if (!PlayerCharacter)
	{
		return 0.0f;
	}
	
	FVector CameraLocation = Camera->GetComponentLocation();
	FVector ForwardVector = Camera->GetForwardVector();

	constexpr float TraceLength {50000.0f};
	ForwardVector *= TraceLength;
	FVector End = CameraLocation + ForwardVector;

	FHitResult HitResult;
	if (Camera->GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECollisionChannel::ECC_Visibility))
	{
		return (HitResult.Location - CameraLocation).Size();
	}
	return TraceLength;
}

void UPlayerCameraController::FadeFromBlack(const float Duration)
{
	if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController()){
		const FLinearColor FadeColor(0.0f, 0.0f, 0.0f, 1.0f);
		PlayerController->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, Duration, FadeColor);
	}
}

void UPlayerCameraConfiguration::ApplyToCamera(UCameraComponent* Camera)
{
	if (!Camera) { return; }
	Camera->SetFieldOfView(DefaultFOV);
	Camera->PostProcessSettings.VignetteIntensity = DefaultVignetteIntensity;
}






