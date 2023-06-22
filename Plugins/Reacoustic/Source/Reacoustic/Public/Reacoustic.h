// Copyright (c) 2022-present Nino Saglia. All Rights Reserved.
// Written by Nino Saglia.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine/StaticMesh.h"

class FReacousticModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
