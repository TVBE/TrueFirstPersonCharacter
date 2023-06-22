// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseLayer.h"
#include "AmbiverseSubsystemComponent.h"
#include "AmbiverseLayerManager.generated.h"

class UAmbiverseComposite;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLayerRegisteredDelegate, UAmbiverseLayer*, RegisteredLayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLayerUnregisteredDelegate, UAmbiverseLayer*, UnregisteredLayer);

UCLASS()
class UAmbiverseLayerManager : public UAmbiverseSubsystemComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseLayerManager, Log, All)

public:
	UPROPERTY()
	FOnLayerRegisteredDelegate OnLayerRegistered;

	UPROPERTY()
	FOnLayerRegisteredDelegate OnLayerUnregistered;

private:
	/** The current active ambience layers. */
	UPROPERTY()
	TArray<UAmbiverseLayer*> ActiveLayers;

public:
	virtual void Initialize(UAmbiverseSubsystem* Subsystem) override;
	virtual void Deinitialize(UAmbiverseSubsystem* Subsystem) override;

	virtual void Tick(const float DeltaTime) override;
	
	void RegisterAmbiverseLayer(UAmbiverseLayer* Layer);
	void UnregisterAmbiverseLayer(UAmbiverseLayer* Layer);

	void InitializeLayer(UAmbiverseLayer* Layer, const uint16 WarmUpCount = 3);

	void RegisterAmbiverseComposite(UAmbiverseComposite* Composite);
	void UnregisterAmbiverseComposite(UAmbiverseComposite* Composite);
	
	/** Checks if an ambience layer is already active*/
	UAmbiverseLayer* FindActiveAmbienceLayer(const UAmbiverseLayer* LayerToFind) const;

private:
	UFUNCTION()
	void HandleOnParameterChanged(UAmbiverseParameter* ChangedParameter);

	void UpdateActiveLayers(float DeltaTime);

	void UpdateElements(float DeltaTime, UAmbiverseLayer* Layer);

public:
	FORCEINLINE TArray<UAmbiverseLayer*> GetLayerRegistry() const { return ActiveLayers; }
	
};
