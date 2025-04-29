// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PBTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUE_API UPBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere,Category="AI",meta=(ClampMin="0.0",ClampMax="1.0"))
	float LowHealthFraction;

	UPROPERTY(EditAnywhere,Category="AI")
	FBlackboardKeySelector LowHealthKey;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPBTService_CheckHealth();
	
};
