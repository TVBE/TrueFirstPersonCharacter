// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "UsableObjectInterface.generated.h"

UENUM(BlueprintType)
enum class EUseType : uint8
{
	SinglePress			UMETA(DisplayName = "Single Press"),
	PressAndHold		UMETA(DisplayName = "Press And Hold"),
};

/** Interface for usable objects. This interface can be implemented in any UObject derived class. */
UINTERFACE(Blueprintable, Meta = (DisplayName = "Usable Object Interface", ShortToolTip = "Interface for usable objects."))
class FIRSTPERSONCHARACTER_API UUsableObject : public UInterface
{
	GENERATED_BODY()
};

class FIRSTPERSONCHARACTER_API IUsableObject
{
	GENERATED_BODY()

public:
	/** Begins interaction with the object.
	 *	@Param Instigator The actor that instigated the interaction.
	 *	@Return Whether the object implements an interaction.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Object")
	bool BeginUse(const AActor* Interactor);

	/** Ends interaction with the object.
	 *	@Param Instigator The actor that instigated the interaction.
	 *	@Return Whether the object implements an interaction.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Object")
	bool EndUse(const AActor* Interactor);

	/** Returns whether the object has a single-press interaction, or a press-and-hold interaction. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable Object")
	EUseType GetUseType() const;
};
