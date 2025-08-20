// Fill out your copyright notice in the Description page of Project Settings.


#include "PAction.h"

#include "ActionRougue.h"
#include "PActionComponent.h"
#include "Net/UnrealNetwork.h"

void UPAction::Initialize(UPActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UPAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	UPActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void UPAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Running : %s"),*GetNameSafe(this));

	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	UPActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantedTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void UPAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Stopped : %s"),*GetNameSafe(this));

	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	UPActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantedTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}


UWorld* UPAction::GetWorld() const
{
	// Outer is set when creating Action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}


UPActionComponent* UPAction::GetOwningComponent() const
{
	// AActor* Actor = Cast<AActor>(GetOuter());
	// return Actor->GetComponentByClass(UPActionComponent::StaticClass());

	return ActionComp;
}

void UPAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


bool UPAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UPAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPAction, RepData);
	DOREPLIFETIME(UPAction, TimeStarted);
	DOREPLIFETIME(UPAction, ActionComp);
}
