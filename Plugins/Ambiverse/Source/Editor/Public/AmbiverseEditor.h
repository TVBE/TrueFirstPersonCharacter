// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseCompositeAssetTypeActions.h"
#include "AmbiverseElementAssetTypeActions.h"
#include "AmbiverseLayerAssetTypeActions.h"
#include "AmbiverseParameterAssetTypeActions.h"
#include "Modules/ModuleManager.h"

class FAmbiverseEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FAmbiverseElementAssetTypeActions> AmbiverseElementAssetTypeActions;
	TSharedPtr<FAmbiverseLayerAssetTypeActions> AmbiverseLayerAssetTypeActions;
	TSharedPtr<FAmbiverseCompositeAssetTypeActions> AmbiverseCompositeAssetTypeActions;
	TSharedPtr<FAmbiverseParameterAssetTypeActions> AmbiverseParameterAssetTypeActions;
};