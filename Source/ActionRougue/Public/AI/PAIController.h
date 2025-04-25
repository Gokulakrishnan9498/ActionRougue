// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PAIController.generated.h"

/**
 * 
 */
class UBehaviorTree;
UCLASS()
class ACTIONROUGUE_API APAIController : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,Category="AI")
	UBehaviorTree* BehaviorTree;

public:
	virtual void BeginPlay() override;
	
};
