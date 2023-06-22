// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "DraggableObjectInterface.generated.h"

/** Interface for draggable objects. This interface can be implemented in any UObject derived class. */
UINTERFACE(Blueprintable, Meta = (DisplayName = "Draggable Object Interface", ShortToolTip = "Interface for draggable objects."))
class FIRSTPERSONCHARACTER_API UDraggableObject : public UInterface
{
	GENERATED_BODY()
};

class FIRSTPERSONCHARACTER_API IDraggableObject
{
	GENERATED_BODY()

public:
	/** Starts dragging the object.
	 *	@Param Instigator The actor that instigated the interaction.
	 *	@Return Whether the object implements a special action on drag.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Draggable Object")
	bool BeginDrag(const AActor* Interactor);

	/** Stop dragging the object.
	 *	@Param Instigator The actor that instigated the interaction.
	 *	@Return Whether the object implements a special action on release.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Draggable Object")
	bool EndDrag(const AActor* Interactor);
};