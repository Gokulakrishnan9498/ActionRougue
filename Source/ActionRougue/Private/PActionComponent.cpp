// Fill out your copyright notice in the Description page of Project Settings.


#include "PActionComponent.h"

UPActionComponent::UPActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UPActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for (TSubclassOf<UPAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
	
}

void UPActionComponent::AddAction(AActor* Instigator, TSubclassOf<UPAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	UPAction* NewAction = NewObject<UPAction>(this,ActionClass);
	if (NewAction)
	{
		Actions.Add(NewAction);
	}
	if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
	{
		NewAction->StartAction(Instigator);
	}
}

bool UPActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UPAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString failedMsg = FString::Printf(TEXT("Failed to run %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, failedMsg);
				continue;
			}
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UPActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UPAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

void UPActionComponent::RemoveAction(UPAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);
}


void UPActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + ":" + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
	
}

