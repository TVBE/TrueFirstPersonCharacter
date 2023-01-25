// Copyright 2023 Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "PlayerCameraConfiguration.generated.h"

/** Struct that defines the player's camera settings. */
USTRUCT(BlueprintType)
struct FPlayerCameraConfiguration
{
	GENERATED_USTRUCT_BODY()

	/** The default camera offset in relation to the Pawn's RootComponent. Use this to set the general camera position of the player in relation to their body. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera, Meta = (DisplayName = "Camera Offset"))
	FVector CameraOffset {FVector(22, 0, 75)};

	/** When enabled, the camera's field of view will scale according to the velocity of the player. This makes higher speeds seem more intense. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = FieldOfView, Meta = (DisplayName = "Enable Dynamic FOV"))
	bool EnableDynamicFOV {true};
	
	/** The default FoV for the camera. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = FieldOfView, Meta = (DisplayName = "Default Field Of View"))
	float DefaultFOV {90.f};

	/** The FoV for the camera when the player is sprinting. Set this higher than the default FoV to get some perceived increase in speed for the player. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = FieldOfView, Meta = (DisplayName = "Sprint Field Of View", EditCondition = "EnabeDynamicFOV"))
	float SprintFOV {96.f};

	/** When enabled, the camera will shake according to the movement op the player. Turn this off to reduce the potential of motion sickness for the user. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = CameraSway, Meta = (DisplayName = "Enable Camera Sway"))
	bool EnableCameraSway {true};

	/** The intensity of camera shake effects when moving. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = CameraSway, Meta = (DisplayName = "Camera Shake Intensity"))
	float CameraShakeIntensity {0.22};

	/** When enabled, the camera will lean into the direction of movement and rotation when the character is moving at high speeds.
	 *	This simulates the character leaning into the direction they're moving to balance out the centrifugal force when taking corners. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = CentripetalRotation, Meta = (DisplayName = "Enable Camera Centripetal Rotation"))
	bool EnableCentripetalRotation {true};

	/** The maximum allowed centripetal rotation. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = CentripetalRotation, Meta = (DisplayName = "Max Centripetal Rotation Angle", EditCondition = "EnableCentripetalRotation", EditConditionHides))
	float MaxCentripetalRotation {14.f};

	/** The intensity of the lean effect when the player is rotating their camera while sprinting. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = CentripetalRotation, Meta = (DisplayName = "Centripetal Rotation Intensity", EditCondition = "EnableCentripetalRotation", EditConditionHides))
	float CentripetalRotationIntensity {2.f};

	/** Multiplier for the centripetal rotation caused by lateral velocity of the player. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = CentripetalRotation, Meta = (DisplayName = "Velocity Based Centripetal Rotation", EditCondition = "EnableCentripetalRotation", EditConditionHides))
	float VelocityCentripetalRotation {1.f};

	/** Multiplier for the centripetal rotation caused by rotation of the player. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = CentripetalRotation, Meta = (DisplayName = "Rotation Based Centripetal Rotation", EditCondition = "EnableCentripetalRotation", EditConditionHides))
	float RotationCentripetalRotation {2.f};	
 
	/** Constructor with default values. */
	FPlayerCameraConfiguration()
	{
	}
 
};

