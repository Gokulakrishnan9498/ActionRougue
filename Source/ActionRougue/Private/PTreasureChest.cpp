// Fill out your copyright notice in the Description page of Project Settings.


#include "PTreasureChest.h"

// Sets default values
APTreasureChest::APTreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;

}

void APTreasureChest::Interact_Implementation(APawn* InstigatorPawn)
{
	//IPGamePlayInterface::Interact_Implementation(InstigatorPawn);
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}

// Called when the game starts or when spawned
void APTreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APTreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

