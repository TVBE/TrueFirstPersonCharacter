// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseBPLibrary.h"
#include "Ambiverse.h"
#include "AmbiverseLayerManager.h"
#include "AmbiverseParameterManager.h"
#include "AmbiverseSubsystem.h"

UAmbiverseBPLibrary::UAmbiverseBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UAmbiverseBPLibrary::AddAmbiverseLayer(UObject* WorldContextObject, UAmbiverseLayer* AmbiverseLayer)
{
	if (!WorldContextObject) { return; }

	if (UWorld* World {WorldContextObject->GetWorld()})
	{
		if (UAmbiverseSubsystem* AmbiverseSubsystem { World->GetSubsystem<UAmbiverseSubsystem>()})
		{
			if (UAmbiverseLayerManager* LayerManager {AmbiverseSubsystem->GetLayerManager()})
			{
				LayerManager->RegisterAmbiverseLayer(AmbiverseLayer);
			}
		}
	}
}

void UAmbiverseBPLibrary::PopAmbiverseLayer(UObject* WorldContextObject, UAmbiverseLayer* AmbiverseLayer)
{
	if (!WorldContextObject) { return; }

	if (UWorld* World {WorldContextObject->GetWorld()})
	{
		if (UAmbiverseSubsystem* AmbiverseSubsystem { World->GetSubsystem<UAmbiverseSubsystem>()})
		{
			if (UAmbiverseLayerManager* LayerManager {AmbiverseSubsystem->GetLayerManager()})
			{
				LayerManager->UnregisterAmbiverseLayer(AmbiverseLayer);
			}
		}
	}
}

void UAmbiverseBPLibrary::SetAmbiverseParameter(UObject* WorldContextObject, UAmbiverseParameter* AmbiverseParameter, const float Value)
{
	if (!WorldContextObject) { return; }

	if (UWorld* World {WorldContextObject->GetWorld()})
	{
		if (UAmbiverseSubsystem* AmbiverseSubsystem {World->GetSubsystem<UAmbiverseSubsystem>()})
		{
			if (UAmbiverseParameterManager* ParameterManager {AmbiverseSubsystem->GetParameterManager()})
			{
				ParameterManager->SetParameterValue(AmbiverseParameter, Value);
			}
		}
	}
}
