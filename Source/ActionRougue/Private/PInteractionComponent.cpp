// Fill out your copyright notice in the Description page of Project Settings.


#include "PInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "PGamePlayInterface.h"

// Sets default values for this component's properties
UPInteractionComponent::UPInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPInteractionComponent::PrimaryInteract()
{
	TArray<FHitResult> Hits;
	
	FVector EyeLocation;
	FRotator EyeRotation;
	
	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionShape Shape;
	float Radius = 30.0f;
	Shape.SetSphere(Radius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation ,End , FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult HitResult : Hits)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UPGamePlayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				IPGamePlayInterface::Execute_Interact(HitActor , MyPawn);
				break;
			}
		}
		DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,Radius,32,LineColor,false,2.0f);
	}
	DrawDebugLine(GetWorld(),EyeLocation,End,LineColor,false,2.0f,0,2.0f);
}


// Called when the game starts
void UPInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

