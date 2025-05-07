// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PInteractionComponent.h"
#include "GameFramework/Character.h"
#include "PHeroCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UPInteractionComponent;
class UAnimMontage;
class UPAttributeComponent;

UCLASS()
class ACTIONROUGUE_API APHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHeroCharacter();

protected:
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_DashAttack;
	FTimerHandle TimerHandle_BlackHoleAttack;
	

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> DashProjectileClass;

protected:
	// Called when the game starts or when spawned

	UPROPERTY(VisibleAnywhere , Category="Effects")
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere , Category="Effects")
	FName TimeToHitParamName;

	UPROPERTY(EditDefaultsOnly,Category="Attack");
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly,Category="Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditDefaultsOnly,Category="Attack")
	float AnimDelay;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UPInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UPAttributeComponent* AttributeComp;

	void MoveForward(float Value);
	
	void MoveRight(float Value);

	void StartAttackEffects();

	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void DashAttack();

	void DashAttack_TimeElapsed();

	void BlackHoleAttack();

	void BlackHoleAttack_TimeElapsed();

	void PrimaryInteract();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UPAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount=100);

};
