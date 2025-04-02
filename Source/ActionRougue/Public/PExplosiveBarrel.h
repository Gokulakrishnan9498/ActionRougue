// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "PExplosiveBarrel.generated.h"

UCLASS()
class ACTIONROUGUE_API APExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APExplosiveBarrel();
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BarrelMesh;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForce;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnBarrelHit (UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
public:	

	UFUNCTION()
	void Explode();

};

 
