// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "GrabbableObjectInterface.generated.h"

/** Defines whether the actor requires one or two hands to perform the interaction. */
UENUM(BlueprintType)
enum class EGrabType : uint8
{
	OneHanded			UMETA(DisplayName = "One Handed"),
	TwoHanded			UMETA(DisplayName = "Two Handed"),
};

/** Interface for grabbable objects. This interface can be implemented in any UObject derived class. */
UINTERFACE(Blueprintable, Meta = (DisplayName = "Grabbable Object Interface", ShortToolTip = "Interface for grabbable objects."))
class FIRSTPERSONCHARACTER_API UGrabbableObject : public UInterface
{
	GENERATED_BODY()
};

class FIRSTPERSONCHARACTER_API IGrabbableObject
{
	GENERATED_BODY()

public:
	/** Starts grabbing the object.
	 *	@Param Instigator The actor that instigated the interaction.
	 *	@Return Whether the object implements a special action on grab.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable Object")
	bool BeginGrab(const AActor* Interactor);

	/** Stop grabbing the object.
	 *	@Param Instigator The actor that instigated the interaction.
	 *	@Return Whether the object implements a special action on release.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable Object")
	bool EndGrab(const AActor* Interactor);
	
	/** Returns the interaction hand type of the object. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable Object")
	EGrabType GetGrabType() const;
};