// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TriggerableObjectInterface.generated.h"

UENUM(BlueprintType)
enum class ETriggerableObjectAction : uint8
{
	Trigger			UMETA(DisplayName = "Trigger"),
	Untrigger		UMETA(DisplayName = "Untrigger"),
};

UINTERFACE(Blueprintable, Meta = (DisplayName = "Triggerable Object Interface",
	ShortToolTip = "Interface for objects that can be triggered."))
class UTriggerableObject : public UInterface
{
	GENERATED_BODY()
};

class ITriggerableObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Triggerable Object")
	bool Trigger(const AActor* Initiator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Triggerable Object")
	bool Untrigger(const AActor* Initiator);
};

