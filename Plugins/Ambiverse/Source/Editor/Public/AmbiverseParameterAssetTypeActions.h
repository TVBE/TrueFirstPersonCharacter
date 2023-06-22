// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FAmbiverseParameterAssetTypeActions : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
	virtual const TArray<FText>& GetSubMenus() const override;
};