// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "ActorFunctionCaller.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FirstPersonCharacterFunctionLibrary.generated.h"

class APlayerCharacter;
UENUM(BlueprintType, Meta = (DisplayName = "Function Result"))
enum class EFunctionResult : uint8
{
	Successful					UMETA(DisplayName = "Successful"),
	Unsuccessful				UMETA(DisplayName = "Unsuccessful"),
};

/** The interaction type of an object. This enumeration is currently only used in the the FindObjectThatImplementsInterface function.*/
UENUM(BlueprintType, Meta = (DisplayName = "FirstPersonCharacter Interface Type"))
enum class EFirstPersonCharacterInterfaceType : uint8
{
	InteractableObject			UMETA(DisplayName = "Interactable Object Interface"),
	UsableObject				UMETA(DisplayName = "Usable Object Interface"),
	GrabbableObject				UMETA(DisplayName = "Grabbable Object Interface"),
	DraggableObject				UMETA(DisplayName = "Draggable Object Interface"),
	InventoryObject				UMETA(DisplayName = "Inventory Object Interface"),
	PowerConsumer				UMETA(DisplayName = "Power Consumer Interface")
};

UCLASS()
class FIRSTPERSONCHARACTER_API UFirstPersonCharacterFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Tries to find the first object that implements the specified interface. This object could be the actor itself, or any other UObject derived class.
	 *	@Param Successful Whether the function successfully found the object.
	 *	@Param Actor The actor to search in.
	 *	@Param Interface The interface to search for.
	 *	@Return The object that implements the interface, if any. This can be null, so perform a IsValid check.
	 */
	UFUNCTION(BlueprintCallable, Category = "Interface", Meta = (DisplayName = "Find Object That Implements Interface By Type", ExpandEnumAsExecs = "Result"))
	static UObject* SearchActorForObjectThatImplementsInterface(EFunctionResult& Result, AActor* Actor, EFirstPersonCharacterInterfaceType Interface);

	/** Tries to execute the function of a ActorFunctionCaller struct.
	 *	@Param FunctionCaller The ActorFunctionCaller struct to use.
	 */
	UFUNCTION(BlueprintCallable, Category = "Actor Function Caller", Meta = (DisplayName = "Call Function On Actor"))
	static void CallFunctionOnActorUsingActorFunctionCaller(FActorFunctionCaller FunctionCaller);

	/** Returns the player character instance if it is already spawned in the world.
	 *	This function performs no casts and can safely be called every frame without performance repercussions. */
	UFUNCTION(BlueprintPure, Meta = (DisplayName = "Get FirstPersonCharacter Player Character",
		CompactNodeTitle = "Player Character", Keywords = "Get, FirstPersonCharacter, Player, Character",
		WorldContext = "WorldContextObject"))
	static APlayerCharacter* GetFirstPersonCharacterPlayerCharacter(const UObject* WorldContextObject);

	/** Returns the player character controller instance if it is already spawned in the world.
	 *	This function performs no casts and can safely be called every frame without performance repercussions. */
	UFUNCTION(BlueprintPure, Meta = (DisplayName = "Get FirstPersonCharacter Player Controller", CompactNodeTitle = "Player Controller",
		Keywords = "Get, FirstPersonCharacter, Player, Character, Controller",
		WorldContext = "WorldContextObject"))
	static APlayerCharacterController* GetFirstPersonCharacterPlayerCharacterController(const UObject* WorldContextObject);
};


