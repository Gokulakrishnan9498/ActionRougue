// Fill out your copyright notice in the Description page of Project Settings.


#include "PActionComponent.h"

#include "ActionRougue.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_MyProject);

UPActionComponent::UPActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}



void UPActionComponent::BeginPlay()
{
	Super::BeginPlay();

	//Server only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UPAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
	
	
}

void UPActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Stop all actions before ending play
	//This is important to ensure that any cleanup logic in the actions is executed
	TArray<UPAction*> ActionsCopy = Actions; // Copy to avoid modifying while iterating
	for (UPAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}
	Super::EndPlay(EndPlayReason);
}

void UPActionComponent::AddAction(AActor* Instigator, TSubclassOf<UPAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	//Skip for Clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to add action [class: %s]"), *GetNameSafe(ActionClass));
		return;
	}
	
	UPAction* NewAction = NewObject<UPAction>(GetOwner(),ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		
		Actions.Add(NewAction);
	}
	if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
	{
		NewAction->StartAction(Instigator);
	}
}

void UPActionComponent::RemoveAction(UPAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);
}

UPAction* UPActionComponent::GetAction(TSubclassOf<UPAction> ActionClass) const
{
	for (UPAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}
	return nullptr;
}


bool UPActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName)
	
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

			// Is Client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			//Bookmark for Unreal Insights
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));
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
				// Is Client?
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
				
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

void UPActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FString DebugMsg = GetNameSafe(GetOwner()) + ":" + ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	//Draw all Actions
	for (UPAction* Action : Actions)
	{
		FColor TxtColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),*GetNameSafe(GetOwner()),*GetNameSafe(Action));

		LogOnScreen(this, ActionMsg, TxtColor, 0.0f);
	}
}


void UPActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UPActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool UPActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool wroteSomething =  Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UPAction* Action : Actions)
	{
		if (Action)
		{
			wroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return wroteSomething;
}

void UPActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPActionComponent, Actions);
}
