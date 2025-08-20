// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PAction.h"
#include "Components/ActorComponent.h"
#include "PActionComponent.generated.h"

class UPAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UPActionComponent*, OwningComp, UPAction*, Action);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGUE_API UPActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPActionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable,Category="Action")
	void AddAction(AActor* Instigator, TSubclassOf<UPAction> ActionClass);

	UFUNCTION(BlueprintCallable,Category="Action")
	void RemoveAction(UPAction* ActionToRemove);

	UFUNCTION(BlueprintCallable,Category="Action")
	UPAction* GetAction(TSubclassOf<UPAction> ActionClass) const;

	UFUNCTION(BlueprintCallable,Category="Action")
	bool StartActionByName(AActor* Instigator , FName ActionName);

	UFUNCTION(BlueprintCallable,Category="Action")
	bool StopActionByName(AActor* Instigator , FName ActionName);

	

protected:
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UPAction*> Actions;

	UPROPERTY(EditAnywhere,Category="Action")
	TArray<TSubclassOf<UPAction>> DefaultActions;

public:

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
		
};
