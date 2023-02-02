// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PlayerAudioController.h"
#include "PlayerCameraController.h"
#include "PlayerCharacterMovementComponent.h"
#include "PlayerFlashlightController.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/Vector.h"
#include "TrueFirstPerson/TrueFirstPerson.h"

#define IS_BLUEPRINT(Object) (Object->GetClass()->GetName().StartsWith("BP_") || Object->GetClass()->GetName().StartsWith("BPC_"))

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct Camera.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(this->RootComponent);
	Camera->SetRelativeLocation(FVector(22.0, 0.0, 75.0));
	Camera->FieldOfView = 90.0;
	Camera->bUsePawnControlRotation = false;

	// Construct FlashlightSpringArm.
	FlashlightSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Flashlight Spring Arm"));
	FlashlightSpringArm->SetupAttachment(this->RootComponent);
	FlashlightSpringArm->SetRelativeLocation(FVector(20.12,2.6,40.9));
	FlashlightSpringArm->SetComponentTickEnabled(false); // We only want the flashlight spring arm to update when the flashlight is enabled.
	FlashlightSpringArm->TargetArmLength = 0.0;
	FlashlightSpringArm->bDoCollisionTest = false;
	FlashlightSpringArm->bUsePawnControlRotation = false;
	FlashlightSpringArm->bEnableCameraLag = false;
	FlashlightSpringArm->bEnableCameraRotationLag = true;
	FlashlightSpringArm->CameraRotationLagSpeed = 8.5;
	
	// Construct Flashlight.
	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(FlashlightSpringArm);
	Flashlight->Intensity = 5000.0;
	Flashlight->SetVisibility(false); // We don't want the flashlight to be enabled on startup.
	
	// Construct CameraController
	CameraController = CreateDefaultSubobject<UPlayerCameraController>(TEXT("Player Camera Controller"));
	CameraController->bWantsInitializeComponent = true;
	
	// Construct FlashlightController
	FlashlightController = CreateDefaultSubobject<UPlayerFlashlightController>(TEXT("Player Flashlight Controller"));
	FlashlightController->bWantsInitializeComponent = true;

	// Construct Audio Controller
	AudioController = CreateDefaultSubobject<UPlayerAudioController>(TEXT("Player Audio Controller"));
	AudioController->bWantsInitializeComponent = true;

	// Construct VFX Controller
	VFXController = CreateDefaultSubobject<UPlayerVFXController>(TEXT("Player VFX Controller"));
	VFXController->bWantsInitializeComponent = true;

	static ConstructorHelpers::FClassFinder<APlayerCharacter> Blueprint(TEXT("/Game/PlayerCharacter/Blueprint/BP_PlayerCharacter"));
	if(!(Blueprint.Succeeded() || this->GetClass()->GetName().StartsWith("BP_"))) // Check if this class is a Blueprint derived class.
		{
		UE_LOG(LogPlayerCharacter, Error, TEXT("Non Blueprint derived instance of APlayerCharacter is created. "
		"APlayerCharacter was specifically designed to have a Blueprint derived class to extend functionality."
		"Try to create an instance of a Blueprint derived class of APlayerCharacter instead."))
		}
	
}

void APlayerCharacter::PostInitProperties()
{
	Super::PostInitProperties();
	
	if(UPlayerCharacterMovementComponent* PlayerCharacterMovementComponent {Cast<UPlayerCharacterMovementComponent>(GetCharacterMovement())})
	{
		PlayerCharacterMovement = PlayerCharacterMovementComponent;
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("PlayerCharacter does not have PlayerCharacterMovementComponent set as its CharacterMovementComponent."))
	}

	/** Check if our components are blueprint derived classes or not. */
	if(!IS_BLUEPRINT(CameraController))
	{
		// UE_LOG(LogPlayerCharacter, Warning, TEXT(""))
	}
	if(!IS_BLUEPRINT(FlashlightController))
	{
		// UE_LOG(LogPlayerCharacter, Warning, TEXT(""))
	}
	if(!IS_BLUEPRINT(AudioController))
	{
		// UE_LOG(LogPlayerCharacter, Warning, TEXT(""))
	}
	if(!IS_BLUEPRINT(VFXController))
	{
		// UE_LOG(LogPlayerCharacter, Warning, TEXT(""))
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
	
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::SetIsJumping(bool Value)
{
	IsJumping = Value;
}

// Called by the PlayerController
void APlayerCharacter::SetIsSprinting(bool Value, APlayerController* PlayerController)
{
	if(Controller == GetController())
	{
		IsSprinting = Value;
	}
}

















