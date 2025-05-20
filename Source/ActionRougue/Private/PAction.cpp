// Fill out your copyright notice in the Description page of Project Settings.


#include "PAction.h"

void UPAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Running : %s"),*GetNameSafe(this));
}

void UPAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Stopped : %s"),*GetNameSafe(this));
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
