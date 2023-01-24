// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraController.h"

#include <string>

#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TrueFirstPerson/TrueFirstPerson.h"

// Sets default values for this component's properties
UPlayerCameraController::UPlayerCameraController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Initializes the component. Occurs at level startup or actor spawn. This is before BeginPlay.
void UPlayerCameraController::InitializeComponent()
{
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if(PlayerCharacter != nullptr)
	{
		HeadSocketLocation = PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor).GetLocation();
		PlayerCharacter->ReceiveControllerChangedDelegate.AddDynamic(this, &UPlayerCameraController::HandleCharacterControllerChanged);

		// Set the default camera properties for the PlayerCharacter's camera.
		PlayerCharacter->GetCamera()->SetFieldOfView(CameraConfiguration.DefaultFOV);
	}
	else
	{
		FString Class {GetOwner()->GetClass()->GetDisplayNameText().ToString()};
		UE_LOG(LogPlayerCameraController, Error, TEXT("PlayerCameraController expected pawn of type APlayerCharacter, is attached to Actor of type %s instead"), *Class);
	}
	FString PawnInstance {GetOwner()->GetActorNameOrLabel()};
	UE_LOG(LogPlayerCameraController, VeryVerbose, TEXT("CameraController initialized for %s."), *PawnInstance);
	Super::InitializeComponent();
}

// Called after the pawn's controller has changed.
void UPlayerCameraController::HandleCharacterControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	APlayerCharacterController* Controller {Cast<APlayerCharacterController>(NewController)};
	if(Controller != nullptr)
	{
		PlayerCharacterController = Controller;
	}
}

// Called when the game starts
void UPlayerCameraController::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPlayerCameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(PlayerCharacter != nullptr && PlayerCharacterController != nullptr)
	{
		UpdateCameraRotation();
		UpdateCameraLocation();
		UpdateCameraFieldOfView();
	}
	
}

// Called by TickComponent
void UPlayerCameraController::UpdateCameraLocation()
{
	/* Get an alpha value based on the pitch of the camera. We do not want the camera to explicitly follow the head socket if the body of the player isn't visible (e.g. looking down),
	 as this could be perceived as annoying by the user. */ 
	const double PitchAlpha
	{FMath::GetMappedRangeValueClamped(FVector2d(-30.0, -55.0), FVector2d(0.0, 1.0), PlayerCharacter->GetCamera()->GetComponentRotation().Pitch)};

	// UE_LOG(LogPlayerCameraController, Log, TEXT(PitchAlpha: "%f"), PitchAlpha);

	// Get the delta position of the current head socket location in relation to the default location. This allows us to introduce some socket-bound headbobbing with scalable intensity.
	const FVector DeltaPosition {FVector(0, 0,(PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor).GetLocation() - HeadSocketLocation).Z * 0.5)};
	
	FVector Result {FVector()};
	// If the player is looking forward or up, we don't need to perform any additional calculations and can set the relative location to the CameraConfiguration's default value.
	if(PitchAlpha == 0.0)
	{
		Result = CameraConfiguration.CameraOffset + DeltaPosition;
	}
	else
	{
		// Get the target location if the player is not looking down.
		FVector UprightCameraLocation {CameraConfiguration.CameraOffset + DeltaPosition};
		
		// Calculate the target location if the player is looking down.
		FVector DownwardCameraLocation {PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor).GetLocation() + FVector(CameraConfiguration.CameraOffset.X * 0.625, 0, 0)
		- FVector(0, 0, (PlayerCharacter->GetVelocity().X * 0.02))};
		
		// Interpolate between the two target locations depending on PitchAlpha.
		Result = FMath::Lerp(UprightCameraLocation, DownwardCameraLocation, PitchAlpha); //NOTE: In UE 5.1 using FMath::Lerp with two FVectors can cause semantic errors, but the code will compile just fine.
	}
	PlayerCharacter->GetCamera()->SetRelativeLocation(Result);

}

// Called by TickComponent
void UPlayerCameraController::UpdateCameraRotation()
{
	const FRotator Roll {GetCameraShakeOffset() + GetCameraLeanOffset()};
	PlayerCharacter->GetCamera()->SetWorldRotation(Roll + PlayerCharacter->GetControlRotation());
}

// Called by UpdateCameraRotation
FRotator UPlayerCameraController::GetCameraShakeOffset()
{
	// Get the current ground movement type from the PlayerController.
	EPlayerGroundMovementType MovementType {PlayerCharacterController->GetGroundMovementType()};
	// Get a oscillation multiplier value according to the ground movement type.
	float IntensityMultiplier {0.0};
	switch(MovementType)
	{
	case 0: IntensityMultiplier = 0.1; // Idle
		break;
	case 1: IntensityMultiplier = 0.3; // Walking
		break;
	case 2: IntensityMultiplier = 1.65; // Sprinting
		break;
	default: IntensityMultiplier = 0.3; // Miscellaneous
		break;
	}
	// Multiply the OscillationMultiplier with the CameraConfiguration CameraShakeIntensity scalar.
	IntensityMultiplier = IntensityMultiplier * CameraConfiguration.CameraShakeIntensity;
	
	// Get a mapped deviation value that scales the shake intensity and speed. Used to introduce some cyclical pseudo-random variance.
	double Deviation {FMath::GetMappedRangeValueClamped(FVector2d(-1.0, 1.00), FVector2d(0.75, 1.5), FMath::Cos(GetWorld()->GetDeltaSeconds() * 2.4))};
	
	// Calculate the target shake rotation.
	double TargetRollOffset {FMath::Cos(GetWorld()->GetDeltaSeconds() * Deviation) * IntensityMultiplier * Deviation};
	
	// Interpolate between the current camera roll and the target camera roll.
	CameraShakeRoll = FMath::FInterpTo(CameraShakeRoll, TargetRollOffset, GetWorld()->GetDeltaSeconds(), 6.0);
	
	// Return a rotator with the camera roll offset.
	return FRotator(0, 0, CameraShakeRoll);
}

// Called by UpdateCameraRotation
FRotator UPlayerCameraController::GetCameraLeanOffset()
{
	FRotator Rotation {FRotator()};
	double TargetRoll {0.0};
	if(PlayerCharacter->GetIsSprinting())
	{
		constexpr float LateralVelocityMultiplier {0.002353};
		const FVector WorldVelocity {PlayerCharacter->GetMovementComponent()->Velocity};
		const FVector LocalVelocity {PlayerCharacter->GetActorTransform().InverseTransformVector(WorldVelocity)};
		const double LateralVelocityRoll {LocalVelocity.Y * LateralVelocityMultiplier};
		
		// When the player is rotating horizontally while sprinting, we want the camera to lean into that direction.
		constexpr float MaxRotationLean {14.f};
		const float HorizontalRotationRoll{FMath::Clamp(PlayerCharacterController->GetHorizontalRotationInput() * 2, -MaxRotationLean, MaxRotationLean)};;

		TargetRoll = LateralVelocityRoll + HorizontalRotationRoll;
	}
	// Interpolate the roll value.
	CameraLeanRoll = FMath::FInterpTo(CameraLeanRoll, TargetRoll, GetWorld()->GetDeltaSeconds(), 4.f);
	Rotation = (FRotator(0, 0, CameraLeanRoll));
	return Rotation;
}


// Called by TickComponent
void UPlayerCameraController::UpdateCameraFieldOfView()
{
	FPlayerCharacterConfiguration Configuration {PlayerCharacterController->GetPlayerCharacterConfiguration()};
	float TargetFOV {CameraConfiguration.DefaultFOV};
	const FVector WorldVelocity {PlayerCharacter->GetMovementComponent()->Velocity};
	const FVector LocalVelocity {PlayerCharacter->GetActorTransform().InverseTransformVector(WorldVelocity)};
	if (LocalVelocity.X > Configuration.WalkSpeed * 1.1)
	{
		TargetFOV = FMath::GetMappedRangeValueClamped(FVector2D(Configuration.WalkSpeed * 1.1, Configuration.SprintSpeed),
			FVector2D(CameraConfiguration.DefaultFOV, CameraConfiguration.SprintFOV), LocalVelocity.X);
	} 

	PlayerCharacter->GetCamera()->FieldOfView = FMath::FInterpTo(PlayerCharacter->GetCamera()->FieldOfView, TargetFOV, GetWorld()->GetDeltaSeconds(),2.f );
	
	// Implementation that only checks if the player is sprinting
	/* if(PlayerCharacter->GetIsSprinting() && PlayerCharacter->GetCamera()->FieldOfView != CameraConfiguration.SprintFOV ||
		!PlayerCharacter->GetIsSprinting() && PlayerCharacter->GetCamera()->FieldOfView != CameraConfiguration.DefaultFOV)
	{
		float TargetFOV = PlayerCharacter->GetIsSprinting() ? CameraConfiguration.SprintFOV : CameraConfiguration.DefaultFOV;
		PlayerCharacter->GetCamera()->FieldOfView = FMath::FInterpTo(PlayerCharacter->GetCamera()->FieldOfView, TargetFOV, GetWorld()->GetDeltaSeconds(),2.f );
	} */
}





