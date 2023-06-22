// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "Mouse.h"
#include "MeshGrabComponent.h"

AMouse::AMouse()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	MouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MouseMesh;

	GrabComponent = CreateDefaultSubobject<UMeshGrabComponent>("Interaction Component");
	GrabComponent->SetupAttachment(RootComponent);
}

void AMouse::PostInitProperties()
{
	Super::PostInitProperties();
	if (MouseMesh)
	{
		MouseMesh->BodyInstance.bGenerateWakeEvents = true;
	}
}

void AMouse::BeginPlay()
{
	Super::BeginPlay();
	MouseMesh->OnComponentWake.AddDynamic(this, &AMouse::HandleOnMeshWake);
	MouseMesh->OnComponentSleep.AddDynamic(this, &AMouse::HandleOnMeshSleep);
}

void AMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector Start {MouseMesh->GetComponentLocation()};
	FVector End {Start + FVector(0, 0, -15)};
	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, QueryParams))
	{
		FVector Velocity = MouseMesh->GetComponentVelocity();
		FVector2D MouseVelocity(Velocity.X, Velocity.Y);
		OnMouseMovement.Broadcast(MouseVelocity);
	}
}

void AMouse::HandleOnMeshWake(UPrimitiveComponent* WakingComponent, FName Name)
{
	this->SetActorTickEnabled(true);
}

void AMouse::HandleOnMeshSleep(UPrimitiveComponent* SleepingComponent, FName Name)
{
	this->SetActorTickEnabled(false);
}





