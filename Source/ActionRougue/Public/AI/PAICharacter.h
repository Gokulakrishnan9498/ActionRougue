// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PAICharacter.generated.h"

class UPawnSensingComponent;
class UPAttributeComponent;

UCLASS()
class ACTIONROUGUE_API APAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APAICharacter();

protected:
	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere,Category="Components")
	UPAttributeComponent* AttributeComp;

	void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen (APawn* Pawn);



};
