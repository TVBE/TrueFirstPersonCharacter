// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "FirstPersonCharacterWorldSubystem.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"

DEFINE_LOG_CATEGORY_CLASS(UFirstPersonCharacterWorldSubsystem, LogFirstPersonCharacterWorldSubsystem);

void UFirstPersonCharacterWorldSubsystem::RegisterPlayerCharacter(APlayerCharacter* Character)
{
	if (Character)
	{
		if (PlayerCharacter)
		{
			UE_LOG(LogFirstPersonCharacterWorldSubsystem, Warning, TEXT("Tried to register a player character while a player character is already registered to the subsystem."));
			return;
		}
		PlayerCharacter = Character;
	}
	else
	{
		UE_LOG(LogFirstPersonCharacterWorldSubsystem, Error, TEXT("Could not register player character."));
	}
}

void UFirstPersonCharacterWorldSubsystem::RegisterPlayerController(APlayerCharacterController* Controller)
{
	if (Controller)
	{
		if (PlayerController)
		{
			UE_LOG(LogFirstPersonCharacterWorldSubsystem, Warning, TEXT("Tried to register a player controller while a player character is already registered to the subsystem."));
			return;
		}
		PlayerController = Controller;
	}
	else
	{
		UE_LOG(LogFirstPersonCharacterWorldSubsystem, Error, TEXT("Could not register player controller."));
	}
}

void UFirstPersonCharacterWorldSubsystem::UnregisterPlayerCharacter(APlayerCharacter* Character)
{
	if (Character)
	{
		if (PlayerCharacter == Character)
		{
			PlayerCharacter = nullptr;
		}
	}
}

void UFirstPersonCharacterWorldSubsystem::UnregisterPlayerController(APlayerCharacterController* Controller)
{
	if (Controller)
	{
		if (PlayerController == Controller)
		{
			PlayerController = nullptr;
		}
	}
}
