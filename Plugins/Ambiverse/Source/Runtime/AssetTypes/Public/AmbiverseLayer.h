// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseParameter.h"
#include "AmbiverseProceduralElement.h"
#include "TimerManager.h"
#include "AmbiverseLayer.generated.h"

class UAmbiverseSubsystem;
class UAmbiverseParameter;
class UAmbiverseSoundSourceManager;

UCLASS(Blueprintable, BlueprintType, ClassGroup = "Ambiverse", Meta = (DisplayName = "Ambiverse Layer",
	ShortToolTip = "A list of Ambiverse Elements that can be procedurally played."))
class AMBIVERSE_API UAmbiverseLayer : public UObject
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseLayer, Log, All)

public:
#if WITH_EDITORONLY_DATA
	/** User friendly name for the layer. */
	UPROPERTY(EditAnywhere, Category = "Editor")
	FName Name;

	/** A description for the layer. */
	UPROPERTY(EditAnywhere, Category = "Editor", Meta = (MultiLine = "true"))
	FText Description;
	
	/** The color of the layer when visualised in the editor. */
	UPROPERTY(EditAnywhere, Category = "Editor")
	FColor Color {FColor::Blue};
#endif
	
	/** the procedural sound data of this layer. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Elements", Meta = (DisplayName = "Elements", TitleProperty = "Element"))
	TArray<FAmbiverseProceduralElement> ProceduralElements;

	/** Parameters that influence all procedural sounds in this layer. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters", Meta = (TitleProperty = "Parameter"))
	TArray<FAmbiverseParameterModifiers> Parameters;

	/** Volume multiplier for all sounds in this layer. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (DisplayName = "Volume", ClampMin = "0", ClampMax = "2", UIMin = "1", UIMax ="2"))
	float LayerVolume {1.0f};

	/** Rate multiplier for all sounds in this layer. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (DisplayName = "Density", ClampMin = "0", ClampMax = "2", UIMin = "1", UIMax ="2"))
	float LayerDensity {1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (DisplayName = "Enable Layer"))
	bool IsEnabled {true};

	/** If true, the layer has a finite lifetime and will expire when this duration is reached after becoming active. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lifetime")
	bool EnableLifetime {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lifetime", Meta = (EditCondition = "EnableLifetime", ClampMin = "0"))
	float Lifetime {30.0f};

	float ActiveDuration {0.0f};

	/** @brief The normalized ratio of the active duration of the layer relative to its maximum lifetime.
	* This property provides a normalized value indicating how much of the layer's total lifetime has been used up.
	* It is calculated as the active duration divided by the total lifetime. 
	* For instance, if the active duration is 20 seconds and the total lifetime is 40 seconds, LifetimePercentage will be 0.5. */
	float LifetimeRatio {0.0f};
	
	UPROPERTY()
	FTimerHandle TimerHandle;
	
	FTimerDelegate TimerDelegate;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};




