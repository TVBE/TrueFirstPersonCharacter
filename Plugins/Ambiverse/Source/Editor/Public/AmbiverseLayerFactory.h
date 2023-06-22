// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AmbiverseLayerFactory.generated.h"

UCLASS()
class AMBIVERSEEDITOR_API UAmbiverseLayerFactory : public UFactory
{
	GENERATED_BODY()

public:
	UAmbiverseLayerFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

