// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Nino Saglia.

#include "ReacousticSettings.h"
#include "ReacousticSubsystem.h"
#include "Reacoustic.h"
#include "AudioAnalyzerNRT.h"
#include "OnsetNRT.h"
#include "Engine/DataTable.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "Serialization/JsonTypes.h"

UReacousticProjectSettings::UReacousticProjectSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR
void UReacousticProjectSettings::GenerateRuntimeData()
{
	//TODO: Create the assets
	if (ReacousticSoundDataAsset){}
	if (ReacousticSoundDataRefMap){}
	UDataTable* Objects = Cast<UDataTable>(ReacousticObjects.TryLoad());
	UDataTable* Surfaces = Cast<UDataTable>(ReacousticSurfaces.TryLoad());
	if (Objects)
	{
		/** Check that the RowStruct type is correct */
		if (Objects->RowStruct->IsChildOf(FReacousticObjects::StaticStruct()))
		{
			for (auto& Row : Objects->GetRowMap())
			{
				FName RowName = Row.Key;

				FReacousticObjects* RowData = Objects->FindRow<FReacousticObjects>(RowName, TEXT("ContextString"));

				if(RowData)
				{
					/** Create FReacousticSoundData from the RowData and add it to your asset */
					FReacousticSoundData NewData;
					NewData.Gain = RowData->Gain_Db;
					NewData.ImpulseLength = RowData->ImpulseLength;
					NewData.ImpactWaveAsset = RowData->HitSound.Get();
					NewData.MaxSpeedScalar = RowData->MaxSpeedScalar;

					//TODO: create an onset nrt and delete it later.
					UOnsetNRT* OnsetNRT = NewObject<UOnsetNRT>();

					
					OnsetNRT->Sound = RowData->HitSound.Get();
					
					TArray<float> OutOnsetTimestamps;
					TArray<float> OutOnsetStrengths;
					/**Retreive the onset data.*/
					OnsetNRT->GetNormalizedChannelOnsetsBetweenTimes(0.0,RowData->HitSound->Duration,2,OutOnsetTimestamps,OutOnsetStrengths);
					
					int NewIndex = ReacousticSoundDataAsset->AudioData.Add(NewData);

					/** Now create entries in UReacousticSoundDataRef_Map for each static mesh */
					for (auto& Mesh : RowData->Meshes)
					{
						FMeshToAudioMapEntry NewEntry;
						NewEntry.Mesh = Mesh;
						NewEntry.ReacousticSoundDataRef = NewIndex;
						ReacousticSoundDataRefMap->MeshMapEntries.Add(NewEntry);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("DataTable has incorrect RowStruct type"));
		}
	}
}

void UReacousticProjectSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GenerateRuntimeData();
}

#endif