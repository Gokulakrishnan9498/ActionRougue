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

UCLASS()
class ACTIONROUGUE_API APHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHeroCharacter();

protected:
	// Called when the game starts or when spawned

	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly,Category="Attack");
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly,Category="Attack")
	float AnimDelay;

	UPROPERTY(EditDefaultsOnly, Category="Attack");
	TSubclassOf<AActor> ProjectileClass;
	
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UPInteractionComponent* InteractionComp;

	void MoveForward(float Value);
	
	void MoveRight(float Value);

	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
