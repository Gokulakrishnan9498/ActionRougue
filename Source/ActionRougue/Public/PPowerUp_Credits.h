// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPowerUpActor.h"
#include "PPowerUp_Credits.generated.h"

/**
 * 
 */
class UStaticMeshComponent;

UCLASS()
class ACTIONROUGUE_API APPowerUp_Credits : public APPowerUpActor 
{
	GENERATED_BODY()

private:
	// UPROPERTY(VisibleAnywhere,Category="Components")
	// UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere,Category="Credits")
	int32 CreditAmount;
	
public:
	APPowerUp_Credits();

	void Interact_Implementation(APawn* InstigatorPawn) override;

	
	
	
};
