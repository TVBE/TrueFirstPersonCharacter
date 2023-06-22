// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "UsableObjectInterface.h"
#include "GameFramework/Actor.h"
#include "Desktop.generated.h"

class UMeshGrabComponent;
class UBoxComponent;
class UStaticMeshComponent;

/** Base class for an all-in-one desktop that can receive keyboard and mouse input.
 *	This class is expected to be extended and fully implemented in a blueprint derived class. */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "Desktop", Meta = (DisplayName = "Desktop"))
class FIRSTPERSONCHARACTER_API ADesktop : public AActor, public IUsableObject
{
	GENERATED_BODY()

protected:
	/** The static mesh of the desktop screen. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Screen")
	UStaticMeshComponent* ScreenMesh;

	/** The text displayed on the desktop screen. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Screen", Meta = (DisplayName = "Cursor Screen Space"))
	UBoxComponent* ScreenSpace;
	
	/** The static mesh for the screen cursor*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Cursor")
	UStaticMeshComponent* CursorMesh;

	/** The mesh interaction component for this actor. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMeshGrabComponent* GrabComponent;
	
	/** The text displayed on the desktop screen. */
	UPROPERTY(BlueprintReadWrite, Category = "Text")
	TArray<FString> TextLineArray;

	/** The amount of lines that can be displayed on the desktop screen. */
	UPROPERTY(BlueprintReadOnly, Category = "Text", Meta = (DisplayName = "Maximum Lines On Screen"))
	int32 MaxLines {18};

	/** The amount of characters allowed per line. */
	UPROPERTY(BlueprintReadOnly, Category = "Text", Meta = (DisplayName = "Maximum Characters Per Line"))
	int32 MaxLineLength {72};

	/** The full text to display on the desktop's screen. */
	UPROPERTY(BlueprintReadOnly, Category = "Text")
	FString DisplayText;

	/** The time the player requires to hold the use key down to turn on or shut off the computer. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", Meta = (DisplayName = "Use Press And Hold Time"))
	float UseTime {1.0f};

	/** If true, the desktop is currently turned on. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTurnedOn {true};

	/** If true, the desktop is currently destroyed. */
	UPROPERTY(BlueprintReadWrite)
	bool IsBroken {false};
	
public:	
	ADesktop();

	bool BeginUse_Implementation(const AActor* Interactor) override;
	bool EndUse_Implementation(const AActor* Interactor) override;
	FORCEINLINE EUseType GetUseType_Implementation() const override { return EUseType::PressAndHold; }

protected:
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;

	/** Moves the cursor over the screen. */
	UFUNCTION(BlueprintCallable, Category = "Cursor")
	void MoveCursor(FVector2D InputVelocity);
	
	/** Formats the display text. */
	UFUNCTION(BlueprintCallable, Category = "Keyboard")
	void FormatDisplayText();
	
	/** Adds a character to the array of text lines. */
	UFUNCTION(BlueprintCallable, Category = "Keyboard", Meta = (DisplayName = "Add Character To Display List"))
	void AppendCharacterToArray(const FString& Character);

	/** Returns a reference to the last line available with space. */
	UFUNCTION(BlueprintCallable, Category = "Keyboard", Meta = (DisplayName = "Get Available Text Line From List"))
	UPARAM(ref) FString& GetLastLineWithSpace();
};
	
