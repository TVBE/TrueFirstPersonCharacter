// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Nino Saglia.

#include "ReacousticSubsystem.h"
#include "ReacousticComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "ReacousticDataTypes.h"

DEFINE_LOG_CATEGORY_CLASS(UReacousticSubsystem, LogReacousticSubsystem);

UReacousticSubsystem::UReacousticSubsystem()
{
	Settings = GetMutableDefault<UReacousticProjectSettings>();

	//TODO: get all the settings from the project settngs and use them to populate wold with reacoustic objects without any functions in the level blueprint.
}

void UReacousticSubsystem::PostInitProperties()
{
	
	if(!ReacousticSoundDataAsset || !ReacousticSoundDataRefMap)
	{
		
	}
	
	Super::PostInitProperties();
	
	UWorld* World = GetWorld();
	if (World)
	{
		World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &UReacousticSubsystem::OnActorSpawned));
	}
}


void UReacousticSubsystem::OnActorSpawned(AActor* Actor)
{
	
}

void UReacousticSubsystem::RegisterComponent(UReacousticComponent* Component)
{
	ReacousticComponents.AddUnique(Component);
}

void UReacousticSubsystem::UnregisterComponent(UReacousticComponent* Component)
{
	ReacousticComponents.Remove(Component);
}

bool UReacousticSubsystem::IsReacousticCompatible(AActor* Actor)
{
	if (Actor->GetRootComponent() && Actor->GetRootComponent()->IsSimulatingPhysics())
	{
		TArray<UStaticMeshComponent*> Components;
		Actor->GetComponents<UStaticMeshComponent>(Components);
		if(!Components.IsEmpty())
		{
			for(UStaticMeshComponent* Component : Components)
			{
				if(Component->BodyInstance.bNotifyRigidBodyCollision)
				{
					return true;
				}
			}
		}
	}
	return false;
}

/* Adds a reacousticComponent derived component to an actor. and returns the pointer to this component.*/
void UReacousticSubsystem::AddBPReacousticComponentToActor(AActor* Actor, TSubclassOf<UReacousticComponent> ComponentClass, FReacousticSoundData MeshSoundData)
{
	if (!ReacousticSoundDataRefMap || !ReacousticSoundDataAsset)
	{
		return;
	}
	
    UActorComponent* NewComponent {nullptr};
    if (!Actor)
    {
        UE_LOG(LogReacousticSubsystem, Warning, TEXT("AddComponentToActor was called without passing an actor pointer."))
        return;
    }
    if (Actor->GetComponentByClass(ComponentClass))
    {
        const FString ActorName{Actor->GetActorNameOrLabel()};
        UE_LOG(LogReacousticSubsystem, Log, TEXT("Skipped adding a component to %s because this actor already has a component of the same class attached to it."), *ActorName);
    }
    else
    {
        NewComponent = Actor->AddComponentByClass(ComponentClass, false, FTransform(), true);
    }
    if(NewComponent)
    {
        if(UReacousticComponent* ReacousticComponent {Cast<UReacousticComponent>(NewComponent)})
        {
            ReacousticComponent->TransferData(ReacousticSoundDataAsset, ReacousticSoundDataRefMap, MeshSoundData);
            ReacousticComponent->RegisterComponent();
        }
    }
}

TArray<AActor*> UReacousticSubsystem::GetCompatibleActorsOfClass(UClass* ClassType)
{

    TArray<AActor*> Array{};
    const UWorld* World{GetWorld()};
    if (!World || !ClassType)
    {
        UE_LOG(LogReacousticSubsystem, Log, TEXT("Error world or class is invalid"));
        return Array;
    }
    TArray<AActor*> FoundActors{};
    UGameplayStatics::GetAllActorsOfClass(World, ClassType, FoundActors);
    const int TotalCount{FoundActors.Num()};
    int CompatibleCount{0};
    for (AActor* Actor : FoundActors)
    {
        if(IsReacousticCompatible(Actor))
        {
            Array.Add(Actor);
            CompatibleCount++;
        }
    }
    const FString Name{ClassType->GetName()};
    UE_LOG(LogReacousticSubsystem, Log, TEXT("Found %d actors of type %s out of which %d can be used by the Reacoustic system."), TotalCount, *Name, CompatibleCount);
    return Array;
}


/** For current use: adds a blueprint component derrived from the reacoustics c++ component. */
void UReacousticSubsystem::PopulateWorldWithBPReacousticComponents(TSubclassOf<class UReacousticComponent> ComponentClass)
{
	if (!ComponentClass)
	{
		UE_LOG(LogReacousticSubsystem, Warning, TEXT("ComponentClass is nullptr."));
		return;
	}

	if (!ReacousticSoundDataRefMap || !ReacousticSoundDataAsset)
	{
		return;
	}

	TArray<AActor*> CompatibleActors {GetCompatibleActorsOfClass(AStaticMeshActor::StaticClass())};
	for (AActor* Actor : CompatibleActors)
	{
		if (!Actor)
		{
			continue;
		}

		TArray<UActorComponent*> Components{};
		Actor->GetComponents(Components);
		for (UActorComponent* ActorComponent : Components)
		{
			if (!ActorComponent)
			{
				continue;
			}

			if (const UStaticMeshComponent* StaticMeshComponent {Cast<UStaticMeshComponent>(ActorComponent)})
			{
				const FReacousticSoundData MeshSoundData {GetMeshSoundData(StaticMeshComponent)};
				AddBPReacousticComponentToActor(Actor, ComponentClass, MeshSoundData);
			}
		}
	}
}

FReacousticSoundData UReacousticSubsystem::GetMeshSoundData(const UStaticMeshComponent* StaticMeshComponent) const
{
	if (!StaticMeshComponent || !ReacousticSoundDataRefMap || !ReacousticSoundDataAsset)
	{
		return FReacousticSoundData{};
	}

	for (const auto& [Mesh, SoundDataRef] : ReacousticSoundDataRefMap->MeshMapEntries)
	{
		UStaticMesh* MeshPtr {Mesh};
		const int32 SoundDataRefValue {SoundDataRef};

		if (StaticMeshComponent->GetStaticMesh() == MeshPtr)
		{
			return ReacousticSoundDataAsset->AudioData[SoundDataRefValue];
		}
	}
	return FReacousticSoundData{};
}