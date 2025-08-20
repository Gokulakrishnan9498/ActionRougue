// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PActionComponent.h"
#include "Animation/AnimInstance.h"
#include "PAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGUE_API UPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UPActionComponent* ActionComp;

	//Is pawn stunned or not based on gameplay tags
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;

	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;
};
