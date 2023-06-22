// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseSubsystem.h"
#include "AmbiverseDistributor.h"
#include "AmbiverseDistributorManager.h"
#include "AmbiverseElement.h"
#include "AmbiverseLayer.h"
#include "AmbiverseLayerManager.h"
#include "AmbiverseParameterManager.h"
#include "AmbiverseSoundSourceData.h"
#include "AmbiverseSoundSourceManager.h"
#include "AmbiverseVisualisationComponent.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseSubsystem, LogAmbiverseSubsystem);

UAmbiverseSubsystem::UAmbiverseSubsystem()
{
}

void UAmbiverseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LayerManager = NewObject<UAmbiverseLayerManager>(this);
	if (LayerManager) { LayerManager->Initialize(this); }
	
	ParameterManager = NewObject<UAmbiverseParameterManager>(this);
	if (ParameterManager) { ParameterManager->Initialize(this); }

	SoundSourceManager = NewObject<UAmbiverseSoundSourceManager>(this);
	if (SoundSourceManager) { SoundSourceManager->Initialize(this); }

	DistributorManager = NewObject<UAmbiverseDistributorManager>(this);
	if (DistributorManager) { DistributorManager->Initialize(this); }

#if !UE_BUILD_SHIPPING
	VisualisationComponent.Reset(NewObject<UAmbiverseVisualisationComponent>(this));
#endif
}

void UAmbiverseSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	UE_LOG(LogAmbiverseSubsystem, Log, TEXT("Adaptive Ambience System initialized successfully."))
}

void UAmbiverseSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LayerManager && LayerManager->IsInitialized)
	{
		LayerManager->Tick(DeltaTime);
	}
}

void UAmbiverseSubsystem::ProcessProceduralElement(UAmbiverseLayer* Layer, FAmbiverseProceduralElement& ProceduralElement)
{
	if (!Layer)
	{
		UE_LOG(LogAmbiverseSubsystem, Error, TEXT("ProcessProceduralElement: Layer is nullptr."));
		return;
	}
	
	ProceduralElement.ReferenceTime = FMath::RandRange(ProceduralElement.IntervalRange.X,
	                                                   ProceduralElement.IntervalRange.Y);

	float DensityModifier {1.0f};
	float VolumeModifier {1.0f};

	if (!ParameterManager)
	{
		UE_LOG(LogAmbiverseSubsystem, Error, TEXT("ProcessProceduralElement: ParameterManager is nullptr."));
		return;
	}
	
	ParameterManager->GetScalarsForElement(DensityModifier, VolumeModifier, Layer, ProceduralElement);

	ProceduralElement.Time = ProceduralElement.ReferenceTime * DensityModifier;

	/** We try to get the location of the listener here.*/
	FVector CameraLocation;
	bool IsCameraValid {false};

	if (APlayerController* PlayerController {GetWorld()->GetFirstPlayerController()})
	{
		if (const APlayerCameraManager* CameraManager{PlayerController->PlayerCameraManager})
		{
			CameraLocation = CameraManager->GetCameraLocation();
			IsCameraValid = true;
		}
	}

	if (!IsCameraValid)
	{
		UE_LOG(LogAmbiverseSubsystem, Error, TEXT("ProcessProceduralElement: Unable to obtain valid camera location."));
		return;
	}

	/** Prepare the sound source data. */
	FAmbiverseSoundSourceData SoundSourceData{FAmbiverseSoundSourceData()};

	SoundSourceData.Sound = UAmbiverseElement::GetSoundFromMap(ProceduralElement.Element->Sounds);
	SoundSourceData.Volume = ProceduralElement.Element->Volume;
	SoundSourceData.Name = FName(ProceduralElement.Element->GetName());
	SoundSourceData.Layer = Layer;

	if (const TSubclassOf<UAmbiverseDistributor> DistributorClass{ProceduralElement.Element->DistributorClass})
	{
		if (!DistributorManager)
		{
			UE_LOG(LogAmbiverseSubsystem, Error, TEXT("ProcessProceduralElement: DistributorManager is nullptr."));
			return;
		}

		if (UAmbiverseDistributor* Distributor{DistributorManager->GetDistributorByClass(DistributorClass)})
		{
			FTransform Transform{};
			if (Distributor->ExecuteDistribution(this, Transform, CameraLocation, ProceduralElement.Element))
			{
				SoundSourceData.Transform = Transform;
			}
		}
	}
	else
	{
		SoundSourceData.Transform = FAmbiverseSoundDistributionData::GetSoundTransform(
			ProceduralElement.Element->DistributionData, CameraLocation);
	}
	
	if (!SoundSourceManager)
	{
		UE_LOG(LogAmbiverseSubsystem, Error, TEXT("ProcessProceduralElement: SoundSourceManager is nullptr."));
		return;
	}

	SoundSourceManager->InitiateSoundSource(SoundSourceData);
}

void UAmbiverseSubsystem::SetNewTimeForProceduralElement(FAmbiverseProceduralElement& ProceduralElement, UAmbiverseLayer* Layer)
{
	ProceduralElement.ReferenceTime = FMath::RandRange(ProceduralElement.IntervalRange.X,
													   ProceduralElement.IntervalRange.Y);

	float DensityModifier {1.0f};
	float VolumeModifier {1.0f};

	if (!ParameterManager)
	{
		UE_LOG(LogAmbiverseSubsystem, Error, TEXT("SetNewTimeForProceduralElement: ParameterManager is nullptr."));
		return;
	}
	
	ParameterManager->GetScalarsForElement(DensityModifier, VolumeModifier, Layer, ProceduralElement);

	ProceduralElement.Time = ProceduralElement.ReferenceTime * DensityModifier;
}

float UAmbiverseSubsystem::GetSoundVolume(const UAmbiverseLayer* Layer, const FAmbiverseProceduralElement& ProceduralElement)
{
	if (!Layer) { return -1.0f; }
	float Volume {ProceduralElement.Element->Volume};
	Volume *= Layer->LayerVolume;
	return Volume;
}

void UAmbiverseSubsystem::HandleParameterChanged()
{
	if (!LayerManager || LayerManager->GetLayerRegistry().IsEmpty()) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Parameter Changed!"))

	TArray<UAmbiverseLayer*> Layers {LayerManager->GetLayerRegistry()};
	for (UAmbiverseLayer* Layer : Layers)
	{
		for (FAmbiverseProceduralElement Entry : Layer->ProceduralElements)
		{
			float DensityScalar {1.0f};
			float VolumeScalar {1.0f};
			
			ParameterManager->GetScalarsForElement(DensityScalar, VolumeScalar, Layer, Entry);

			Entry.Time = Entry.ReferenceTime * DensityScalar;
		}
	}
}

void UAmbiverseSubsystem::Deinitialize()
{
	if (LayerManager)
	{
		LayerManager->Deinitialize(this);
		LayerManager = nullptr;
	}
	if (ParameterManager)
	{
		ParameterManager->Deinitialize(this);
		SoundSourceManager = nullptr;
	}
	if (SoundSourceManager)
	{
		SoundSourceManager->Deinitialize(this);
		SoundSourceManager = nullptr;
	}
	if (DistributorManager)
	{
		DistributorManager->Deinitialize(this);
		DistributorManager = nullptr;
	}

#if !UE_BUILD_SHIPPING
	if (VisualisationComponent.IsValid())
	{
		VisualisationComponent->ConditionalBeginDestroy();
		VisualisationComponent.Reset();
	}
#endif
	
	Super::Deinitialize();
}


