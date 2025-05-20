// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PAction.h"
#include "PAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUE_API UPAction_ProjectileAttack : public UPAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere , Category="Effects")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly,Category="Attack");
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly,Category="Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditDefaultsOnly,Category="Attack")
	float AnimDelay;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:
	UPAction_ProjectileAttack();

	UFUNCTION()
	void StartAction_Implementation(AActor* Instigator) override;
	
};
