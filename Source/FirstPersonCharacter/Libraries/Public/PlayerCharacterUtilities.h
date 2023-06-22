// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayerCharacterUtilities.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONCHARACTER_API UPlayerCharacterUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Returns data about a footstep for the specified actor, like the object or physical material underneath the foot at the time of the footstep.
	*	@Return FootstepData structure containing relevant information about the location and velocity of the foot at the time of the footstep. 
	*/
	UFUNCTION(BlueprintPure, Category = "Character", Meta = (DisplayName = "Get Footstep Data Underneath Actor", WorldContext = "WorldContextObject"))
	static FStepData GetStepData(const UObject* WorldContextObject, const AActor* Actor, const float TraceLength);
};
