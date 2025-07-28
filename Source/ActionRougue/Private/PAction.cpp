// Fill out your copyright notice in the Description page of Project Settings.


#include "PAction.h"
#include "PActionComponent.h"


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
	
	UPActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantedTags);

	bIsRunning = true;
}

void UPAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Stopped : %s"),*GetNameSafe(this));

	ensureAlways(bIsRunning);

	UPActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantedTags);

	bIsRunning = false;
}


UWorld* UPAction::GetWorld() const
{
	// Outer is set when creating Action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}


UPActionComponent* UPAction::GetOwningComponent() const
{
	return Cast<UPActionComponent>(GetOuter());
}

bool UPAction::IsRunning() const
{
	return bIsRunning;
}


