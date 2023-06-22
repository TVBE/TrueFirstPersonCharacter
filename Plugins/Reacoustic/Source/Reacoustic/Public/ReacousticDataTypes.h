// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Nino Saglia.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundConcurrency.h"
#include "Sound/SoundAttenuation.h"
#include "ReacousticDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FReacousticSoundData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ReacousticSoundData", Meta = (ClampMin = "-50.0", ClampMax = "50.0", UIMin = "-50.0", UIMax = "50.0"))
	float Gain{0.0};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ReacousticSoundData)
	float MaxSpeedScalar{1.0};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ReacousticSoundData)
	float ImpulseLength{1.5};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Analysis)
	TArray<float> OnsetTimingData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Analysis)
	TMap<float, float> OnsetDataMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Analysis)
	TArray<float> OnsetVolumeData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sounds)
	USoundWave* ImpactWaveAsset {nullptr};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sounds)
	USoundWave* SlidingWaveAsset {nullptr};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Surface, Meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float SurfaceDampeningPercentage{0.0};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ReacousticSoundData)
	USoundAttenuation* Attenuation{nullptr};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ReacousticSoundData)
	USoundConcurrency* Concurrency{nullptr};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ReacousticSoundData)
	float SoundSenseVolume{10.0f};

	

	FReacousticSoundData(){}
	
};

UCLASS(BlueprintType)
class UReacousticSoundDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ReacousticSoundData)
	TArray<FReacousticSoundData> AudioData;
	
};

USTRUCT(BlueprintType)
struct FMeshToAudioMapEntry
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh To Audio Map")
	UStaticMesh* Mesh{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh To Audio Map")
	int32 ReacousticSoundDataRef{0};
};

USTRUCT(BlueprintType)
struct FPhysicalMaterialToAudioMapEntry
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EPhysicalSurface To Audio Map")
	TEnumAsByte<EPhysicalSurface> SurfaceType{SurfaceType_Default};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EPhysicalSurface To Audio Map")
	int32 ReacousticSoundDataRef{0};
	
};

// The map with all references to quickly be able to access the right audio data.
UCLASS(BlueprintType)
class UReacousticSoundDataRef_Map : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MeshToAudioMap)
	TArray<FMeshToAudioMapEntry> MeshMapEntries;
 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MaterialToAudioMap)
	TArray<FPhysicalMaterialToAudioMapEntry> PhysicalMaterialMapEntries;
};

USTRUCT(BlueprintType)
struct FReacousticObjects
{
	GENERATED_USTRUCT_BODY()

public:
	/** The gain in decibels of the impact sound */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Gain_Db"))
double Gain_Db {0.0};

/** The sound to play when the impact occurs */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="HitSound"))
TObjectPtr<USoundWave> HitSound;

/** An array of meshes that can be used for the impact effect */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Meshes"))
TArray<UStaticMesh*> Meshes;

/** The sound to play when the object is sliding or rolling */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="SlidingRollingSound"))
TObjectPtr<USoundWave> SlidingRollingSound;

	/** How fast the object is expected to hit things eg: a cup would be 1. A basketball 5 */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="MaxSpeedScalar", MakeStructureDefaultValue="1.000000"))
double MaxSpeedScalar{1.0};

/** The attenuation settings for the impact sound */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Sound_Attenuation", MakeStructureDefaultValue="/Script/Engine.SoundAttenuation'/Game/ThirdPartyContent/BallisticsVFX/SFX/Attentuations/ImpactsAttenuation.ImpactsAttenuation'"))
TObjectPtr<USoundAttenuation> Sound_Attenuation;

/** The sound concurrency settings for the impact sound */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Sound_Concurrency", MakeStructureDefaultValue="None"))
TObjectPtr<USoundConcurrency> Sound_Concurrency;

/** The length of the impulse for the impact effect */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="ImpulseLength", MakeStructureDefaultValue="0.600000"))
double ImpulseLength{0.6};

/** The pitch shift in semitones for the impact sound */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Semitone pitch shift", MakeStructureDefaultValue="0.0"))
float PitchShift{0.0};

/** Whether to use weight-dependent pitch for the impact sound */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Use weight-dependent pitch", MakeStructureDefaultValue="false"))
bool UseWeightDependentPitch{false};

/** The default weight for weight-dependent pitch for the impact sound */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Default weight", MakeStructureDefaultValue="0.0", EditCondition="UseWeightDependentPitch"))
float DefaultWeight{10.0};

/** The pitch factor for weight-dependent pitch for the impact sound */
UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Weight pitch factor", MakeStructureDefaultValue="0.0", EditCondition="UseWeightDependentPitch"))
float WeightPitchFactor{0.0};

	FReacousticObjects(){};
};


USTRUCT(BlueprintType)
struct FReacousticSurfaces
{
	GENERATED_USTRUCT_BODY()
public:
	/** The gain for the hitsound */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Gain_Db", MakeStructureDefaultValue="0.000000"))
	double Gain_Db{0.0};

	/** The source sound to be used */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="HitSound", MakeStructureDefaultValue="None"))
	TObjectPtr<USoundWave> HitSound;

	/** Surfacematerial for this sound */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Material", MakeStructureDefaultValue="SurfaceType_Default"))
	TEnumAsByte<EPhysicalSurface> Material;

	/** How much a surface sound would dampen the velocity of the object sound */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="SurfaceDampening%", MakeStructureDefaultValue="0.000000"))
	double SurfaceDampeningPercentage{0.0};

	/** Looping sound to be used for when the object is sliding */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="SlidingRollingSound", MakeStructureDefaultValue="None"))
	TObjectPtr<USoundWave> SlidingRollingSound;

	/** How fast the object is expected to hit things eg: a cup would be 1. A basketball 5 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="MaxSpeedScalar", MakeStructureDefaultValue="1.000000"))
	double MaxSpeedScalar{1.0};

	/** How long the sound should be */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="ImpulseLength", MakeStructureDefaultValue="0.500000"))
	double ImpulseLength{0.6};

	FReacousticSurfaces(){}
};
