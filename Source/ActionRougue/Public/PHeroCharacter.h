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
class UPActionComponent;

UCLASS()
class ACTIONROUGUE_API APHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHeroCharacter();

protected:
	// Called when the game starts or when spawned
	
	UPROPERTY(VisibleAnywhere , Category="Effects")
	FName TimeToHitParamName;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UPInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UPAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UPActionComponent* ActionComp;

	void MoveForward(float Value);
	
	void MoveRight(float Value);

	void StartSprint();

	void StopSprint();

	void PrimaryAttack();

	void DashAttack();

	void BlackHoleAttack();

	void PrimaryInteract();

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
