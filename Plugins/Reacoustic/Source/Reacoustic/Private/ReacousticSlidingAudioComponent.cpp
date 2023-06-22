// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Tim Verberne.

#include "ReacousticSlidingAudioComponent.h"


UReacousticSlidingAudioComponent::UReacousticSlidingAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
}


// Called when the game starts
void UReacousticSlidingAudioComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UReacousticSlidingAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

