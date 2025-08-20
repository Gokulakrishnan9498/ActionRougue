// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PAction.h"
#include "PPowerUpActor.h"
#include "PPowerUp_Action.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUE_API APPowerUp_Action : public APPowerUpActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="PowerUp")
	TSubclassOf<UPAction> ActionToGrant;

public:
	
	void Interact_Implementation(APawn* InstigatorPawn) override;
};
