// Fill out your copyright notice in the Description page of Project Settings.


#include "PPowerUpActor.h"

#include "Components/SphereComponent.h"

// Sets default values
APPowerUpActor::APPowerUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionProfileName("PowerUp");

	ReSpawnTime = 10.0f;

}

// Called when the game starts or when spawned
void APPowerUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPowerUpActor::ShowPowerUp()
{
	SetPowerUpState(true);
}

void APPowerUpActor::HideAndFreezePowerUp()
{
	SetPowerUpState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_ReSpawn,this,&APPowerUpActor::ShowPowerUp,ReSpawnTime);
}

void APPowerUpActor::SetPowerUpState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	//Set visibility on root and all children
	RootComponent->SetVisibility(bNewIsActive,true);
}

// Called every frame
void APPowerUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APPowerUpActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//Logic in derived classes
}

