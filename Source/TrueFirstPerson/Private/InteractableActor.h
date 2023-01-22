// Copyright 2023 Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

/** Base class for actors that can be interacted with by the PlayerCharacter. */
UCLASS(Abstract, Blueprintable)
class AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

	/** Tries to interact with this actor. */
	UFUNCTION(BlueprintCallable, Category = Default, Meta = (DisplayName = "Interact"))
	virtual void Interact(class APlayerCharacter* Interactor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
