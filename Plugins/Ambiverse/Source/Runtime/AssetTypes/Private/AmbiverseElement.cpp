// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseElement.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseElement, LogAmbiverseElement);

UMetaSoundSource* UAmbiverseElement::GetSoundFromMap(const TMap<UMetaSoundSource*, int>& SoundMap)
{
	if (SoundMap.Num() == 0)
	{
		UE_LOG(LogAmbiverseElement, Error, TEXT("GetSoundFromMap: SoundMap is empty."));
		return nullptr;
	}

	int TotalWeight {0};
	for (const auto& SoundWeightPair : SoundMap)
	{
		TotalWeight += SoundWeightPair.Value;
	}

	if (TotalWeight <= 0)
	{
		UE_LOG(LogAmbiverseElement, Error, TEXT("GetSoundFromMap: Total weight is less than or equal to zero."));
		return nullptr;
	}

	int RandomWeight {FMath::RandRange(0, TotalWeight - 1)};
	int AccumulatedWeight = 0;

	for (const auto& SoundWeightPair : SoundMap)
	{
		AccumulatedWeight += SoundWeightPair.Value;
		if (AccumulatedWeight > RandomWeight)
		{
			return SoundWeightPair.Key;
		}
	}

	/** We should not reach this point due to the way the weights are accumulated and checked.
	 * If we do, there might be some problem with the weights in the map. */
	UE_LOG(LogAmbiverseElement, Error, TEXT("GetSoundFromMap: Unable to select a sound from the map. Check the weight configuration."));
	return nullptr;
}

#if WITH_EDITOR
void UAmbiverseElement::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAmbiverseElement, Sounds))
	{
		for (auto& Entry : Sounds)
		{
			if (Entry.Value < 1)
			{
				Entry.Value = 1;
			}
		}
	}
}
#endif

