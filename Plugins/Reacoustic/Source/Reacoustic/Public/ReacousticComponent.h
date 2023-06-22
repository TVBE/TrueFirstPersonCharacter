// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Nino Saglia & Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReacousticDataTypes.h"
#include "components/AudioComponent.h"
#include "ReacousticComponent.generated.h"

class ReacousticSoundDataRef_Map;

UCLASS(Abstract, Blueprintable, ClassGroup = "Reacoustic", Meta = (BlueprintSpawnableComponent))
class REACOUSTIC_API UReacousticComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogReacousticComponent, Log, All)

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	UAudioComponent* AudioComponent {nullptr};

	UPROPERTY(Transient, BlueprintReadWrite, Meta = (DisplayName = "Sound Data Asset"))
	UReacousticSoundDataAsset* ReacousticSoundDataAsset {nullptr};

	UPROPERTY(Transient, BlueprintReadWrite, Meta = (DisplayName = "Sound Data Asset Reference Map"))	
	UReacousticSoundDataRef_Map* UReacousticSoundDataRefMap {nullptr};

	UPROPERTY(BlueprintReadWrite)
	FReacousticSoundData MeshAudioData;
	
	/** Used to choose the impact sound during a hit.*/
	UPROPERTY(BlueprintReadOnly, Category = Default, Meta = (DisplayName = "Impact Force"))	
	float ImpactForce;

private:
	/** Pointer to the StaticMeshComponent of the owner of this component. */
	UPROPERTY(Transient, BlueprintGetter = GetOwnerMeshComponent, Meta = (DisplayName = "Static Mesh Component"))
	UStaticMeshComponent* MeshComponent {nullptr};
	
	/** These variables are used to calculate the difference in distance and time between hits.
	 *  We use these to filter out unwanted hits */
	float LatestTime;

	FVector LatestLocation;
	
	float DeltaHitTime;
	float DeltaLocationDistance;
	float DeltaDirectionVector;
	
	FVector LatestForwardVector;
	FVector LatestRightVector;
	FVector LatestUpVector;
	
	TArray<float> DeltaStateArray;
	
	/** Array used to store the latest hit values so that we can prevent multiple triggers of the same sound.*/
	UPROPERTY(BlueprintReadOnly, Meta = (DisplayName = "Latest Hit Results", AllowPrivateAccess = "true"))
	TArray<float> LatestMatchingElements;

public:	
	UReacousticComponent();
	
	/** Callback function for the OnHit event delegate of a physics enabled static mesh component.
	 *	@HitComp The component that was hit.
	 *	@OtherActor The other actor that was hit by the component.
	 *	@OtherComp	The other actor's root component.
	 *	@NormalImpulse	The normal impulse of the hit.
	 *	@Hit A FHitResult struct of the hit.
	 */
	UFUNCTION()
	void HandleOnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	

	UFUNCTION(BlueprintNativeEvent, Category = Default, Meta = (DisplayName = "Initialize"))
	void Initialize(USoundBase* SoundBase = nullptr);
	
	UFUNCTION(BlueprintNativeEvent, Category = Default, Meta = (DisplayName = "Trigger manual hit"))
	void TriggerManualHit(float HitStrength);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	/** Returns the MeshComponent of the owner. */
	UFUNCTION(BlueprintGetter, Category = Reacoustic, Meta = (DisplayName = "Get Owner StaticMeshComponent"))
	FORCEINLINE UStaticMeshComponent* GetOwnerMeshComponent() const {return MeshComponent; }
	
	/** Get the time interval between hits */
	UFUNCTION(BlueprintPure)
	double ReturnDeltaTime();
	
	/** Get the location interval between hits */
	UFUNCTION(BlueprintPure)
	double ReturnDeltaLocationDistance();
	
	UFUNCTION()
	void TransferData(UReacousticSoundDataAsset* SoundDataArray, UReacousticSoundDataRef_Map* ReferenceMap, FReacousticSoundData MeshSoundDataIn);

	bool FilterImpact(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	int FindTimeStampEntry(FReacousticSoundData SoundData, float ImpactValue);


protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void TriggerManualHit_Implementation(float HitStrength);

	UFUNCTION(BlueprintCallable, Category = "Reacoustic", Meta = (DisplayName = "Get Scaled Impact Value"))
	static float CalculateImpactValue(const FVector& NormalImpulse, const UPrimitiveComponent* HitComponent, const AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Reacoustic", Meta = (DisplayName = "Get Surface Hit Sound"))
	static FReacousticSoundData GetSurfaceHitSoundX(const AActor* Actor, const UPhysicalMaterial* PhysicalMaterial);
};
