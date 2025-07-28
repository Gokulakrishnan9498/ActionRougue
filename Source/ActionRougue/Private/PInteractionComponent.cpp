// Fill out your copyright notice in the Description page of Project Settings.


#include "PInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "PGamePlayInterface.h"
#include "PWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("pr.InteractionDebugDraw"),false,TEXT("Enables Debug lines for interaction Comp"),ECVF_Cheat);

// Sets default values for this component's properties
UPInteractionComponent::UPInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
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

	FindBestInteractable();
}

void UPInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	TArray<FHitResult> Hits;
	
	FVector EyeLocation;
	FRotator EyeRotation;
	
	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	FCollisionShape Shape;
	
	Shape.SetSphere(TraceRadius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation ,End , FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	
	for (FHitResult HitResult : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,TraceRadius,32,LineColor,false,2.0f);
		}
		
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UPGamePlayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}
	
	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UPWorldUserWidget>(GetWorld(),DefaultWidgetClass);
		}
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}		
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(),EyeLocation,End,LineColor,false,2.0f,0,2.0f);
	}
}


void UPInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to Interact.");
		return;
	}
	APawn* MyPawn = Cast<APawn>(GetOwner());
	IPGamePlayInterface::Execute_Interact(FocusedActor , MyPawn);
}

