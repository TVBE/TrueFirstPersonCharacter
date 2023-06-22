// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Nino Saglia & Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"

class UPlayerGrabConfiguration;
class UPlayerDragConfiguration;
class UPlayerDragComponent;
class UPlayerDragConfiguration;
class UPlayerUseComponent;
class UPlayerGrabComponent;
class UCameraComponent;
struct FCollisionQueryParams;

/** The interaction type. */
UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	Usable					UMETA(DisplayName = "Usable"),
	Grabbable				UMETA(DisplayName = "Grabbable"),
	Draggable				UMETA(DisplayName = "Draggable")
};

/** The interaction action type. */
UENUM(BlueprintType)
enum class EInteractionActionType : uint8
{
	Primary					UMETA(DisplayName = "Primary", ToolTip = "The primary action is used to 'use' actors."),
	Secondary				UMETA(DisplayName = "Secondary", ToolTip = "The secondary action is used to grab actors."),
};

USTRUCT(BlueprintType)
struct FInteractableObjectData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	UObject* Object {nullptr};

	UPROPERTY(BlueprintReadOnly)
	EInteractionType InteractionType {EInteractionType::Grabbable};

	FInteractableObjectData()
	{
	}

	FInteractableObjectData(UObject* InObject, EInteractionType InInteractionType)
		: Object(InObject)
		, InteractionType(InInteractionType)
	{
	}
};


/** PlayerCharacter component that checks for interactable objects in front of the player character. */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "PlayerCharacter",
	Meta = (DisplayName = "Player Interaction Component", ShortToolTip = "Component that handles player interaction."))
class FIRSTPERSONCHARACTER_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** The length of the initial line trace. */
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInteraction", Meta = (DisplayName = "Camera Trace Length", ClampMax = "500", UIMax = "500"))
	uint16 CameraTraceLength {300};

	/** The radius of the interactable object multi sphere trace. */
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInteraction", Meta = (DisplayName = "Object Trace Radius", ClampMax = "500", UIMax = "500"))
	uint16 ObjectTraceRadius {50};

private:
	/** The use component that is used to use actors. */
	UPROPERTY(BlueprintGetter = GetUseComponent)
	UPlayerUseComponent* UseComponent;
	
	/** The physics grab component that is used to grab actors. */
	UPROPERTY(BlueprintGetter = GetGrabComponent)
	UPlayerGrabComponent* GrabComponent;

	/** The physics drag component that is used to drag actors. */
	UPROPERTY(BlueprintGetter = GetDragComponent)
	UPlayerDragComponent* DragComponent;
	
	/** The camera component of the Player Character. */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerInteraction|Components", Meta = (DisplayName = "Camera", AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	/** If true, the interaction component is currently performing a tertiary interaction. */
	UPROPERTY(BlueprintGetter = GetIsTertiaryInteractionActive)
	bool IsTertiaryInteractionActive {false};
	
	/** The location of the camera. */
	UPROPERTY()
	FVector CameraLocation;
	
	/** The hit result for the initial visibility line trace collision query performed from the camera. */
	UPROPERTY()
	FHitResult CameraTraceHitResult {FHitResult()};

	/** The hit result for the occlusion line trace collision query performed from the camera. */
	UPROPERTY()
	FHitResult OcclusionTraceHitResult {FHitResult()};

	/** The offset used for the occlusion trace. Prevents the occlusion trace hitting the actor underneath
	 *	the interactable object that the occlusion trace is performed for. */
	UPROPERTY()
	FVector OcclusionOffset {FVector(0, 0, 5)};

	/** The array of hit results for the interactable object multi sphere trace. */
	UPROPERTY()
	TArray<FHitResult> ObjectTraceHitResults;

	/** The actor that currently can be interacted with. Will be a nullptr if no object can be interacted with at the moment. */
	UPROPERTY(BlueprintGetter = GetCurrentInteractableActor)
	AActor* CurrentInteractableActor;

	/** The interactable objects for the current interactable actor. */
	UPROPERTY(BlueprintGetter = GetCurrentInteractableObjects)
	TArray<FInteractableObjectData> CurrentInteractableObjects;

	/** The closest interactable object to the camera. */
	UPROPERTY()
	FInteractableObjectData ClosestInteractableObject;
	
	/** The actor that is currently being interacted with. */
	UPROPERTY(BlueprintReadOnly, Category = "PlayerInteraction", Meta = (DisplayName = "Current Interacting Actor", AllowPrivateAccess = "true"))
	AActor* CurrentInteractingActor;
	
	/** The collision query data for the camera trace. */
	FCollisionQueryParams CameraTraceQueryParams;
	
	/** The timer for the update function. */
	UPROPERTY()
	FTimerHandle UpdateTimerHandle;
	
	/** For setting the parameters of the PlayerGrabComponent. */
	UPROPERTY(EditAnywhere, Category = "PlayerInteractionComponent", Meta = (DisplayName = "Settings for player grab compoent"))
	TSoftObjectPtr<UPlayerGrabConfiguration> PlayerGrabConfiguration;

	/** For setting the parameters of the PlayerDragComponent. */
	UPROPERTY(EditAnywhere, Category = "PlayerInteractionComponent", Meta = (DisplayName = "Settings for player drag compoent"))
	TSoftObjectPtr<UPlayerDragConfiguration> PlayerDragConfiguration;


#if WITH_EDITORONLY_DATA
	/** When true, we will draw debug visualisation to the screen for every collision query. */
	UPROPERTY(EditAnywhere, Category = "PlayerInteractionComponent|Debugging", Meta = (DisplayName = "Enable Debug Visualisation"))
	bool IsDebugVisEnabled {false};
#endif

public:	
	UPlayerInteractionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Begins the primary interaction action. */
	UFUNCTION(BlueprintCallable, Category = "PlayerInteration|Actions", Meta = (DisplayName = "Begin Primary Interaction", BlueprintProtected))
	void BeginPrimaryInteraction();
	
	/** Ends the primary interaction action. */
	UFUNCTION(BlueprintCallable, Category = "PlayerInteraction|Actions", Meta = (DisplayName = "End Primary Interaction", BlueprintProtected))
	void EndPrimaryInteraction();
	
	/** Begins the secondary interaction action. */
	UFUNCTION(BlueprintCallable, Category = "PlayerInteraction|Actions", Meta = (DisplayName = "Begin Secondary Interaction", BlueprintProtected))
	void BeginSecondaryInteraction();

	/** Ends the secondary interaction action. */
	UFUNCTION(BlueprintCallable, Category = "PlayerInteraction|Actions", Meta = (DisplayName = "End Secondary Interaction", BlueprintProtected))
	void EndSecondaryInteraction();

	/** Begins the tertiary interaction action. */
	UFUNCTION(BlueprintCallable, Category = "PlayerInteraction|Actions", Meta = (DisplayName = "Begin Tertiary Interaction", BlueprintProtected))
	void BeginTertiaryInteraction();

	/** Ends the tertiary interaction action. */
	UFUNCTION(BlueprintCallable, Category = "PlayerInteraction|Actions", Meta = (DisplayName = "End Tertiary Interaction", BlueprintProtected))
	void EndTertiaryInteraction();
	
	/** Handles scroll input. */
	UFUNCTION(BlueprintCallable, Category = "PlayerInteraction|Actions", Meta = (DisplayName = "Add Scroll Input", BlueprintProtected))
	void AddScrollInput(const float Input);

	/** Adds pitch input to the interaction component. */
	UFUNCTION()
	void AddPitchInput(const float Input);

	/** Adds yaw input to the interaction component. */
	UFUNCTION()
	void AddYawInput(const float Input);

protected:
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void BeginPlay() override;
	 
	/** Checks if any interactable objects are in front of the player. */
	UFUNCTION(BlueprintCallable, Category = "PlayerInteractionComponent", Meta = (DisplayName = "Check For Interactable Objects", BlueprintProtected))
	AActor* CheckForInteractableActor();

	UFUNCTION(BlueprintNativeEvent, Category = "PlayerInteraction", Meta = (DisplayName = "Begin Interaction"))
	void EventBeginInteraction(const EInteractionActionType Type, const UObject* Object);
    
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerInteraction", Meta = (DisplayName = "End Interaction"))
	void EventEndInteraction(const EInteractionActionType Type, const UObject* Object);

private:
	/** Performs a line trace from the camera. */
	UFUNCTION()
	void PerformTraceFromCamera(FHitResult& HitResult);

	/** Performs a multi sphere trace for objects that respond to the interactable trace channel. */
	UFUNCTION()
	void PerformInteractableObjectTrace(TArray<FHitResult>& Array, const FHitResult& HitResult);

	/** Returns the closest object to the specified hit hit result from an array of hit results. */
	UFUNCTION()
	static AActor* GetClosestActor(const TArray<FHitResult>& Array, const FHitResult& HitResult);

	/** Checks if an actor or one of its components implements the IInteractableObject interface.
	 *	Returns the first UObject that implements the interface that it finds. */
	template <typename TInterface>
	UObject* FindInteractableObject(AActor* Actor) const;

	/** Tries to find a component that implements the IInteractableObject interface in a specified actor.*/
	template <typename TInterface>
	UActorComponent* FindInteractableComponent(const AActor* Actor) const;

	/** Checks if an actor or one of its components implements the IInteractableObject interface.
	*	Returns all UObjects that implements the interface that it finds. */
	template <typename TInterface>
	TArray<UObject*> FindInteractableObjects(AActor* Actor) const;
	
	template <class TInterface>
	void AddInteractableObjectsOfType(AActor* Actor, EInteractionType InteractionType);

	/** Updates the current interactable actor data. */
	UFUNCTION()
	void UpdateInteractableObjectData(AActor* NewInteractableActor);

	/** Checks whether an actor is occluded. Is used to prevent the interaction component from highlighting objects behind walls or other objects. */
	UFUNCTION()
	bool IsActorOccluded(const AActor* Actor);
	
	/** Converts a UObject pointer to an AActor pointer.
	 *	Either by checking if the UObject is an AActor,
	 *	or by casting the UObject to an UActorComponent and retrieving the owner from the component. */
	UFUNCTION()
	AActor* GetActorFromObject(UObject* Object) const;

	/** Returns the closest USceneComponent to the player's look-at location. */
	bool GetClosestObjectToLocation(FInteractableObjectData& OutInteractableObjectData, const FVector& Location, TArray<FInteractableObjectData> Objects);

public:
	/** Returns a pointer to the use component. */
	UFUNCTION(BlueprintGetter)
	FORCEINLINE UPlayerUseComponent* GetUseComponent() const { return UseComponent; }

	/** Returns a pointer to the grab component. */
	UFUNCTION(BlueprintGetter)
	FORCEINLINE UPlayerGrabComponent* GetGrabComponent() const { return GrabComponent; }

	/** Returns a pointer to the drag component. */
	UFUNCTION(BlueprintGetter)
	FORCEINLINE UPlayerDragComponent* GetDragComponent() const { return DragComponent; }
	
	/** Returns the object the interaction component is currently interacting with. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetCurrentInteractingActor() const { return CurrentInteractingActor; }

	/** Returns whether the interaction component is currently performing a tertiary interaction or not. */
	UFUNCTION(BlueprintGetter)
	FORCEINLINE bool GetIsTertiaryInteractionActive() const { return IsTertiaryInteractionActive; }

	/** Returns whether there is an object in front of the player that can be interacted with. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool CanInteract() const { return static_cast<bool>(CurrentInteractableActor); }

	/** Returns a pointer to an object that is currently available for interaction. Returns a nullptr if no interactable object is in front of the player. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetCurrentInteractableActor() const { return CurrentInteractableActor; }

	/** Returns data about all interactable objects in an actor. This could be the actor itself as well. */
	UFUNCTION(BlueprintGetter)
	FORCEINLINE TArray<FInteractableObjectData> GetCurrentInteractableObjects() const { return CurrentInteractableObjects; }

	/** Returns data about the the closest interactable object */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetClosestInteractableObject(FInteractableObjectData& InteractableObjectData)
	{
		if (!CurrentInteractableActor) { return false; }

		InteractableObjectData = ClosestInteractableObject;
		return true;
	}

	/** Returns the most recent camera trace result. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FHitResult GetCameraTraceHitResult() const { return CameraTraceHitResult; }
};

