// Fill out your copyright notice in the Description page of Project Settings.


#include "PLever.h"

#include "PActionComponent.h"
#include "PExplosiveBarrel.h"

// Sets default values
APLever::APLever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	SetRootComponent(Box);

	Lever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever"));
	Lever->SetupAttachment(Box);

	LeverRot = 30.0f;

}

// Called when the game starts or when spawned
void APLever::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APLever::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!InstigatorPawn) return;

	if (UPActionComponent* ActionComp = InstigatorPawn->GetComponentByClass<UPActionComponent>())
	{
		ActionComp->ActiveGameplayTags.AddTag(KeyCard);
	}

	if (Lever)
	{
		Lever->SetRelativeRotation(FRotator(-LeverRot, 0, 0));
	}
	
}
	

