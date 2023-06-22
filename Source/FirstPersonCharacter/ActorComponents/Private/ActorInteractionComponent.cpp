// Copyright 2023 Barrelhouse


#include "ActorInteractionComponent.h"

// Sets default values for this component's properties
UActorInteractionComponent::UActorInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

