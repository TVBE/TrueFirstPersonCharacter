// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Nino Saglia & Tim Verberne.


#include "PlayerVocalComponent.h"

// Sets default values for this component's properties
UPlayerCharacterVocalComponent::UPlayerCharacterVocalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerCharacterVocalComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerCharacterVocalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

