// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseSoundSourceData.h"
#include "AmbiverseSubsystemComponent.h"
#include "AmbiverseSoundSourceManager.generated.h"

class AAmbiverseSoundSource;

UCLASS()
class UAmbiverseSoundSourceManager : public UAmbiverseSubsystemComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseSoundSourceManager, Log, All)

private:
	/** Pool of SoundSource instances. These can include subobjects. */
	UPROPERTY(Transient)
	TArray<AAmbiverseSoundSource*> Pool;

	/** Array of sound sources that are currently playing. */
	UPROPERTY(Transient)
	TArray<AAmbiverseSoundSource*> ActiveSoundSources;

#if !UE_BUILD_SHIPPING
	bool EnableSoundSourceVisualisation {false};
#endif

public:
	void InitiateSoundSource(FAmbiverseSoundSourceData& SoundSourceData);

	UFUNCTION(BlueprintCallable)
	void ReleaseToPool(AAmbiverseSoundSource* SoundSource);

#if !UE_BUILD_SHIPPING
	void SetSoundSourceVisualisationEnabled(const bool IsEnabled);
#endif

	FORCEINLINE TArray<AAmbiverseSoundSource*> GetActiveSoundSources() const { return ActiveSoundSources; }
};
