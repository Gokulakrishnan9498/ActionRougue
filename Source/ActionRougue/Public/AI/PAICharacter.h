// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "PAICharacter.generated.h"

class UPawnSensingComponent;
class UPAttributeComponent;
class UUserWidget;
class UPWorldUserWidget;

UCLASS()
class ACTIONROUGUE_API APAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APAICharacter();

protected:

	UPWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UPAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere,Category="Effects")
	FName TimeToHitParamName;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UPAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);
	
	void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen (APawn* Pawn);



};
