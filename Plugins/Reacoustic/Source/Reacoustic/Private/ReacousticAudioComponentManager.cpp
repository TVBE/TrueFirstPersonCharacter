// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Tim Verberne.

#include "ReacousticAudioComponentManager.h"
#include "ReacousticSubsystem.h"
#include "Components/AudioComponent.h"

DEFINE_LOG_CATEGORY_CLASS(UReacousticAudioComponentManager, LogReacousticAudioComponentManager);

void UReacousticAudioComponentManager::Initialize(UReacousticSubsystem* Subsystem)
{
	Super::Initialize(Subsystem);
	
	constexpr int32 AudioComponentAmount {32};
	
	for (int32 i {0}; i < AudioComponentAmount; ++i)
	{
		if (UAudioComponent* NewAudioComponent {NewObject<UAudioComponent>(Owner)})
		{
			AvailableAudioComponents.Add(NewAudioComponent);
		}
	}

	if (AvailableAudioComponents.Num() != AudioComponentAmount)
	{
		UE_LOG(LogReacousticAudioComponentManager, Warning, TEXT("Failed to populate AudioComponent pool. Expected: '%d', Created: '%d'"), AudioComponentAmount, AvailableAudioComponents.Num());
	}
	else
	{
		UE_LOG(LogReacousticAudioComponentManager, Log, TEXT("Successfully initialized ReacousticAudioComponentManager"));
	}
}
