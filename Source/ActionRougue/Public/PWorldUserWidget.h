// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "PWorldUserWidget.generated.h"

/**
 * 
 */

class USizeBox;
UCLASS()
class ACTIONROUGUE_API UPWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, Category="UI", meta=(ExposeOnSpawn = true))
	AActor* AttachedActor;

	UPROPERTY(EditAnywhere,Category="UI")
	FVector WorldOffset;
	
};
