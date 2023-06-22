// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseParameterManager.h"
#include "AmbiverseLayerManager.h"
#include "AmbiverseParameter.h"
#include "AmbiverseSubsystem.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseParameterManager, LogAmbiverseParameterManager);

void UAmbiverseParameterManager::Initialize(UAmbiverseSubsystem* Subsystem)
{
	Super::Initialize(Subsystem);
	
	if (UAmbiverseLayerManager* LayerManager {Subsystem->GetLayerManager()})
	{
		LayerManager->OnLayerRegistered.AddDynamic(this, &UAmbiverseParameterManager::HandleOnLayerRegistered);
	}
}

void UAmbiverseParameterManager::GetScalarsForElement(float& DensityScalar, float& VolumeScalar, const UAmbiverseLayer* Layer, const FAmbiverseProceduralElement& ProceduralElement)
{
	DensityScalar = 1.0f;
	VolumeScalar = 1.0f;
	
	if (!Layer) { return; }
	
	TArray<FAmbiverseParameterModifiers> Parameters;
	
	Parameters.Append(Layer->Parameters);
	// Parameters.Append(ProceduralElement.Element.Parameters); // For now, we're not implementing parameters inside elements.
	
	if (Parameters.IsEmpty()) { return; }

	for (const FAmbiverseParameterModifiers& Modifier : Parameters)
	{
		UAmbiverseParameter* RegisteredParameter {nullptr};
		
		for (UAmbiverseParameter* Parameter : ParameterRegistry)
		{
			if (Parameter == Modifier.Parameter)
			{
				RegisteredParameter = Parameter;
				break;
			}
		}

		if (!RegisteredParameter)
		{
			RegisterParameter(Modifier.Parameter);
			continue;
		}

		DensityScalar *= FMath::GetMappedRangeValueClamped(FVector2D(0, 1), Modifier.DensityRange, RegisteredParameter->ParameterValue);
		VolumeScalar *= FMath::GetMappedRangeValueClamped(FVector2D(0, 1), Modifier.VolumeRange, RegisteredParameter->ParameterValue);
	}

	DensityScalar *= Layer->LayerDensity;
	DensityScalar = 1 / DensityScalar;
	VolumeScalar *= Layer->LayerVolume;
}

void UAmbiverseParameterManager::SetParameterValue(UAmbiverseParameter* Parameter, const float Value)
{
	if (ParameterRegistry.IsEmpty())
	{
		RegisterParameter(Parameter);
	}

	for (UAmbiverseParameter* AmbiverseParameter : ParameterRegistry)
	{
		if (AmbiverseParameter == Parameter)
		{
			AmbiverseParameter->SetParameter(Value);
			break;
		}
	}

	OnParameterChangedDelegate.Broadcast(Parameter);
}

void UAmbiverseParameterManager::RegisterParameter(UAmbiverseParameter* Parameter)
{
	if (!Parameter) { return; }
	if (!IsParameterRegistered(Parameter))
	{
		ParameterRegistry.Add(Parameter);
		UE_LOG(LogAmbiverseParameterManager, Verbose, TEXT("Registered parameter: '%s'"), *Parameter->GetName())
	}
}

bool UAmbiverseParameterManager::IsParameterRegistered(const UAmbiverseParameter* Parameter) const
{
	if (!Parameter) { return false; }
	if (ParameterRegistry.Contains(Parameter))
	{
		return true;
	}
	return false;
}

void UAmbiverseParameterManager::HandleOnLayerRegistered(UAmbiverseLayer* RegisteredLayer)
{
	if (!RegisteredLayer) { return; }

	UE_LOG(LogAmbiverseParameterManager, Verbose, TEXT("Updating parameter registry for: '%s'"), *RegisteredLayer->GetName());

	TArray<UAmbiverseParameter*> RequiredParameters;

	for (FAmbiverseParameterModifiers& ParameterModifiers : RegisteredLayer->Parameters)
	{
		RequiredParameters.AddUnique(ParameterModifiers.Parameter);
	}

	// for (FAmbiverseProceduralElement ProceduralSoundData : RegisteredLayer->ProceduralSounds)
	// {
	// 	for (FAmbiverseParameterModifiers& ParameterModifiers : ProceduralSoundData.Parameters)
	// 	{
	// 		RequiredParameters.AddUnique(ParameterModifiers.Parameter);
	// 	}
	// }

	for (UAmbiverseParameter* RequiredParameter : RequiredParameters)
	{
		if (!ParameterRegistry.Contains(RequiredParameter))
		{
			RegisterParameter(RequiredParameter);
			UE_LOG(LogAmbiverseParameterManager, Verbose, TEXT("Registered parameter: '%s'"), *RequiredParameter->GetName());
		}
		else
		{
			UE_LOG(LogAmbiverseParameterManager, Verbose, TEXT("Parameter already registered: '%s'"), *RequiredParameter->GetName());
		}
	}
}

void UAmbiverseParameterManager::Deinitialize(UAmbiverseSubsystem* Subsystem)
{
	if (!Subsystem) { return; }
	
	if (UAmbiverseLayerManager* LayerManager {Subsystem->GetLayerManager()})
	{
		LayerManager->OnLayerRegistered.RemoveDynamic(this, &UAmbiverseParameterManager::HandleOnLayerRegistered);
	}

	Super::Deinitialize(Subsystem);
}

