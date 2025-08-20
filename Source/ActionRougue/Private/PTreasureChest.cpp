// Fill out your copyright notice in the Description page of Project Settings.


#include "PTreasureChest.h"

#include "Net/UnrealNetwork.h"

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

	bReplicates = true;

}


void APTreasureChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void APTreasureChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void APTreasureChest::OnRep_LidOpened()
{
	float CurrentPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
	UE_LOG(LogTemp, Warning, TEXT("OnRep_LidOpened: %s"), bLidOpened ? TEXT("Open") : TEXT("Closed"));
}

void APTreasureChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APTreasureChest, bLidOpened);
}


