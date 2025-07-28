// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "PMagicProjectile.generated.h"

class UPAction_Effect;
class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class UAudioComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class ACTIONROUGUE_API APMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	APMagicProjectile();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere , Category="Damage")
	float DamageAmount;

	UPROPERTY(EditAnywhere , Category="Damage")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditAnywhere , Category="Damage")
	float ImpactShakeOuterRadius;

	UPROPERTY(EditDefaultsOnly , Category="Damage")
	TSubclassOf<UCameraShakeBase> ImpactShake;

	UPROPERTY(EditDefaultsOnly , Category="Damage")
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly , Category="Damage")
	USoundCue* ImpactSound;

	UPROPERTY(EditDefaultsOnly , Category="Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly , Category="Damage")
	TSubclassOf<UPAction_Effect> BurningActionClass;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
