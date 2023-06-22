// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "PlayerFlashlightComponent.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterMovementComponent.h"
#include "LogCategories.h"

#include "Components/SpotLightComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_CLASS(UPlayerFlashlightComponent, LogPlayerFlashlightComponent)

/** Sets default values for this component's properties. */
UPlayerFlashlightComponent::UPlayerFlashlightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPlayerFlashlightComponent::OnRegister()
{
	Super::OnRegister();

	/** Load the configuration asset. If no configuration asset is provided, construct a default instance of the configuration asset instead. */
	if (!Configuration)
	{
		Configuration = ConfigurationAsset.LoadSynchronous();
		if (!Configuration)
		{
			Configuration = NewObject<UPlayerFlashlightConfiguration>();
			Configuration->AddToRoot();
		}
	}
	
	/** Get a pointer to the member components of the PlayerCharacter this flashlight is part of. */
	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (!PlayerCharacter) { return; }
	Mesh = PlayerCharacter->GetMesh();
	Camera = PlayerCharacter->GetCamera();
	Movement = PlayerCharacter->GetPlayerCharacterMovement();
	if (!Mesh || !Camera || !Movement) { return; }
	
	/** Construct FlashlightSpringArm. */
	FlashlightSpringArm = Cast<USpringArmComponent>(GetOwner()->AddComponentByClass(USpringArmComponent::StaticClass(), false, FTransform(), false));
	if (!FlashlightSpringArm) { return; }

	/** Place the spring arm at the right location depending on the attachment context of the flashlight configuration asset. */
	FVector RelativeLocation;
	switch(Configuration->AttachmentContext)
	{
	case EFlashlightSocketContext::Chest:
		RelativeLocation = FVector(Mesh->GetSocketTransform(Configuration->ChestSocket, RTS_Actor).GetLocation());
		break;
	case EFlashlightSocketContext::Head:
		RelativeLocation = FVector(Mesh->GetSocketTransform(Configuration->HeadSocket, RTS_Actor).GetLocation());
		break;
	default: RelativeLocation = FVector(Camera->GetRelativeTransform().GetLocation());
	}
	FlashlightSpringArm->SetRelativeLocation(RelativeLocation);

	/** Set some initial properties for the spring arm component. */
	FlashlightSpringArm->SetComponentTickEnabled(false);
	FlashlightSpringArm->TargetArmLength = 0.0;
	FlashlightSpringArm->bDoCollisionTest = false;
	FlashlightSpringArm->bUsePawnControlRotation = false;
	FlashlightSpringArm->bEnableCameraLag = false;
	FlashlightSpringArm->bEnableCameraRotationLag = true;

	/** Construct Flashlight. */
	Flashlight = Cast<USpotLightComponent>(GetOwner()->AddComponentByClass(USpotLightComponent::StaticClass(), false, FTransform(), false));
	if (!Flashlight) { return; }
	Flashlight->AttachToComponent(FlashlightSpringArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Flashlight->SetVisibility(false);

	/** Apply the flashlight configuration data asset to this component. */
	Configuration->ApplyToFlashlightComponent(this);
}

/** Called when the game starts */
void UPlayerFlashlightComponent::BeginPlay()
{
	if (!Mesh || !Camera || !Movement) { return; }
	Super::BeginPlay();
}



/** Called every frame. */
void UPlayerFlashlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Mesh || !Camera || !Movement || !Flashlight || !FlashlightSpringArm)
	{
		SetComponentTickEnabled(false);
		SetFlashlightEnabled(false);
		UE_LOG(LogPlayerFlashlightComponent, Error, TEXT("Some member properties are null, disabled flashlight."))
		return;
	}
	UpdateMovementAlpha(DeltaTime);
		
	const FRotator IdleRotation {GetFlashlightFocusRotation() + GetFlashlightSwayRotation()};
	const FRotator MovementRotation {(GetSocketRotationWithOffset("spine_05", Movement->GetGroundMovementType()) + IdleRotation).GetNormalized()};
		
	const FQuat IdleQuaternion {IdleRotation.Quaternion()};
	const FQuat MovementQuaternion {MovementRotation.Quaternion()};
		
	const FRotator TargetRotation {(FQuat::Slerp(IdleQuaternion, MovementQuaternion, MovementAlpha)).Rotator()};
	
	FlashlightSpringArm->SetWorldRotation(TargetRotation);
}

void UPlayerFlashlightComponent::UpdateMovementAlpha(const float DeltaTime)
{
	const bool IsMoving {Movement->Velocity.Length() > 1};
	if (MovementAlpha != static_cast<int8>(IsMoving))
	{
			constexpr float InterpolationSpeed {4};
			MovementAlpha = FMath::FInterpTo(MovementAlpha, IsMoving, DeltaTime, InterpolationSpeed);
	}
}

FRotator UPlayerFlashlightComponent::GetFlashlightFocusRotation() const
{
	FVector Target {FVector()};
	
	FVector TraceStart {Camera->GetComponentLocation()};
	FVector TraceEnd {Camera->GetForwardVector() * 5000 + TraceStart};
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	if  (this->GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
	{
		Target = HitResult.ImpactPoint;
	}
	else
	{
		Target = TraceEnd;
	}
		
	FRotator Rotation {FRotationMatrix::MakeFromX(Target - Flashlight->GetComponentLocation()).Rotator()};
	constexpr float PitchRange {60};
	Rotation = FRotator(FMath::Clamp(Rotation.Pitch, -PitchRange, PitchRange), Rotation.Yaw, Rotation.Roll);
	return Rotation;
}

FRotator UPlayerFlashlightComponent::GetFlashlightSwayRotation() const
{
	FRotator Rotation {FRotator()};
	
	const EPlayerGroundMovementType MovementType {Movement->GetGroundMovementType()};
	const float MappedVelocity {static_cast<float>(FMath::Clamp(Movement->Velocity.Length() * 0.0325, 0.2f, 1.f))};
		
	/** Variables to store the sway speed and intensity for each axis. */
	float PitchSwaySpeed {0.f};
	float YawSwaySpeed {0.f};
	float RollSwaySpeed {0.f};

	float PitchSwayIntensity {0.f};
	float YawSwayIntensity {0.f};
	float RollSwayIntensity {0.f};

	/** Set the sway speed and intensity based on the player's movement type. */
	switch(MovementType)
	{
	case EPlayerGroundMovementType::Idle:
		PitchSwaySpeed = 1.65f;
		PitchSwayIntensity = 1.7f;
		YawSwaySpeed = 1.23f;
		YawSwayIntensity = 1.25f;
		RollSwaySpeed = 0.675f;
		RollSwayIntensity = 1.5f;
		break;
	case EPlayerGroundMovementType::Walking:
		PitchSwaySpeed = 3.12f * MappedVelocity;
		PitchSwayIntensity = 1.7f;
		YawSwaySpeed = 4.65 * MappedVelocity;
		YawSwayIntensity = 1.25f;
		RollSwaySpeed = 2.55f * MappedVelocity;
		RollSwayIntensity = 1.5f; ;
		break;
	case EPlayerGroundMovementType::Sprinting:
		PitchSwaySpeed = 9.55f;
		PitchSwayIntensity = 3.21f;
		YawSwaySpeed = 5.0f;
		YawSwayIntensity = 1.5f;
		RollSwaySpeed = 3.54f;
		RollSwayIntensity = 1.56f;
		break;
	}
		
	if (const UWorld* World {GetWorld()})
	{
		const double GameTime {World->GetTimeSeconds()};
			
		/** Calculate a multiplier for the pitch sway intensity using a mapped range value of the cosine of the game time multiplied by an arbitrary value. */
		const float PitchIntensityMultiplier {static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(-1, 1), FVector2D(0.75, 1.5),FMath::Cos(GameTime * 2.13f)))};
		/** Calculate a multiplier for the yaw sway intensity using a mapped range value of the cosine of the game time multiplied by an arbitrary value. */
		const float YawIntensityMultiplier {static_cast<float>(FMath::GetMappedRangeValueClamped(FVector2D(-1, 1), FVector2D(0.75, 1.5),FMath::Cos(GameTime * 3.02f)))};
			
		PitchSwayIntensity = PitchSwayIntensity * PitchIntensityMultiplier;
		YawSwayIntensity = YawSwayIntensity * YawIntensityMultiplier;

		Rotation.Pitch = FMath::Cos(GameTime * PitchSwaySpeed) * PitchSwayIntensity * Configuration->RotationSway;
		Rotation.Yaw = FMath::Cos(GameTime * YawSwaySpeed) * YawSwayIntensity * Configuration->RotationSway;
		Rotation.Roll = FMath::Cos(GameTime * RollSwaySpeed) * RollSwayIntensity * Configuration->RotationSway;
		
	}
	return Rotation;
}

FRotator UPlayerFlashlightComponent::GetSocketRotationWithOffset(const FName Socket, const EPlayerGroundMovementType MovementType) const
{
	const FRotator SocketRotation {Mesh->GetSocketTransform(Socket, RTS_Actor).Rotator()};
	double Pitch {SocketRotation.Pitch};
	double Yaw {SocketRotation.Yaw};
		
	const bool IsSprinting {MovementType == EPlayerGroundMovementType::Sprinting};

	/** Constants to tweak the flashlight's orientation when sprinting or walking. */
	const float PitchOffset {MovementType == EPlayerGroundMovementType::Sprinting ? 70.0f : 80.0f};
	const float PitchMultiplier {MovementType == EPlayerGroundMovementType::Sprinting ? 1.45f : 0.75f};
	const float YawMultiplier {MovementType == EPlayerGroundMovementType::Sprinting ? 0.175f : 0.075f};
		
	/** Select an 2-dimensional vector to compensate for socket rotation deviation between different animations. */
	FVector2D Offset;
	switch(MovementType)
	{
	case EPlayerGroundMovementType::Idle: Offset = Configuration->IdleOffset;
		break;
	case EPlayerGroundMovementType::Walking: Offset = Configuration->WalkingOffset;
		break;
	case EPlayerGroundMovementType::Sprinting: Offset = Configuration->SprintingOffset;
		break;
	default: Offset = Configuration->IdleOffset;
	}
		
	/** Offset adjustments. */
	Pitch = ((Pitch - PitchOffset) * PitchMultiplier * Configuration->SocketRotation - 1.5f) * 0.4;
	Yaw = Yaw * YawMultiplier * Configuration->SocketRotation - 2.4f;
	FRotator Rotation {FRotator(Pitch, Yaw, 0)};
		
	Rotation += FRotator(Offset.Y, Offset.X, 0);
		
	return Rotation;
}

void UPlayerFlashlightComponent::SetFlashlightEnabled(const bool Value)
{
	if (Flashlight && FlashlightSpringArm)
	{
		SetComponentTickEnabled(Value);
		FlashlightSpringArm->SetComponentTickEnabled(Value);
		Flashlight->SetVisibility(Value);
	}
}

bool UPlayerFlashlightComponent::IsFlashlightEnabled() const
{
	if (Flashlight)
	{
		return Flashlight->IsVisible();
	}
	return false;
}

void UPlayerFlashlightComponent::CleanupComponent()
{
	if (Flashlight)
	{
		Flashlight->SetVisibility(false);
		Flashlight->DestroyComponent();
		Flashlight = nullptr;
	}
	if (FlashlightSpringArm)
	{
		FlashlightSpringArm->SetComponentTickEnabled(false);
		FlashlightSpringArm->DestroyComponent();
		FlashlightSpringArm = nullptr;
	}

	Mesh = nullptr;
	Camera = nullptr;
	Movement = nullptr;
}

void UPlayerFlashlightComponent::OnUnregister()
{
	CleanupComponent();
	Super::OnUnregister();
}

void UPlayerFlashlightComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CleanupComponent();
	Super::EndPlay(EndPlayReason);
}

void UPlayerFlashlightConfiguration::ApplyToFlashlightComponent(const UPlayerFlashlightComponent* Component)
{
	USpotLightComponent* Flashlight {Component->GetFlashlight()};
	USpringArmComponent* FlashlightSpringArm {Component->GetFlashlightSpringArm()};
	
	if (Flashlight && FlashlightSpringArm)
	{
		Flashlight->Intensity = Intensity;
		Flashlight->LightColor = LightColor;
		Flashlight->AttenuationRadius = AttenuationRadius;
		Flashlight->InnerConeAngle = InnerConeAngle;
		Flashlight->OuterConeAngle = OuterConeAngle;
		Flashlight->CastShadows = CastsShadows;
		Flashlight->VolumetricScatteringIntensity = VolumetricScatteringIntensity;
		Flashlight->SetUseInverseSquaredFalloff(UseInverseSquaredFalloff);
		Flashlight->LightFalloffExponent = LightFalloffExponent;
		if (LightFunctionMaterial)
		{
			Flashlight->LightFunctionMaterial = LightFunctionMaterial;
		}
		if (IESTexture)
		{
			Flashlight->IESTexture = IESTexture;
		}

		FlashlightSpringArm->CameraRotationLagSpeed = RotationLag;
	}
}






