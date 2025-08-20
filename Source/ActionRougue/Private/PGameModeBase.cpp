// Fill out your copyright notice in the Description page of Project Settings.


#include "PGameModeBase.h"

#include "ActionRougue.h"
#include "EngineUtils.h"
#include "PActionComponent.h"
#include "PAttributeComponent.h"
#include "PGamePlayInterface.h"
#include "PHeroCharacter.h"
#include "PHeroPlayerState.h"
#include "PMonsterDataAsset.h"
#include "PSaveGame.h"
#include "AI/PAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("pr.SpawnBots"),true,TEXT("Enable spawning of bots via timer."),ECVF_Cheat);

APGameModeBase::APGameModeBase()
{
	SpawnBotInterval = 2.0f;

	CreditsPerKill = 20;
	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = APHeroPlayerState::StaticClass();

	SlotName = "SaveGame_01";
}

void APGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0)
	{
		SlotName = SelectedSaveSlot;
	}

	LoadSaveGame();
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

void APGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	
	// Calling Before Super:: so we set variables before 'beginplayingstate' is called in PlayerController (which is where we instantiate UI)
	APHeroPlayerState* PS = NewPlayer->GetPlayerState<APHeroPlayerState>();
	
	if (ensure(PS))
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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

		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			//Get Random enemy
			int32 RandomIndex = FMath::RandRange(0,Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfInitialized();
			if (Manager)
			{
				LogOnScreen(this, "Loading Monster...", FColor::Green);
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &APGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}
		

		//Track all the used spawn locations
		DrawDebugSphere(GetWorld(),Locations[0],50.0f,20,FColor::Blue,false,60.0f);
	}
}

void APGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading monster", FColor::Green);
	UAssetManager* Manager = UAssetManager::GetIfInitialized();
	if (Manager)
	{
		UPMonsterDataAsset* MonsterData = Cast<UPMonsterDataAsset>(Manager->GetPrimaryAssetObject(LoadedId));

		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);

			if (NewBot)
			{
				LogOnScreen(this,FString::Printf(TEXT("Spawned enemy : %s at (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				UPActionComponent* ActionComp = Cast<UPActionComponent>(NewBot->GetComponentByClass(UPActionComponent::StaticClass()));
				if (ActionComp)
				{
					for (TSubclassOf<UPAction> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
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
	if (KillerPawn && KillerPawn != VictimActor)
	{
		// Only Players will have a 'PlayerState' instance, bots have nullptr
		if (APHeroPlayerState* PS = KillerPawn->GetPlayerState<APHeroPlayerState>())
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}

void APGameModeBase::WriteSaveGame()
{
	//Iterate all player states, we don't have proper ID to match yet (requires steam or EOS)
	for (int32 i=0; i < GameState->PlayerArray.Num(); i++)
	{
		APHeroPlayerState* PS = Cast<APHeroPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;//Only single player at this point
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	//Iterate the entire world of actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		//Only interested in our gameplay actors
		if (!Actor->Implements<UPGamePlayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();


		//Pass the array to fill with data from actor 
		FMemoryWriter MemWriter(ActorData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true); // true for persistent serialization
		
		//Finds variables only with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		
		//Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void APGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName , 0))
	{
		CurrentSaveGame = Cast<UPSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp,Log,TEXT("Failed to load save game data"));
			return;
		}
		UE_LOG(LogTemp,Log,TEXT("Loaded save game data"));

		//Iterate the entire world of actors
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UPGamePlayInterface>())
			{
				continue;
			}
			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					//Finds variables only with UPROPERTY(SaveGame)
					Ar.ArIsSaveGame = true;
					//Convert binary array back into actor variables
					Actor->Serialize(Ar);

					IPGamePlayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}


		}
	}
	else
	{
		CurrentSaveGame = Cast<UPSaveGame>(UGameplayStatics::CreateSaveGameObject(UPSaveGame::StaticClass()));

		UE_LOG(LogTemp,Log,TEXT("Created new save game data"));
	}
}

