// Copyright 2023 Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "PlayerCameraConfiguration.generated.h"

/** Struct that defines the player's camera settings. */
USTRUCT(BlueprintType)
struct FPlayerCameraConfiguration
{
	GENERATED_USTRUCT_BODY()

	/** The default camera offset in relation to the character. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera, Meta = (DisplayName = "Camera Offset"))
	FVector CameraOffset {FVector(22.0, 0, 75)};
	
	/** The default FoV for the camera. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera, Meta = (DisplayName = "Default Field Of View"))
	float DefaultFOV {90.f};

	/** The FoV for the camera when the player is sprinting. Set this higher than the default FoV to get some perceived increase in speed for the player. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera, Meta = (DisplayName = "Sprint Field Of View"))
	float SprintFOV {96.f};
 
	/** Constructor with default values. */
	FPlayerCameraConfiguration()
	{
	}
 
};

