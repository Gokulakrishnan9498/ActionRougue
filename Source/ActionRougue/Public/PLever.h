// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PExplosiveBarrel.h"
#include "PGamePlayInterface.h"
#include "GameFramework/Actor.h"
#include "PLever.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROUGUE_API APLever : public AActor , public IPGamePlayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APLever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* Box;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* Lever;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Lever")
	float LeverRot;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Lever")
	APExplosiveBarrel* TargetBarrel;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn);

};
