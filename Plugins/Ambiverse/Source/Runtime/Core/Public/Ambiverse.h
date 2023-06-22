// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FAmbiverseModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
