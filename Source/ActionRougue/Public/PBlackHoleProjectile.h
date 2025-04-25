// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PProjectileBase.h"
#include "PBlackHoleProjectile.generated.h"

/**
 * 
 */
class URadialForceComponent;
UCLASS()
class ACTIONROUGUE_API APBlackHoleProjectile : public APProjectileBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForce;

	UFUNCTION()
	void BlackHoleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void PostInitializeComponents() override;

public:
	APBlackHoleProjectile();
	
};
