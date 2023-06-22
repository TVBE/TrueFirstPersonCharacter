// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Nino Saglia.

#pragma once

#include "CoreMinimal.h"
#include "ReacousticDataTypes.h"
#include "Engine/DeveloperSettings.h"
#include "UObject/SoftObjectPath.h"
#include "ReacousticSettings.generated.h"

UCLASS(config = Reacoustic, defaultconfig, Meta = (DisplayName = "Reacoustic"))
class REACOUSTIC_API UReacousticProjectSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()
private:

	UPROPERTY()
	bool CanGenerateNRTAssets{false};
	
	UPROPERTY()
	bool CanGenerateRuntimeData{false};

	
public:
	UPROPERTY(Config, EditAnywhere, Meta = (AllowedClasses = DataTable))
	FSoftObjectPath ReacousticObjects;

	UPROPERTY(Config, EditAnywhere, Meta = (AllowedClasses = DataTable))
	FSoftObjectPath ReacousticSurfaces;

	UPROPERTY(Config, EditAnywhere, Meta = (AllowedClasses = UReacousticComponent))
	FSoftObjectPath ReacousticComponent;

protected:
	/** The GENERATED data used by the reacoustic subsystem.#1#*/
	UReacousticSoundDataAsset* ReacousticSoundDataAsset;
	UReacousticSoundDataRef_Map* ReacousticSoundDataRefMap;


public:
	/** Calls the GenerateRuntimeData function in the subsystem.*/
	void GenerateRuntimeData();





#if WITH_EDITOR
	virtual FName GetCategoryName() const override { return FName(TEXT("Game")); }
	virtual FText GetSectionText() const override { return NSLOCTEXT("ReacousticPlugin", "ReacousticSettingsSection", "Reacoustic"); };

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
