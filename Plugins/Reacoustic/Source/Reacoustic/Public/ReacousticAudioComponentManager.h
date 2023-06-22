// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Nino Saglia.

#pragma once

#include "CoreMinimal.h"
#include "ReacousticSubsystemComponent.h"
#include "ReacousticAudioComponentManager.generated.h"

UCLASS()
class UReacousticAudioComponentManager : public UReacousticSubsystemComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogReacousticAudioComponentManager, Log, All)

private:
	/** Pool of AudioComponent instances.
	 *  We pre populate the pool at initialization, and keep the pool size fixed during runtime.
	 *  This means that the array can contain with nullptrs, always check each element before using. */
	UPROPERTY(Transient)
	TArray<UAudioComponent*> AvailableAudioComponents;

	/** Array of AudioComponent instances that are currently playing. */
	UPROPERTY(Transient)
	TArray<UAudioComponent*> ActiveAudioComponents;

protected:
	virtual void Initialize(UReacousticSubsystem* Subsystem) override;

public:
	FORCEINLINE TArray<UAudioComponent*> GetActiveAudioComponents() const { return ActiveAudioComponents; }
};