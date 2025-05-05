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

bool UPAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor,-HealthMax);
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

bool UPAttributeComponent::ApplyHealthChange(AActor* InstigatorActor , float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta,0.0f,HealthMax);
	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor,this,Health,ActualDelta);
	
	return ActualDelta != 0.0f;
}

UPAttributeComponent* UPAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return  Cast<UPAttributeComponent>(FromActor->GetComponentByClass(UPAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UPAttributeComponent::IsActorAlive(AActor* Actor)
{
	UPAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}
