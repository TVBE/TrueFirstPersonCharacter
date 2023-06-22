// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "ActorFunctionCaller.generated.h"

USTRUCT(BlueprintType)
struct FActorFunctionCaller
{
	GENERATED_BODY()

	/** The target actor to call the function on. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Function Caller", Meta = (DisplayName = "Target"))
	TSoftObjectPtr<AActor> Target;

	/** The function to call on the actor.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Function Caller", Meta = (DisplayName = "Function"))
	FName FunctionName;

	void CallFunction();
};