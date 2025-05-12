// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PGamePlayInterface.h"
#include "GameFramework/Actor.h"
#include "PPowerUpActor.generated.h"

class USphereComponent;
UCLASS()
class ACTIONROUGUE_API APPowerUpActor : public AActor, public IPGamePlayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPowerUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,Category="Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere,Category="Components")
	UStaticMeshComponent* MeshComp;


	UPROPERTY(EditDefaultsOnly,Category="Components")
	float ReSpawnTime;

	FTimerHandle TimerHandle_ReSpawn;

	UFUNCTION()
	void ShowPowerUp();
	void HideAndFreezePowerUp();
	void SetPowerUpState(bool bNewIsActive);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn) override;

};
