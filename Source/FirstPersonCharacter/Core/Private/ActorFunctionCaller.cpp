// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "ActorFunctionCaller.h"

#include "LogCategories.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UObject/UObjectGlobals.h"

/** Calls the specified function on a target actor if the soft object pointer is valid and the function exists on the actor. */
void FActorFunctionCaller::CallFunction()
{
	if (Target.IsValid())
	{
		if (AActor* ValidActor {Target.Get()})
		{
			if (UFunction* Function {ValidActor->FindFunction(FunctionName)})
			{
				ValidActor->ProcessEvent(Function, nullptr);
			}
			else
			{
				UE_LOG(LogActorFunctionCaller, Warning, TEXT("Actor '%s' does not have function: '%s'."), *ValidActor->GetName(), *FunctionName.ToString());
			}
		}
		else
		{
			UE_LOG(LogActorFunctionCaller, Warning, TEXT("Failed to get a valid actor: '%s'."), *Target.ToString());
		}
	}
	else
	{
		UE_LOG(LogActorFunctionCaller, Warning, TEXT("Actor is not loaded or valid: '%s'."), *Target.ToString());
	}
}