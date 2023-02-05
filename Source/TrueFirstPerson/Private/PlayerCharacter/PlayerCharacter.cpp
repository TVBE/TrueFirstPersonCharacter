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
// Define macros

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

#if WITH_EDITOR
	if(!(IsRunningCommandlet() && UE::IsSavingPackage()))
	{
		/** Check if this instance of a PlayerCharacter is a blueprint derived class or not. */
		ValidateObject(this, "PlayerCharacter");
		/** Check if all components have been succesfully initialized. */
		ValidateObject(CameraController, "CameraController");
		ValidateObject(FlashlightController, "FlashlightController");
		ValidateObject(AudioController, "AudioController");
		ValidateObject(VFXController, "VFXController");
	}
#endif
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

#if WITH_EDITOR
void APlayerCharacter::ValidateObject(const UObject* Object, const FString ObjectName)
{
	if(!Object)
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("%s was not properly initialized during the construction of the PlayerCharacter."), *ObjectName); \
	}
	else if (!IsBlueprintClass(Object))
	{
		UE_LOG(LogPlayerCharacter, Warning, TEXT("%s is not a blueprint derived class. Please implement a blueprint derived class to allow designers to extend functionality."), *ObjectName);
	}
	
}
#endif

















