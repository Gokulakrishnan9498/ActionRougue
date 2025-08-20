// Fill out your copyright notice in the Description page of Project Settings.


#include "PActionEffect_Thorns.h"

#include "PActionComponent.h"
#include "PAttributeComponent.h"
#include "PGameplayFunctionLibrary.h"

UPActionEffect_Thorns::UPActionEffect_Thorns()
{
	ReflectFraction = 0.2f; // 20% of damage dealt back to attacker

	Duration = 0.0f; // Thorns effect lasts indefinitely until stopped
	Period = 0.0f; // No periodic effect, just immediate reflection
}

void UPActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	//Start Listening
	UPAttributeComponent* AttributeComp = UPAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UPActionEffect_Thorns::OnHealthChanged);
	}
}

void UPActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	//Stop Listening
	UPAttributeComponent* AttributeComp = UPAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UPActionEffect_Thorns::OnHealthChanged);
	}
}

void UPActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UPAttributeComponent* OwningComp, float NewValue,float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	//Damage Only
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);

		if (ReflectedAmount == 0)
		{
			return;
		}
		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// Return damage sender...
		UPGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}
