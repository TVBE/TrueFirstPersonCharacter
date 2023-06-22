// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObjectInterface.generated.h"

/** Interface for grabbable objects. This interface can be implemented in any UObject derived class. */
UINTERFACE(Blueprintable, Meta = (DisplayName = "Interactable Object Interface", ShortToolTip = "Interface for interactable objects."))
class FIRSTPERSONCHARACTER_API UInteractableObject : public UInterface
{
	GENERATED_BODY()
};

class FIRSTPERSONCHARACTER_API IInteractableObject
{
	GENERATED_BODY()

public:
	/** Returns the interaction widget location in world space. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable Object")
	FVector GetInteractionWidgetLocation() const;
};