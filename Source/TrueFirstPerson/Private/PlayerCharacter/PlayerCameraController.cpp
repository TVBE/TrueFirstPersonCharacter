// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraController.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "Camera/CameraComponent.h"
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
		UpdateCameraFieldOfView();
	}
	
}

// Called every frame
void UPlayerCameraController::UpdateCameraFieldOfView()
{
	if(PlayerCharacter->GetIsSprinting() && PlayerCharacter->GetCamera()->FieldOfView != CameraConfiguration.SprintFOV ||
		!PlayerCharacter->GetIsSprinting() && PlayerCharacter->GetCamera()->FieldOfView != CameraConfiguration.DefaultFOV)
	{
		float TargetFOV = PlayerCharacter->GetIsSprinting() ? CameraConfiguration.SprintFOV : CameraConfiguration.DefaultFOV;
		PlayerCharacter->GetCamera()->FieldOfView = FMath::FInterpTo(PlayerCharacter->GetCamera()->FieldOfView, TargetFOV, GetWorld()->GetDeltaSeconds(),2.f );
	}
	
	
}





