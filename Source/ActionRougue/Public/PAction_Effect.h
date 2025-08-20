// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PAction.h"
#include "PAction_Effect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUE_API UPAction_Effect : public UPAction
{
	GENERATED_BODY()

public:
	UPAction_Effect();
	
	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable, Category="Action")
	float GetTimeRemaining() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	float Period;

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;

	UFUNCTION(BlueprintNativeEvent, Category="Effects")
	void ExecutePeriodicEffect(AActor* Instigator);
	
	
};
