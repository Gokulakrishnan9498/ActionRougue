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
class UPActionComponent;

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

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UPWorldUserWidget> SpottedWidgetClass;
	
	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UPAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere,Category="Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TargetActorKey;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UPAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION(BlueprintCallable, Category="AI")
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category="AI")
	AActor* GetTargetActor() const;
	
	void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen (APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();



};
