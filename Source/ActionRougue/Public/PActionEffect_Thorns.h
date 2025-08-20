// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PAction_Effect.h"
#include "PActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUE_API UPActionEffect_Thorns : public UPAction_Effect
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,Category="Thorns")
	float ReflectFraction;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UPAttributeComponent* OwningComp, float NewValue, float Delta);

public:
	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UPActionEffect_Thorns();
	
};
