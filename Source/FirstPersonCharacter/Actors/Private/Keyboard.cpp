// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "Keyboard.h"

AKeyboard::AKeyboard()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AKeyboard::BeginPlay()
{
	Super::BeginPlay();
}

FString AKeyboard::GetRandomCharacter()
{
	const FRandomStream RandomStream(FMath::Rand());
	const int32 RandomIndex {RandomStream.RandRange(0, 25)};
	const TCHAR RandomChar {TCHAR('A' + RandomIndex)};
	return FString(1, &RandomChar);
}


