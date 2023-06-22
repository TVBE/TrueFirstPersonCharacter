// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "StepData.generated.h"

class UPhysicalMaterial;

/** Enum for defining which foot performed a footstep. */
UENUM(BlueprintType)
enum class ELeftRight : uint8
{
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right"),
};

/** Struct containing data about a footstep. */
USTRUCT(BlueprintType)
struct FIRSTPERSONCHARACTER_API FStepData
{
	GENERATED_USTRUCT_BODY()
	
	/** Location of the step in world space. */
	UPROPERTY(BlueprintReadWrite, Category = "StepData", Meta = (DisplayName = "Location"))
	FVector Location;

	/** The velocity of the character when the step was made. */
	UPROPERTY(BlueprintReadWrite, Category = "StepData", Meta = (DisplayName = "Velocity"))
	float Velocity;

	/** The PhysicalMaterial underneath the foot. */
	UPROPERTY(BlueprintReadWrite, Category = "StepData", Meta = (DisplayName = "Physical Material"))
	UPhysicalMaterial* PhysicalMaterial;
	
	/** The object underneath the foot. */
	UPROPERTY(BlueprintReadWrite, Category = "StepData", Meta = (DisplayName = "Object Underneath Foot"))
	UObject* Object;
	
	/** Constructor with default values. */
	FStepData()
	{
		Location = FVector();
		Velocity = 0.0f;
		PhysicalMaterial = nullptr;
		Object = nullptr;
	}

	/** Constructor. */
	FStepData(ELeftRight Foot, FVector Location, float Velocity, UPhysicalMaterial* PhysicalMaterial, UObject* Object);
};
