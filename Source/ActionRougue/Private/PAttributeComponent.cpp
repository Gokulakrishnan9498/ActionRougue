// Fill out your copyright notice in the Description page of Project Settings.


#include "PAttributeComponent.h"

#include "PGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier (TEXT("pr.DamageMultiplier"),1.0f,TEXT("Damage Multiplier for Attribute Component"),ECVF_Cheat);

// Sets default values for this component's properties
UPAttributeComponent::UPAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
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
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta,0.0f,HealthMax);
	float ActualDelta = NewHealth - OldHealth;

	//Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MultiCastHealthChanged(InstigatorActor, Health,ActualDelta);
		}

		//Died
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			APGameModeBase* GM = GetWorld()->GetAuthGameMode<APGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	//OnHealthChanged.Broadcast(InstigatorActor,this,Health,ActualDelta);
	
	return ActualDelta != 0.0f;
}

float UPAttributeComponent::GetRage() const
{
	return Rage;
}

bool UPAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Delta,0.0f, RageMax);

	float ActualDelta = Rage - OldRage;

	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}
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

void UPAttributeComponent::MultiCastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor,this,NewHealth,Delta);
}

void UPAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor,this,NewRage,Delta);
}

void UPAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPAttributeComponent, Health);
	DOREPLIFETIME(UPAttributeComponent, HealthMax);
	DOREPLIFETIME(UPAttributeComponent, Rage);
	DOREPLIFETIME(UPAttributeComponent, RageMax);
}
