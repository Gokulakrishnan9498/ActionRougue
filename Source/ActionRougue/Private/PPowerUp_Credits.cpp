// Fill out your copyright notice in the Description page of Project Settings.


#include "PPowerUp_Credits.h"

#include "PAttributeComponent.h"
#include "PHeroPlayerState.h"

APPowerUp_Credits::APPowerUp_Credits()
{
	// MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// MeshComp->SetupAttachment(RootComponent);
	// MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CreditAmount = 80;
}

void APPowerUp_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	//Super::Interact_Implementation(InstigatorPawn);
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	if (APHeroPlayerState* PS = InstigatorPawn->GetPlayerState<APHeroPlayerState>())
	{
		PS->AddCredits(CreditAmount);
		HideAndFreezePowerUp();
	}
	
}
