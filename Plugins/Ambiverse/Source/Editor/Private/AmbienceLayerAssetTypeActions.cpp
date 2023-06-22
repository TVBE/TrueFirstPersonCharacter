// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin.

#include "AmbiverseLayerAssetTypeActions.h"
#include "AmbiverseLayer.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText FAmbiverseLayerAssetTypeActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_AmbiverseLayer", "Ambiverse Layer");
}

UClass* FAmbiverseLayerAssetTypeActions::GetSupportedClass() const
{
	return UAmbiverseLayer::StaticClass();
}

FColor FAmbiverseLayerAssetTypeActions::GetTypeColor() const
{
	return FColor(100, 50, 130);
}

uint32 FAmbiverseLayerAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Sounds;
}

const TArray<FText>& FAmbiverseLayerAssetTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
{
	LOCTEXT("AssetAmbiverseSubMenu", "Ambiverse")
};
	return SubMenus;
}
