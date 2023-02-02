// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraController.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "TrueFirstPerson/TrueFirstPerson.h"

// Sets default values for this component's properties.
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
		HeadSocketTransform = PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor);
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

// Called when the game starts.
void UPlayerCameraController::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame.
void UPlayerCameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(PlayerCharacter != nullptr && PlayerCharacterController != nullptr)
	{
		UpdateCameraRotation(); // Even with camera sway and centripetal rotation disabled, we need to call this function every frame to update the actual orientation of the camera.
		UpdateCameraLocation();
		if(CameraConfiguration.EnableDynamicFOV)
		{
			UpdateCameraFieldOfView();
		}
		
	}
	
}

// Called by TickComponent.
void UPlayerCameraController::UpdateCameraLocation()
{
	/* Get an alpha value based on the pitch of the camera. We do not want the camera to explicitly follow the head socket if the body of the player isn't visible (e.g. looking down),
	 as this could be perceived as annoying by the user. */ 
	const double PitchAlpha
	{FMath::GetMappedRangeValueClamped(FVector2d(-30.0, -55.0), FVector2d(0.0, 1.0), PlayerCharacter->GetCamera()->GetComponentRotation().Pitch)};
	
	// Get the delta position of the current head socket location in relation to the default location. This allows us to introduce some socket-bound headbobbing with scalable intensity.
	const FVector SocketLocation {FVector(0, 0,(PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor).GetLocation() - HeadSocketTransform.GetLocation()).Z * 0.5)};
	
	FVector Result {FVector()};
	// If the player is looking forward or up, we don't need to perform any additional calculations and can set the relative location to the CameraConfiguration's default value.
	if(PitchAlpha == 0.0)
	{
		Result = CameraConfiguration.CameraOffset + SocketLocation;
	}
	else
	{
		// Get the target location if the player is not looking down.
		FVector UprightCameraLocation {CameraConfiguration.CameraOffset + SocketLocation};
		
		// Calculate the target location if the player is looking down.
		FVector DownwardCameraLocation {PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor).GetLocation() + FVector(CameraConfiguration.CameraOffset.X * 0.625, 0, 0)
		- FVector(0, 0, (PlayerCharacter->GetVelocity().X * 0.02))}; // We lower the camera slightly when the character is moving forward to simulate the body leaning forward.
		
		// Interpolate between the two target locations depending on PitchAlpha.
		Result = FMath::Lerp(UprightCameraLocation, DownwardCameraLocation, PitchAlpha); //NOTE: In UE 5.1 using FMath::Lerp with two FVectors can cause semantic errors, but the code will compile just fine.
	}
	PlayerCharacter->GetCamera()->SetRelativeLocation(Result);

}

// Called by TickComponent.
void UPlayerCameraController::UpdateCameraRotation()
{
	const FRotator Sway {CameraConfiguration.EnableCameraSway ? GetCameraSwayRotation() : FRotator()};
	const FRotator CentripetalRotation {CameraConfiguration.EnableCentripetalRotation ? GetCameraCentripetalRotation() : FRotator()};
	const FRotator SocketRotation {GetScaledHeadSocketDeltaRotation()};
	PlayerCharacter->GetCamera()->SetWorldRotation(Sway + CentripetalRotation + SocketRotation + PlayerCharacter->GetControlRotation());
}

// Called by UpdateCameraRotation.
FRotator UPlayerCameraController::GetCameraSwayRotation()
{
	// Get the current ground movement type from the PlayerController.
	const EPlayerGroundMovementType MovementType {PlayerCharacterController->GetGroundMovementType()};
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
	// Get a mapped deviation value that scales the shake intensity and speed. Used to introduce some cyclical pseudo-random variance.
	const double Deviation {FMath::GetMappedRangeValueClamped(FVector2d(-1.0, 1.00), FVector2d(0.75, 1.5),
					UKismetMathLibrary::Cos(UGameplayStatics::GetTimeSeconds(GetWorld()) * 2.4))};
	
	// Calculate the target shake rotation.
	float Intensity {CameraConfiguration.CameraShakeIntensity};
	const double TargetRollOffset {UKismetMathLibrary::Cos(UGameplayStatics::GetTimeSeconds(GetWorld()) * Deviation) * IntensityMultiplier * Deviation * CameraConfiguration.CameraShakeIntensity};
	
	// Interpolate between the current camera roll and the target camera roll.
	CameraShakeRoll = FMath::FInterpTo(CameraShakeRoll, TargetRollOffset, GetWorld()->GetDeltaSeconds(), 3.0);
	
	// Return a rotator with the camera roll offset.
	return FRotator(0, 0, CameraShakeRoll);
}

// Called by UpdateCameraRotation.
FRotator UPlayerCameraController::GetCameraCentripetalRotation()
{
	FRotator Rotation {FRotator()};
	double TargetRoll {0.0};
	if(PlayerCharacter->GetIsSprinting())
	{
		// When the player is moving laterally while sprinting, we want the camera to lean into that direction.
		const float LateralVelocityMultiplier {0.002353f * CameraConfiguration.VelocityCentripetalRotation};
		const FVector WorldVelocity {PlayerCharacter->GetMovementComponent()->Velocity};
		const FVector LocalVelocity {PlayerCharacter->GetActorTransform().InverseTransformVector(WorldVelocity)};
		const double LateralVelocityRoll {LocalVelocity.Y * LateralVelocityMultiplier};
		
		// When the player is rotating horizontally while sprinting, we want the camera to lean into that direction.
		const float HorizontalRotationRoll{FMath::Clamp(PlayerCharacterController->GetHorizontalRotationInput() * CameraConfiguration.RotationCentripetalRotation,
					-CameraConfiguration.MaxCentripetalRotation, CameraConfiguration.MaxCentripetalRotation)};

		TargetRoll = LateralVelocityRoll + HorizontalRotationRoll;
	}
	// Interpolate the roll value.
	CameraLeanRoll = FMath::FInterpTo(CameraLeanRoll, TargetRoll, GetWorld()->GetDeltaSeconds(), 4.f);
	Rotation = (FRotator(0, 0, CameraLeanRoll));
	return Rotation;
}

FRotator UPlayerCameraController::GetScaledHeadSocketDeltaRotation()
{
	// Get the current ground movement type from the PlayerController.
	const EPlayerGroundMovementType MovementType {PlayerCharacterController->GetGroundMovementType()};
	// Get a oscillation multiplier value according to the ground movement type.
	float IntensityMultiplier {0.0};
	if(!PlayerCharacter->GetMovementComponent()->IsFalling())
	{
		switch(MovementType)
		{
		case 2: IntensityMultiplier = 1.25; // Sprinting
			break;
		default: IntensityMultiplier = 0.5; // Miscellaneous
			break;
		}
	}
	// Get the delta head socket rotation.
	FRotator TargetHeadSocketRotation {(PlayerCharacter->GetMesh()->GetSocketTransform("head", RTS_Actor).GetRotation()
		- HeadSocketTransform.GetRotation()) * IntensityMultiplier};

	// Apply scalars
	TargetHeadSocketRotation = FRotator(TargetHeadSocketRotation.Pitch, (TargetHeadSocketRotation.Yaw * 0), (TargetHeadSocketRotation.Roll * 1.5));

	// Interpolate the rotation value to smooth out jerky rotation changes.
	if(const UWorld* World {GetWorld()})
	{
		InterpolatedHeadSocketRotation = FMath::RInterpTo(InterpolatedHeadSocketRotation, TargetHeadSocketRotation, World->GetDeltaSeconds(), 4);
	}
	return InterpolatedHeadSocketRotation;
}

// Called by TickComponent.
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
}





