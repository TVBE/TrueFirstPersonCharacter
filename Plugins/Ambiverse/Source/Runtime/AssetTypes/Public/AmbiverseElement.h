// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseSoundDistributionData.h"
#include "MetasoundSource.h"
#include "AmbiverseSoundSource.h"
#include "AmbiverseElement.generated.h"

class UAmbiverseDistributor;

/** An ambiverse element is a single procedural sound. It can be played directly, or used in a layer to create a procedural soundscape. */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "Ambiverse", Meta = (DisplayName = "Ambiverse Element",
	ShortToolTip = "A single sound element that can be used inside an Ambiverse Layer"))
class AMBIVERSE_API UAmbiverseElement : public UObject
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseElement, Log, All)

public:
	/** The MetaSoundSource types and their corresponding weights.
	 *  If the TMap is empty or the weights are set improperly, an even distribution of selection for the sounds will be used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	TMap<UMetaSoundSource*, int> Sounds;

	/** The volume multiplier for an AmbienceSystem preset entry */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Data",
		Meta = (ClampMin = "0"))
	float Volume {1.0f};
	
	/** The distributor to use for this element. */
	UPROPERTY(EditAnywhere, Category = "Distribution", Meta = (BlueprintBaseOnly))
	TSubclassOf<UAmbiverseDistributor> DistributorClass;
	
	/** PlayRange data for an AmbienceSystem preset entry. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distribution", Meta = (EditCondition = "DistributorClass == nullptr"))
	FAmbiverseSoundDistributionData DistributionData;

	/** The SoundSource class to use to for this element. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Source")
	TSubclassOf<AAmbiverseSoundSource> SoundSourceClass {AAmbiverseSoundSource::StaticClass()};
	
	bool IsValid {true};
	
	static UMetaSoundSource* GetSoundFromMap(const TMap<UMetaSoundSource*, int>& SoundMap);

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif
};
