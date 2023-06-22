// Copyright Notice

#pragma once

#include "CoreMinimal.h"
#include "UsableObjectInterface.h"
#include "Components/SceneComponent.h"
#include "MeshUseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginUseDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndUseDelegate);

UCLASS(NotBlueprintable, BlueprintType, ClassGroup = "Interaction", Meta = (BlueprintSpawnableComponent,
	DisplayName = "Mesh Use Component", ShortToolTip = "Component that allows the player to use an actor."))
class FIRSTPERSONCHARACTER_API UMeshUseComponent : public USceneComponent, public IUsableObject
{
	GENERATED_BODY()

public:
	/** Delegate that is broadcast when the player begins use on the component. */
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnBeginUseDelegate OnBeginUse;

	/** Delegate that is broadcast when the player ends use on the component. */
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnEndUseDelegate OnEndUse;

private:
	/** The use type for this component. */
	UPROPERTY(EditAnywhere)
	EUseType UseType {EUseType::SinglePress};

public:	
	UMeshUseComponent();

	/** IUsableObject implementation. */
	virtual bool BeginUse_Implementation(const AActor* Interactor) override
	{
		OnBeginUse.Broadcast();
		return true;
	};
	virtual bool EndUse_Implementation(const AActor* Interactor) override
	{
		OnEndUse.Broadcast();
		return true;
	};
	virtual EUseType GetUseType_Implementation() const override {return UseType; }

protected:
	virtual void BeginPlay() override;
	
};
