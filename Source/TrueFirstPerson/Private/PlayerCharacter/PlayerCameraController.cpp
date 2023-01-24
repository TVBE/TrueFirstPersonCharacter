// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraController.h"

#include <string>

#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TrueFirstPerson/TrueFirstPerson.h"

// Sets default values for this component's properties
UPlayerCameraController::UPlayerCameraController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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
	
	if(PlayerCharacter != nullptr)
	{
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

	UE_LOG(LogPlayerCameraController, Warning, TEXT("%f"), PitchAlpha);

	// Get the delta position of the current head socket location in relation to the default location
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
		Result = FMath::Lerp(UprightCameraLocation, DownwardCameraLocation, PitchAlpha); //NOTE: In UE 5.1 using FMath::Lerp with two FVectors will cause semantic errors, but the code will compile just fine.
	}
	PlayerCharacter->GetCamera()->SetRelativeLocation(Result);

}


// Called by TickComponent
void UPlayerCameraController::UpdateCameraFieldOfView()
{
	if(PlayerCharacter->GetIsSprinting() && PlayerCharacter->GetCamera()->FieldOfView != CameraConfiguration.SprintFOV ||
		!PlayerCharacter->GetIsSprinting() && PlayerCharacter->GetCamera()->FieldOfView != CameraConfiguration.DefaultFOV)
	{
		float TargetFOV = PlayerCharacter->GetIsSprinting() ? CameraConfiguration.SprintFOV : CameraConfiguration.DefaultFOV;
		PlayerCharacter->GetCamera()->FieldOfView = FMath::FInterpTo(PlayerCharacter->GetCamera()->FieldOfView, TargetFOV, GetWorld()->GetDeltaSeconds(),2.f );
	}
	
	
}





