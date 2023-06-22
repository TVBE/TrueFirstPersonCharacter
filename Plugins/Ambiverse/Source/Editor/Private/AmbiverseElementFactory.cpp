// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin.

#include "AmbiverseElementFactory.h"
#include "AmbiverseElement.h"

UAmbiverseElementFactory::UAmbiverseElementFactory()
{
	SupportedClass = UAmbiverseElement::StaticClass();
	bCreateNew = true;
}

UObject* UAmbiverseElementFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UAmbiverseElement>(InParent, Class, Name, Flags, Context);
}