// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FirstPersonCharacterGameMode.generated.h"

class APlayerCharacter;

/**
 * 
 */
UCLASS()
class AFirstPersonCharacterGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	bool IsPlayerActive {false};

public:
	AFirstPersonCharacterGameMode();
	
	/** Notifies the gamemode that a player character is fully initialized and is ready for use. */
	void NotifyPlayerCharacterBeginPlay(APlayerCharacter* Character);

protected:
	/** Called when the player character is ready for use in the world. */
	UFUNCTION(BlueprintNativeEvent, Category = "Events", Meta = (DisplayName = "On Player Spawn"))
	void EventOnPlayerSpawn(APlayerCharacter* Character);
};
