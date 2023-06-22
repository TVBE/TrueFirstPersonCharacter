// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Nino Saglia & Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerVocalComponent.generated.h"

/** Actor component that handles all vocal audio for the player character. */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "PlayerCharacter", Meta = (BlueprintSpawnableComponent,
	DisplayName = "Player Vocal Component", ShortToolTip = "Component that handles vocal audio for the player character."))
class FIRSTPERSONCHARACTER_API UPlayerCharacterVocalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerCharacterVocalComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
