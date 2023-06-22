// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseSubsystemComponent.h"
#include "AmbiverseVisualisationComponent.generated.h"

UCLASS()
class UAmbiverseVisualisationComponent : public UAmbiverseSubsystemComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseVisualisation, Log, All)

public:
	TUniquePtr<FAutoConsoleCommand> SoundSourceVisualisationConsoleCommand;

	bool IsDebugEnabled {true};

public:
	virtual void Initialize(UAmbiverseSubsystem* Subsystem) override;
	
	virtual void Tick(const float DeltaTime) override;
	
	void SetSoundSourceVisualisationEnabled(bool IsEnabled);
};
