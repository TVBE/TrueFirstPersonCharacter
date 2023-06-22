// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseSoundSourceData.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "AmbiverseSoundSource.generated.h"

class UAmbiverseLayer;
class UAmbiverseSoundSourceManager;

UCLASS(Blueprintable, BlueprintType, NotPlaceable, ClassGroup = "Ambiverse")
class AMBIVERSE_API AAmbiverseSoundSource : public AActor
{
	GENERATED_BODY()
	
	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseSoundSource, Log, All)
	
public:
#if !UE_BUILD_SHIPPING
	bool IsDebugVisualisationEnabled {false};
	float ActiveTime {0.0f};
#endif

private:
	/** The audio component to play back sounds at. */
	UPROPERTY(BlueprintGetter = GetAudioComponent)
	UAudioComponent* AudioComponent;

	/** Pointer to the sound source manager. */
	UPROPERTY(Transient)
	UAmbiverseSoundSourceManager* SoundSourceManager;

	UPROPERTY(BlueprintGetter = GetSoundSourceData)
	FAmbiverseSoundSourceData SoundSourceData;

	/** The name of the sound source. */
	UPROPERTY()
	FName SoundSourceName;

	/** The layer the sound source belongs to. */
	UPROPERTY()
	UAmbiverseLayer* AmbiverseLayer {nullptr};

public:	
	AAmbiverseSoundSource();

	void Initialize(UAmbiverseSoundSourceManager* Manager, FAmbiverseSoundSourceData& Data);

	virtual void Tick(float DeltaTime) override;

	void SetSound(USoundBase* NewSound)
	{
		if (!AudioComponent || !NewSound) { return; }
		AudioComponent->SetSound(NewSound);
	}

	void SetVolume (const float NewVolume)
	{
		if (!AudioComponent) { return; }
		AudioComponent->SetVolumeMultiplier(NewVolume);
	}
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintGetter)
	FORCEINLINE UAudioComponent* GetAudioComponent() const { return AudioComponent; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE FAmbiverseSoundSourceData GetSoundSourceData() const { return SoundSourceData; }

private:
	UFUNCTION()
	void HandleOnAudioFinishedPlaying();
};
