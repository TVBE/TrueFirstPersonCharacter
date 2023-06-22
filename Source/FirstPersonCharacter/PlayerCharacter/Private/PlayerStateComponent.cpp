// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Tim Verberne.

#include "PlayerStateComponent.h"

void UPlayerStateComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	
	if (ConfigurationAsset.IsNull()) {return; }
	Configuration = ConfigurationAsset.LoadSynchronous();
}

void UPlayerStateComponent::BeginPlay()
{
	Super::BeginPlay();

	/** Start updating the player state. */
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(StateTimer, this, &UPlayerStateComponent::UpdatePlayerState, 1.0f, true);	
	}
}

void UPlayerStateComponent::ResetPlayerState()
{
	Pain = 100.0f;
	Exertion = 0.0f;
	Fear = 0.0f;
	Vigilance = 0.0f;
}

float UPlayerStateComponent::IncrementValue(float& Property, float Value)
{
	Value = FMath::Clamp(Value, 0, 100);
	if (Property + Value > 100)
	{
		Property = 100;
	}
	else
	{
		Property += Value;
	}
	return Property;
}

float UPlayerStateComponent::DecrementValue(float& Property, float Value)
{
	Value = FMath::Clamp(Value, 0, 100);
	if (Property < Value)
	{
		Property = 0;
	}
	else
	{
		Property -= Value;
	}
	return Property;
}

float UPlayerStateComponent::SetValue(float& Property, float Value)
{
	Value = FMath::Clamp(Value, 0, 100);
	Property = Value;
	return Property;
}

float UPlayerStateComponent::IncrementStateValue(const EPlayerStateValue Type, const float Value)
{
	switch(Type)
	{
	case EPlayerStateValue::Pain:
		if (Value <= 0.0f) {return Pain; }
		IncrementValue(Pain, Value);
		OnPainChanged.Broadcast(Pain);
		return Pain;
		
	case EPlayerStateValue::Exertion:
		if (Value <= 0.0f) {return Exertion; }
		IncrementValue(Exertion, Value);
		OnExertionChanged.Broadcast(Exertion);
		return Exertion;
		
	case EPlayerStateValue::Fear:
		if (Value <= 0.0f) {return Fear; }
		IncrementValue(Fear, Value);
		OnFearChanged.Broadcast(Fear);
		return Fear;
		
	case EPlayerStateValue::Vigilence:
		if (Value <= 0.0f) {return Vigilance; }
		IncrementValue(Vigilance, Value);
		OnVigilanceChanged.Broadcast(Vigilance);
		return Vigilance;
	}
	return 0.0f;
}

float UPlayerStateComponent::DecrementStateValue(const EPlayerStateValue Type, const float Value)
{
	switch(Type)
	{
	case EPlayerStateValue::Pain:
		if (Value <= 0.0f) {return Pain; }
		DecrementValue(Pain, Value);
		OnPainChanged.Broadcast(Pain);
		return Pain;
		
	case EPlayerStateValue::Exertion:
		if (Value <= 0.0f) {return Exertion; }
		DecrementValue(Exertion, Value);
		OnExertionChanged.Broadcast(Exertion);
		return Exertion;
		
	case EPlayerStateValue::Fear:
		if (Value <= 0.0f) {return Fear; }
		DecrementValue(Fear, Value);
		OnFearChanged.Broadcast(Fear);
		return Fear;
		
	case EPlayerStateValue::Vigilence:
		if (Value <= 0.0f) {return Vigilance; }
		DecrementValue(Vigilance, Value);
		OnVigilanceChanged.Broadcast(Vigilance);
		return Vigilance;
	}
	return 0.0f;
}

float UPlayerStateComponent::SetStateValue(const EPlayerStateValue Type, const float Value)
{
	switch(Type)
	{
	case EPlayerStateValue::Pain:
		SetValue(Pain, Value);
		OnPainChanged.Broadcast(Pain);
		return Pain;
		
	case EPlayerStateValue::Exertion:
		SetValue(Exertion, Value);
		OnExertionChanged.Broadcast(Exertion);
		return Exertion;
		
	case EPlayerStateValue::Fear:
		SetValue(Fear, Value);
		OnFearChanged.Broadcast(Fear);
		return Fear;
		
	case EPlayerStateValue::Vigilence:
		if (Value <= 0.0f) {return Vigilance; }
		SetValue(Vigilance, Value);
		OnVigilanceChanged.Broadcast(Vigilance);
		return Vigilance;
	}
	return 0.0f;
}

void UPlayerStateComponent::UpdatePlayerState()
{
	EventOnUpdate();
}

void UPlayerStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	/** Stop updating the player state. */
	StateTimer.Invalidate();
}

void UPlayerStateComponent::EventOnUpdate_Implementation()
{
}


	
