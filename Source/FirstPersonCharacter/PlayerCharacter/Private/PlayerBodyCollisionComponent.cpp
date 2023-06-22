// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "PlayerBodyCollisionComponent.h"

UPlayerBodyCollisionComponent::UPlayerBodyCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPlayerBodyCollisionComponent::OnRegister()
{
	Super::OnRegister();

	SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
}

void UPlayerBodyCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerBodyCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


