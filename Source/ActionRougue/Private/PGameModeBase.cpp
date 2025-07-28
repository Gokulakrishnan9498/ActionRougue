// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameModeBase.h"

#include "EngineUtils.h"
#include "PAttributeComponent.h"
#include "PHeroCharacter.h"
#include "PHeroPlayerState.h"
#include "AI/PAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("pr.SpawnBots"),true,TEXT("Enable spawning of bots via timer."),ECVF_Cheat);

APGameModeBase::APGameModeBase()
{
	SpawnBotInterval = 2.0f;

	CreditsPerKill = 20;
	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = APHeroPlayerState::StaticClass();
}

void APGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot,this,&APGameModeBase::SpawnBotTimeElapsed,SpawnBotInterval,true);

	if (ensure(PowerUpClasses.Num()>0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,PowerUpSpawnQuery,this,EEnvQueryRunMode::AllMatching,nullptr);
		if (QueryInstance)
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&APGameModeBase::OnPowerUpSpawnQueryCompleted);
		}
	}
	
}


void APGameModeBase::KillAll()
{
	for(TActorIterator<APAICharacter> It(GetWorld()); It; ++It)
	{
		APAICharacter* Bot = *It;
		UPAttributeComponent* AttributeComp = UPAttributeComponent::GetAttributes(Bot);
		
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);//@fixMe: Pass in Player to get Kill Credits.
		}
	}
}

void APGameModeBase::SpawnBotTimeElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Warning,TEXT("Bot spawning is disabled via CVar 'CVarSpawnBots'."))
		return;
	}
	
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&APGameModeBase::OnBotSpawnQueryCompleted);	
	}
}

void APGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus)
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

void APGameModeBase::OnPowerUpSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("PowerUp Spawn Query Failed"));
		return;
	}
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points
		int32 RandomLocationIndex = FMath::RandRange(0,Locations.Num() -1);
		
		FVector PickedLocation = Locations[RandomLocationIndex];

		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		//Check minimum distance requirement
		bool bValidLocation = true;

		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();
			if (DistanceTo < RequiredPowerupDistance)
			{
				// Show skipped locations due to distance
				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

			// Failed the distance test
			if (!bValidLocation)
			{
				continue;
			}

			// Pick a random powerup-class
			int32 RandomClassIndex = FMath::RandRange(0,PowerUpClasses.Num()-1);
			TSubclassOf<AActor> RandomPowerUpClass 	= PowerUpClasses[RandomClassIndex];

		
			GetWorld()->SpawnActor<AActor>(RandomPowerUpClass,PickedLocation + FVector(0.0,0.0,30.0),FRotator::ZeroRotator);

			// Keep for distance checks
			UsedLocations.Add(PickedLocation);
			SpawnCounter++;
		
	}
}

void APGameModeBase::RespawnPlayerTimerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void APGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp,Log,TEXT("OnActorKilled , VictimActor : %s & Killer : %s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer));
	
	
	if (APHeroCharacter* Player = Cast<APHeroCharacter>(VictimActor))
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerTimerElapsed",Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate,RespawnDelay,false);
	}
	//UE_LOG(LogTemp, Log, TEXT("OnActorKilled Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

	//Give credits for kill
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		if (APHeroPlayerState* PS = KillerPawn->GetPlayerState<APHeroPlayerState>())
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}

