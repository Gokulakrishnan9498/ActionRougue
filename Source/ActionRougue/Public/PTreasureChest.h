// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PGamePlayInterface.h"
#include "GameFramework/Actor.h"
#include "PTreasureChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROUGUE_API APTreasureChest : public AActor , public IPGamePlayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APTreasureChest();

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	void Interact_Implementation(APawn* InstigatorPawn) ;

	void OnActorLoaded_Implementation();

protected:

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly, SaveGame)  //RepNotify
	bool bLidOpened;
	
	UFUNCTION()
	void OnRep_LidOpened();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="KeyCard")
	FGameplayTag RequiredKeyCard;
	

};
