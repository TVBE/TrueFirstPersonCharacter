// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Tim Verberne.

#include "ReacousticSubsystemComponent.h"

void UReacousticSubsystemComponent::Initialize(UReacousticSubsystem* Subsystem)
{
	if (!Subsystem) { return; }
	Owner = Subsystem;
	IsInitialized = true;
}

void UReacousticSubsystemComponent::Deinitialize(UReacousticSubsystem* Subsystem)
{
	if (!Subsystem) { return; }
	Owner = nullptr;
	IsInitialized = false;
}