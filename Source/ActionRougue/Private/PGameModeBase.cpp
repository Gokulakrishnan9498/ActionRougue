// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameModeBase.h"

#include "EngineUtils.h"
#include "PAttributeComponent.h"
#include "AI/PAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"


APGameModeBase::APGameModeBase()
{
	SpawnBotInterval = 2.0f;
}

void APGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot,this,&APGameModeBase::SpawnBotTimeElapsed,SpawnBotInterval,true);
}

void APGameModeBase::SpawnBotTimeElapsed()
{
	int32 NumOfAliveBots = 0;
	for(TActorIterator<APAICharacter> It(GetWorld()); It; ++It)
	{
		APAICharacter* Bot = *It;
		UPAttributeComponent* AttributeComp = UPAttributeComponent::GetAttributes(Bot);
		
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	UE_LOG(LogTemp,Log,TEXT("Found %i alive bots"),NumOfAliveBots);

	float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NumOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp,Warning,TEXT("At maximum bot capacity , skipping bot spawn"));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,SpawnBotQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&APGameModeBase::OnQueryCompleted);	
	}
}

void APGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn Bot Query Failed"));
		return;
	}


	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator);

		//Track all the used spawn locations
		DrawDebugSphere(GetWorld(),Locations[0],50.0f,20,FColor::Blue,false,60.0f);
	}
}
