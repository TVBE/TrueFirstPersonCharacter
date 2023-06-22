// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin.

#include "AmbiverseElementAssetTypeActions.h"
#include "AmbiverseElement.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText FAmbiverseElementAssetTypeActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_AmbiverseElement", "Ambiverse Element");
}

UClass* FAmbiverseElementAssetTypeActions::GetSupportedClass() const
{
	return UAmbiverseElement::StaticClass();
}

FColor FAmbiverseElementAssetTypeActions::GetTypeColor() const
{
	return FColor(100, 50, 130);
}

uint32 FAmbiverseElementAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Sounds;
}

const TArray<FText>& FAmbiverseElementAssetTypeActions::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		LOCTEXT("AssetAmbiverseSubMenu", "Ambiverse")
	};

	return SubMenus;
}