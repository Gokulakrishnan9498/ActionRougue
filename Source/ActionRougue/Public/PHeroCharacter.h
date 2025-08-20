// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PInteractionComponent.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "PHeroCharacter.generated.h"

class UInputMappingContext;
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

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_LookMouse;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_LookStick;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_Interact;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_Sprint;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_Dash;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_PrimaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Input_SecondaryAttack;
	
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

	/*void MoveForward(float Value);
	
	void MoveRight(float Value);*/

	void Move(const FInputActionInstance& Instance);

	void LookMouse(const FInputActionInstance& InputValue);

	void LookStick(const FInputActionInstance& InputValue);

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

	void FindCrosshairTarget();

	void CrosshairTraceComplete(const FTraceHandle& InTraceHandle, FTraceDatum& InTraceDatum);

	FTraceHandle TraceHandle;

private:

	bool bHasPawnTarget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount=100);

};
