// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseVisualisationComponent.h"
#include "AmbiverseSoundSourceManager.h"
#include "AmbiverseSubsystem.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseVisualisationComponent, LogAmbiverseVisualisation);

void UAmbiverseVisualisationComponent::Initialize(UAmbiverseSubsystem* Subsystem)
{
	Super::Initialize(Subsystem);

	SoundSourceVisualisationConsoleCommand = MakeUnique<FAutoConsoleCommand>(
		TEXT("av.EnableSoundSourceVisualisation"),
		TEXT("Enable or disable sound source visualisation."),
		FConsoleCommandWithArgsDelegate::CreateLambda([this](const TArray<FString>& Args)
		{
			if (Args.Num() > 0)
			{
				SetSoundSourceVisualisationEnabled(FCString::Atoi(*Args[0]) != 0);
			}
		})
	);
}

void UAmbiverseVisualisationComponent::Tick(const float DeltaTime)
{
	if (!IsDebugEnabled || !Owner) { return; }

	if (UAmbiverseSoundSourceManager* SoundSourceManager {Owner->GetSoundSourceManager()})
	{
		// TArray<AAmbiverseSoundSource*> ActiveSoundSources {SoundSourceManager->GetAct}
	}
}


void UAmbiverseVisualisationComponent::SetSoundSourceVisualisationEnabled(bool IsEnabled)
{
	
}
