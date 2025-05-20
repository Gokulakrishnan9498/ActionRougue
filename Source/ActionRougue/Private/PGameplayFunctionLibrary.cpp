// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameplayFunctionLibrary.h"

#include "PAttributeComponent.h"

bool UPGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UPAttributeComponent* AttributeComp = UPAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser,-DamageAmount);
	}
	return false;
}

bool UPGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser,TargetActor,DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			//Distance = Target - Origin
			FVector Distance = HitResult.TraceEnd - HitResult.TraceStart;
			Distance.Normalize();
			HitComp->AddImpulseAtLocation(Distance * 500000.0f , HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
