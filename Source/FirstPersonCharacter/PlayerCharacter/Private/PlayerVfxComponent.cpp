// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "PlayerVfxComponent.h"
#include "PlayerCharacter.h"

#include "NiagaraComponent.h"

/** Sets default values for this component's properties. */
UPlayerVfxComponent::UPlayerVfxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPlayerVfxComponent::OnRegister()
{
	Super::OnRegister();
	
	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (!PlayerCharacter) { return ;}
	
	LeftFootEmitter = Cast<UNiagaraComponent>(GetOwner()->AddComponentByClass(UNiagaraComponent::StaticClass(), false, FTransform(), false));
	if (!LeftFootEmitter) { return; }
	LeftFootEmitter->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "foot_l_socket");
	LeftFootEmitter->bAutoActivate = false;
	LeftFootEmitter->bEditableWhenInherited = false;

	RightFootEmitter = Cast<UNiagaraComponent>(GetOwner()->AddComponentByClass(UNiagaraComponent::StaticClass(), false, FTransform(), false));
	if (!RightFootEmitter) { return; }
	RightFootEmitter->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "foot_r_socket");
	RightFootEmitter->bAutoActivate = false;
	RightFootEmitter->bEditableWhenInherited = false;
}



/** Called when the game starts. */
void UPlayerVfxComponent::BeginPlay()
{
	Super::BeginPlay();
	
}




/** Called every frame. */
void UPlayerVfxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UPlayerVfxComponent::CleanupComponent()
{
	if (LeftFootEmitter)
	{
		LeftFootEmitter->Deactivate();
		LeftFootEmitter->DestroyComponent();
		LeftFootEmitter = nullptr;
	}
	if (RightFootEmitter)
	{
		RightFootEmitter->Deactivate();
		RightFootEmitter->DestroyComponent();
		RightFootEmitter = nullptr;
	}
}

void UPlayerVfxComponent::OnUnregister()
{
	CleanupComponent();
	Super::OnUnregister();
}

void UPlayerVfxComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CleanupComponent();
	Super::EndPlay(EndPlayReason);
}



