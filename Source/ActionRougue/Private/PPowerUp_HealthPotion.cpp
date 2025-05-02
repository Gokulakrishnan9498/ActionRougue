// Fill out your copyright notice in the Description page of Project Settings.


#include "PPowerUp_HealthPotion.h"

#include "PAttributeComponent.h"
#include "SkeletonTreeBuilder.h"

APPowerUp_HealthPotion::APPowerUp_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
		if (AttributeComp->ApplyHealthChange(this,AttributeComp->GetMaxHealth()))
		{
			HideAndFreezePowerUp();
		}
	}
}
