// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseSoundSourceManager.h"
#include "AmbiverseSoundSource.h"
#include "AmbiverseSubsystem.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseSoundSourceManager, LogAmbiverseSoundSourceManager);

void UAmbiverseSoundSourceManager::InitiateSoundSource(FAmbiverseSoundSourceData& SoundSourceData)
{
	if (!SoundSourceData.Sound)
	{
		UE_LOG(LogAmbiverseSoundSourceManager, Warning, TEXT("InitiateSoundSource: SoundSourceData contains no valid sound."))
		return;
	}
	AAmbiverseSoundSource* SoundSourceInstance {nullptr};
	
	if (Pool.Num() == 0)
	{
		SoundSourceInstance = Owner->GetWorld()->SpawnActor<AAmbiverseSoundSource>(AAmbiverseSoundSource::StaticClass());
		UE_LOG(LogAmbiverseSoundSourceManager, Verbose, TEXT("InitiateSoundSource: Created new SoundSource instance."))
	}
	else
	{
		for (AAmbiverseSoundSource* SoundSource : Pool)
		{
			SoundSourceInstance = SoundSource;
			break;
		}
		
		if (!SoundSourceInstance)
		{
			SoundSourceInstance = Owner->GetWorld()->SpawnActor<AAmbiverseSoundSource>(AAmbiverseSoundSource::StaticClass());
			UE_LOG(LogAmbiverseSoundSourceManager, Verbose, TEXT("InitiateSoundSource: Created new SoundSource instance."))
		}
	}
	if (SoundSourceInstance)
	{
		SoundSourceInstance->Initialize(this, SoundSourceData);
		ActiveSoundSources.AddUnique(SoundSourceInstance);
	}
}

void UAmbiverseSoundSourceManager::ReleaseToPool(AAmbiverseSoundSource* SoundSource)
{
	if (!SoundSource) { return; }

	ActiveSoundSources.Remove(SoundSource);
	
	Pool.AddUnique(SoundSource);
}

#if !UE_BUILD_SHIPPING
void UAmbiverseSoundSourceManager::SetSoundSourceVisualisationEnabled(const bool IsEnabled)
{
	EnableSoundSourceVisualisation = IsEnabled;
	
	if (ActiveSoundSources.IsEmpty()) { return; }

	for (AAmbiverseSoundSource* ActiveSoundSource : ActiveSoundSources)
	{
		ActiveSoundSource->IsDebugVisualisationEnabled = IsEnabled;
		if (IsEnabled)
		{
			ActiveSoundSource->SetActorTickEnabled(true);
		}
	}
}
#endif




