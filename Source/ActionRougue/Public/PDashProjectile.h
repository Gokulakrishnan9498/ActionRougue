// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PProjectileBase.h"
#include "PDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUE_API APDashProjectile : public APProjectileBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category="Teleport")
	float DetonateDelay;

	UPROPERTY(EditAnywhere,Category="Teleport")
	float TeleportDelay;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void BeginPlay() override;

public:
	APDashProjectile();
	
};
