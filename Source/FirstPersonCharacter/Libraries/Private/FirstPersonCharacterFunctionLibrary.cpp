// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "FirstPersonCharacterFunctionLibrary.h"
#include "DraggableObjectInterface.h"
#include "FirstPersonCharacterWorldSubystem.h"
#include "GrabbableObjectInterface.h"
#include "InteractableObjectInterface.h"
#include "UsableObjectInterface.h"

template <typename TInterface>
UActorComponent* FindInteractableComponent(const AActor* Actor)
{
	if (!Actor) { return nullptr; }
	
	TSet<UActorComponent*> Components {Actor->GetComponents()};
	
	if (Components.IsEmpty()) { return nullptr; }
	
	for (UActorComponent* Component : Components)
	{
		if (Component->GetClass()->ImplementsInterface(TInterface::StaticClass()))
		{
			return Component;
		}
	}
	return nullptr;
}

template <typename TInterface>
UObject* FindInteractableObject(AActor* Actor)
{
	if (!Actor) { return nullptr; }
	UObject* InteractableObject {nullptr};
	
	/** Check if the actor implements the specified interface. */
	if (Actor->GetClass()->ImplementsInterface(TInterface::StaticClass()))
	{
		InteractableObject = Actor;
	}
	
	/** If the actor does not implement the specified interface, try to find a component that does.*/
	else if (UActorComponent* InteractableComponent {FindInteractableComponent<TInterface>(Actor)})
	{
		InteractableObject = InteractableComponent;
	}
	
	return InteractableObject;
}

UObject* UFirstPersonCharacterFunctionLibrary::SearchActorForObjectThatImplementsInterface(EFunctionResult& Result,
	AActor* Actor, EFirstPersonCharacterInterfaceType Interface)
{
	if (!Actor) { return nullptr; }

	UObject* InterfaceObject {nullptr};

	switch(Interface)
	{
	case EFirstPersonCharacterInterfaceType::InteractableObject: InterfaceObject = FindInteractableObject<UInteractableObject>(Actor);
		break;
	case EFirstPersonCharacterInterfaceType::UsableObject: InterfaceObject = FindInteractableObject<UUsableObject>(Actor);
		break;
	case EFirstPersonCharacterInterfaceType::GrabbableObject: InterfaceObject = FindInteractableObject<UGrabbableObject>(Actor);
		break;
	case EFirstPersonCharacterInterfaceType::DraggableObject: InterfaceObject = FindInteractableObject<UDraggableObject>(Actor);
		break;
	default: break;
	}

	if (InterfaceObject)
	{
		Result = EFunctionResult::Successful;
	}
	else
	{
		Result = EFunctionResult::Unsuccessful;
	}

	return InterfaceObject;
}

void UFirstPersonCharacterFunctionLibrary::CallFunctionOnActorUsingActorFunctionCaller(FActorFunctionCaller FunctionCaller)
{
	FunctionCaller.CallFunction();
}

APlayerCharacter* UFirstPersonCharacterFunctionLibrary::GetFirstPersonCharacterPlayerCharacter(const UObject* WorldContextObject)
{
	if (!WorldContextObject) { return nullptr; }
	if (const UWorld* World {WorldContextObject->GetWorld()})
	{
		if (const UFirstPersonCharacterWorldSubsystem* Subsystem {World->GetSubsystem<UFirstPersonCharacterWorldSubsystem>()})
		{
			return Subsystem->GetPlayerCharacter();
		}
	}
	return nullptr;
}

APlayerCharacterController* UFirstPersonCharacterFunctionLibrary::GetFirstPersonCharacterPlayerCharacterController(const UObject* WorldContextObject)
{
	if (!WorldContextObject) { return nullptr; }
	if (const UWorld* World {WorldContextObject->GetWorld()})
	{
		if (const UFirstPersonCharacterWorldSubsystem* Subsystem {World->GetSubsystem<UFirstPersonCharacterWorldSubsystem>()})
		{
			return Subsystem->GetPlayerController();
		}
	}
	return nullptr;
}

