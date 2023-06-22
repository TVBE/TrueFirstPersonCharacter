// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin.

#include "AmbiverseParameterFactory.h"
#include "AmbiverseParameter.h"

UAmbiverseParameterFactory::UAmbiverseParameterFactory()
{
	SupportedClass = UAmbiverseParameter::StaticClass();
	bCreateNew = true;
}

UObject* UAmbiverseParameterFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UAmbiverseParameter>(InParent, Class, Name, Flags, Context);
}