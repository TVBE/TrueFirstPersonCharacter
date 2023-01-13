// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LandingIntensity.generated.h"

/** Enumeration for landing intensity. */
UENUM(BlueprintType)
enum LandingIntensity
{
	SOFT				UMETA(DisplayName = "Soft"),
	HARD				UMETA(DisplayName = "Hard"),
	HEAVY				UMETA(DisplayName = "Heavy"),
};
