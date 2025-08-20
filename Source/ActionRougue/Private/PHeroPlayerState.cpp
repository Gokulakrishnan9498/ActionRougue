// Fill out your copyright notice in the Description page of Project Settings.


#include "PHeroPlayerState.h"

#include "Net/UnrealNetwork.h"


void APHeroPlayerState::AddCredits(int32 Delta)
{
	// Avoid user-error of adding a negative amount or zero
	if (!ensure(Delta > 0.0f))
	{
		return;
	}
	Credits += Delta;

	OnCreditsChanged.Broadcast(this,Credits,Delta);
}

bool APHeroPlayerState::RemoveCredits(int32 Delta)
{
	//Avoid user-error of subtracting a negative amount ((-)+(-)=(+)Addition) or zero
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (Credits < Delta)
	{
		//Not enough credits
		return false;
	}

	Credits -= Delta;
	
	OnCreditsChanged.Broadcast(this,Credits,-Delta);

	return true;
}

void APHeroPlayerState::SavePlayerState_Implementation(UPSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}


void APHeroPlayerState::LoadPlayerState_Implementation(UPSaveGame* SaveObject)
{
	if (SaveObject)
	{
		//Credits = SaveObject->Credits;
		// Makes sure we trigger credits changed event
		AddCredits(SaveObject->Credits);
	}
}

void APHeroPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

// void APHeroPlayerState::MulticastCredits_Implementation(float NewCredits, float Delta)
// {
// 	OnCreditsChanged.Broadcast(this, NewCredits, Delta);
// }

int32 APHeroPlayerState::GetCredits() const
{
	return Credits;
}

void APHeroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APHeroPlayerState, Credits);
}