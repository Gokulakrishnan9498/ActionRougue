// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PAction.h"
#include "Components/ActorComponent.h"
#include "PActionComponent.generated.h"

class UPAction;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGUE_API UPActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPActionComponent();

	UFUNCTION(BlueprintCallable,Category="Action")
	void AddAction(TSubclassOf<UPAction> ActionClass);

	UFUNCTION(BlueprintCallable,Category="Action")
	bool StartActionByName(AActor* Instigator , FName ActionName);

	UFUNCTION(BlueprintCallable,Category="Action")
	bool StopActionByName(AActor* Instigator , FName ActionName);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<UPAction*> Actions;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
