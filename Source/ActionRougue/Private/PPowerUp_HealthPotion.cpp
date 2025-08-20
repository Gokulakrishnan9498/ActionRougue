// Fill out your copyright notice in the Description page of Project Settings.


#include "PPowerUp_HealthPotion.h"

#include "PAttributeComponent.h"
#include "PHeroPlayerState.h"


#define LOCTEXT_NAMESPACE "InteractableActors"

APPowerUp_HealthPotion::APPowerUp_HealthPotion()
{
	// MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// MeshComp->SetupAttachment(RootComponent);
	// MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CreditCost = 50;
}

void APPowerUp_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	//Super::Interact_Implementation(InstigatorPawn);
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	UPAttributeComponent* AttributeComp =UPAttributeComponent::GetAttributes(InstigatorPawn);
	
		if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
		{
			if (APHeroPlayerState* PS = InstigatorPawn->GetPlayerState<APHeroPlayerState>())
			{
				if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this,AttributeComp->GetMaxHealth()))
				{
					HideAndFreezePowerUp();
				}
			}
		}
	
	
}

FText APPowerUp_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UPAttributeComponent* AttributeComp =UPAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at Full Health");
	}
	return FText::Format(LOCTEXT("HealthPotion_InteractText", "Cost {0} Credits. Restores Health to Maximum."), CreditCost);
}

#undef LOCTEXT_NAMESPACE