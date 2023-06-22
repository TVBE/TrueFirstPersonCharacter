// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAudioComponent.generated.h"

class UMetaSoundSource;
class APlayerCharacter;
class UAudioComponent;

/** UPlayerAudioComponent is an Actor Component responsible for managing all audio specific to the player character. 
 *	This class provides a simple and convenient way for designers to customize the player's audio implementation.
 *	@Brief ActorComponent for managing player audio.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "PlayerCharacter", Meta = (BlueprintSpawnableComponent,
DisplayName = "Player Audio Component", ShortToolTip = "Audio component for the player character.") )
class FIRSTPERSONCHARACTER_API UPlayerAudioComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogPlayerAudio, Log, All)

private:
	/** The AudioComponent for general player audio. */
	UPROPERTY(BlueprintGetter = GetBodyAudioComponent)
	UAudioComponent* BodyAudioComponent;

	/** The Metasound Asset for the body audio component. */
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Body Audio Component Metasound Source"))
	TSoftObjectPtr<UMetaSoundSource> BodyAudioComponentSoundAsset; 
	
public:	
	/** Sets default values for this component's properties. */
	UPlayerAudioComponent();

	/** Called every frame. */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void CleanupComponent();

public:
	/** Returns the body AudioComponent. */
	UFUNCTION(BlueprintGetter)
	FORCEINLINE UAudioComponent* GetBodyAudioComponent() const {return BodyAudioComponent; }
	
};
