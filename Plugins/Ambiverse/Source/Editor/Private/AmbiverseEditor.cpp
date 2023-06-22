// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin.

#include "AmbiverseEditor.h"
#include "AmbiverseLayerAssetTypeActions.h"
#include "AmbiverseParameterAssetTypeActions.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

void FAmbiverseEditorModule::StartupModule()
{
	IAssetTools& AssetTools {FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get()};
	
	UE_LOG(LogTemp, Error, TEXT("Startup"))

	AmbiverseElementAssetTypeActions = MakeShared<FAmbiverseElementAssetTypeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(AmbiverseElementAssetTypeActions.ToSharedRef());
	
	AmbiverseLayerAssetTypeActions = MakeShared<FAmbiverseLayerAssetTypeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(AmbiverseLayerAssetTypeActions.ToSharedRef());

	AmbiverseCompositeAssetTypeActions = MakeShared<FAmbiverseCompositeAssetTypeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(AmbiverseCompositeAssetTypeActions.ToSharedRef());

	AmbiverseParameterAssetTypeActions = MakeShared<FAmbiverseParameterAssetTypeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(AmbiverseParameterAssetTypeActions.ToSharedRef());
}

void FAmbiverseEditorModule::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;

	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(AmbiverseElementAssetTypeActions.ToSharedRef());
	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(AmbiverseLayerAssetTypeActions.ToSharedRef());
	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(AmbiverseCompositeAssetTypeActions.ToSharedRef());
	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(AmbiverseParameterAssetTypeActions.ToSharedRef());
}

IMPLEMENT_MODULE(FAmbiverseEditorModule, AmbiverseEditor)