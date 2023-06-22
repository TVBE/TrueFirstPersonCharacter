// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin.

#include "AmbiverseCompositeFactory.h"
#include "AmbiverseComposite.h"

UAmbiverseCompositeFactory::UAmbiverseCompositeFactory()
{
	SupportedClass = UAmbiverseComposite::StaticClass();
	bCreateNew = true;
}

UObject* UAmbiverseCompositeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UAmbiverseComposite>(InParent, Class, Name, Flags, Context);
}