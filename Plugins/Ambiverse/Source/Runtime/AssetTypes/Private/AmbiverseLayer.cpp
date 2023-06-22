// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseLayer.h"
#include "AmbiverseParameterManager.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseLayer, LogAmbiverseLayer);

#if WITH_EDITOR
void UAmbiverseLayer::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName = PropertyChangedEvent.Property->GetFName();
		
		if(PropertyName == GET_MEMBER_NAME_CHECKED(UAmbiverseLayer, ProceduralElements))
		{
			for(FAmbiverseProceduralElement& Element : ProceduralElements)
			{
				if(Element.IntervalRange.X > Element.IntervalRange.Y)
				{
					Element.IntervalRange.X = Element.IntervalRange.Y;
				}
			}
		}
	}
}
#endif

