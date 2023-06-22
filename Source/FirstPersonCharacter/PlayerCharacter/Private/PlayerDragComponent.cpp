// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Nino Saglia & Tim Verberne.

#include "PlayerDragComponent.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "PlayerInteractionComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"

DEFINE_LOG_CATEGORY_CLASS(UPlayerDragComponent, LogDragComponent)

void UPlayerDragComponent::OnRegister()
{
    Super::OnRegister();

    if (const APlayerCharacter* PlayerCharacter{ Cast<APlayerCharacter>(GetOwner()) })
    {
        Camera = PlayerCharacter->GetCamera();
        Movement = PlayerCharacter->GetPlayerCharacterMovement();
    }
    else
    {
        UE_LOG(LogDragComponent, Warning, TEXT("Player character is not valid."));
    }
	
    if (const UWorld* World = GetWorld())
    {
        Gravity = World->GetGravityZ();
    }

	if(!Configuration)
	{
		UE_LOG(LogDragComponent, Warning, TEXT("Created Default configuration since it's not set."));
		Configuration = NewObject<UPlayerDragConfiguration>();
	}
	ApplyToPhysicsHandle();
}

void UPlayerDragComponent::BeginPlay()
{
    Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UPlayerDragComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (GrabbedComponent)
    {
		// UpdateLocalConstraint();
    	
        if (!Configuration)
        {
        	return;
        }
    	
    	UpdateTargetLocation(DeltaTime);
    	// UpdateCameraRotationSpeed(DeltaTime);
        if (Configuration->LetGoDistance <= FVector::Distance(GrabbedComponent->GetComponentLocation(), TargetLocation))
        {
            ReleaseActor();
        }
    }
}

void UPlayerDragComponent::UpdateCameraRotationSpeed(float DeltaTime)
{
	if(GrabbedComponent)
	{
		CameraRotationMultiplier = 1 / (FVector::Distance(GrabbedComponent->GetComponentLocation(), TargetLocation)
			* Configuration->CameraRotationDecreasingStrength * 0.1);
	}
	else
	{
		CameraRotationMultiplier = 1.0;
	}
}

void UPlayerDragComponent::UpdateLocalConstraint()
{
	if (InteractionComponent)
	{
		const FHitResult HitResult {InteractionComponent->GetCameraTraceHitResult()};
		if (HitResult.IsValidBlockingHit() && HitResult.GetComponent() == GrabbedComponent)
		{
			if (FVector::DistSquared(HitResult.ImpactPoint, GetDragLocation()) >= 1225)
			{
				/** TODO: Find method to prevent physics jump when re-grabbing object. */
				InterpolationSpeed = 0.0f;
				GrabComponentAtLocation(HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint);
				InterpolationSpeed = 50.0f;
			}
		}
	}
}


void UPlayerDragComponent::DragActorAtLocation(AActor* ActorToGrab, const FVector& Location)
{
	if (!ActorToGrab){UE_LOG(LogDragComponent, Warning, TEXT("Actor to grab is null"));return;}
	if (GrabbedComponent){UE_LOG(LogDragComponent, Warning, TEXT("Already dragging a component"));return;}

	UStaticMeshComponent* StaticMeshComponent {Cast<UStaticMeshComponent>(ActorToGrab->GetComponentByClass(UStaticMeshComponent::StaticClass()))};
	if (!StaticMeshComponent){UE_LOG(LogDragComponent, Warning, TEXT("Actor to grab does not have a static mesh component"));return;}
	

	TargetLocationZ = Location.Z;
	GrabComponentAtLocation(StaticMeshComponent, NAME_None, Location);
	CurrentZoomLevel = FVector::Distance(Camera->GetComponentLocation(), Location);
	
	GrabOffset = StaticMeshComponent->GetCenterOfMass() - (Camera->GetComponentLocation() + CurrentZoomLevel * Camera->GetForwardVector());

	// UE_LOG(LogTemp, Display, TEXT("GrabOffset %f %f %f"),GrabOffset.X,GrabOffset.Y,GrabOffset.Z);
	
	SetComponentTickEnabled(true);
	//GrabbedComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	const FBox BoundingBox {GrabbedComponent->Bounds.GetBox()};
	DraggedComponentSize = FVector::Distance(BoundingBox.Min, BoundingBox.Max) / 2;
}

void UPlayerDragComponent::ReleaseActor()
{
	if (!GrabbedComponent){ return; }

	//GrabbedComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	SetComponentTickEnabled(false);
	
	ReleaseComponent();
	UE_LOG(LogDragComponent, VeryVerbose, TEXT("Released Object."));
}

/** The looping function that updates the target location and rotation of the currently dragged object*/
void UPlayerDragComponent::UpdateTargetLocation(float DeltaTime)
{
	if (!GrabbedComponent || !Camera || !Configuration) { UE_LOG(LogDragComponent, Warning, TEXT("UpdateTargetForceWithLocation called with null pointers"));return; }
	AActor* CompOwner = this->GetOwner();

	if (CompOwner)
	{
		{
			CurrentZoomLevel = CurrentZoomLevel + CurrentZoomAxisValue * Configuration->ZoomSpeed * DeltaTime;
		}

		/** Clamp the zoom level within the min max of configuration*/
		CurrentZoomLevel = FMath::Clamp(CurrentZoomLevel, Configuration->MinZoomLevel, Configuration->MaxZoomLevel);
	}
	
	
	FVector TargetLocationPre = Camera->GetComponentLocation() + CurrentZoomLevel * Camera->GetForwardVector() ;
	TargetLocation = FVector(TargetLocationPre.X,TargetLocationPre.Y, TargetLocationZ);
	SetTargetLocation(TargetLocation );
}

FVector UPlayerDragComponent::GetDragLocation() const
{
	if (!GrabbedComponent) { return FVector(); }
	
	const FTransform ComponentTransform {GrabbedComponent->GetComponentTransform()};
	
	return ComponentTransform.TransformPosition(ConstraintLocalPosition);
}


/** The update loop that scales the zoomaxis value from the mouse input */
void UPlayerDragComponent::UpdateZoomAxisValue(float ZoomAxis)
{
	if (!Configuration){UE_LOG(LogDragComponent, Warning, TEXT("UpdateZoomAxisValue called with null Configuration"));return;}
	CurrentZoomAxisValue = FMath::Clamp(((CurrentZoomAxisValue + 0.1 * ZoomAxis) * 0.9), -2.0, 2.0);
}

void UPlayerDragComponent::ApplyToPhysicsHandle()
{
	// Set the member variables of this PhysicsHandleComponent to the values in this data asset.
	SetLinearDamping(Configuration->LinearDamping);
	SetLinearStiffness(Configuration->LinearStiffness);
	SetAngularDamping(Configuration->AngularDamping);
	SetAngularStiffness(Configuration->AngularStiffness);
	SetInterpolationSpeed(Configuration->InterpolationSpeed);
}


