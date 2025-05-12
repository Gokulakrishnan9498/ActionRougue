// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PHeroPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged , APHeroPlayerState*, PlayerState, int32, NewCredits, int32, Delta);


UCLASS()
class ACTIONROUGUE_API APHeroPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly,Category="Credits")
	int32 Credits;

public:
	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;
	
	UFUNCTION(BlueprintCallable,Category="Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable,Category="Credits")
	bool RemoveCredits(int32 Delta);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;
	
	
};
