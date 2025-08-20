// Fill out your copyright notice in the Description page of Project Settings.


#include "PPowerUpActor.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APPowerUpActor::APPowerUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionProfileName("PowerUp");

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ReSpawnTime = 10.0f;
	bIsActive = true; //Start as active

	bReplicates = true;

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
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void APPowerUpActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	//Set visibility on root and all children
	RootComponent->SetVisibility(bIsActive,true);
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

FText APPowerUpActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty()	;
}

void APPowerUpActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APPowerUpActor, bIsActive);
}
