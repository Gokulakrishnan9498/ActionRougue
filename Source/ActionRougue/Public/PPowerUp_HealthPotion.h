// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPowerUpActor.h"
#include "PPowerUp_HealthPotion.generated.h"

/**
 * 
 */
class UStaticMeshComponent;
UCLASS()
class ACTIONROUGUE_API APPowerUp_HealthPotion : public APPowerUpActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere,Category= "Components")
	UStaticMeshComponent* MeshComp;

public:
	APPowerUp_HealthPotion();
	void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
