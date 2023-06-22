// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "GrabbableObjectInterface.h"
#include "GameFramework/Actor.h"
#include "Keyboard.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "Desktop", Meta = (DisplayName = "Keyboard"))
class FIRSTPERSONCHARACTER_API AKeyboard : public AActor, public IGrabbableObject
{
	GENERATED_BODY()

public:	
	AKeyboard();

public:
	/** IInteractableObject interface functions. */
	FORCEINLINE bool BeginGrab_Implementation(const AActor* Interactor) override { return false; }
	FORCEINLINE bool EndGrab_Implementation(const AActor* Interactor) override { return false; }
	FORCEINLINE EGrabType GetGrabType_Implementation() const override { return EGrabType::OneHanded; }

protected:
	virtual void BeginPlay() override;

	/** Returns a random character. */
	UFUNCTION(BlueprintPure)
	FString GetRandomCharacter();
};
