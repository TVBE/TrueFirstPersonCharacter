// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReacousticSlidingAudioComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UReacousticSlidingAudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReacousticSlidingAudioComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
