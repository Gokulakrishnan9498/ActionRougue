// Fill out your copyright notice in the Description page of Project Settings.


#include "PAttributeComponent.h"

// Sets default values for this component's properties
UPAttributeComponent::UPAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}

bool UPAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float UPAttributeComponent::GetHealth() const
{
	return Health;
}

float UPAttributeComponent::GetMaxHealth() const
{
	return HealthMax;
}

bool UPAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

bool UPAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta,0.0f,HealthMax);
	float ActualDelta = OldHealth - Health;
	OnHealthChanged.Broadcast(nullptr,this,Health,ActualDelta);
	
	return ActualDelta != 0.0f;
}


