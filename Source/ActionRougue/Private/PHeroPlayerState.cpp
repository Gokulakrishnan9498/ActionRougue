// Fill out your copyright notice in the Description page of Project Settings.


#include "PHeroPlayerState.h"



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

int32 APHeroPlayerState::GetCredits() const
{
	return Credits;
}