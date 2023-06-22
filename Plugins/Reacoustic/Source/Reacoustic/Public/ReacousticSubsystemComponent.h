// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "ReacousticSubsystemComponent.generated.h"

class UReacousticSubsystem;

UCLASS(MinimalAPI, Transient, Within = "ReacousticSubsystem", ClassGroup = "Reacoustic")
class UReacousticSubsystemComponent : public UObject
{
	GENERATED_BODY()

private:
	bool IsInitialized {false};
	
protected:
	UPROPERTY()
	UReacousticSubsystem* Owner;

public:
	virtual void Initialize(UReacousticSubsystem* Subsystem);
	virtual void Deinitialize(UReacousticSubsystem* Subsystem);
	
	FORCEINLINE UReacousticSubsystem* GetOwner() const { return Owner; }
	FORCEINLINE bool GetIsInitialized() const { return IsInitialized; } 
};
