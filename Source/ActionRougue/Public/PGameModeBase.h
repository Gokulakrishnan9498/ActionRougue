// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "PGameModeBase.generated.h"

/**
 * 
 */

class UEnvQuery;
class UCurveFloat;
class UEnvQueryInstanceBlueprintWrapper;
class UPSaveGame;
class UPMonsterDataAsset;
class UDataTable;

USTRUCT(Blueprintable)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	FMonsterInfoRow()
	{
	    Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;
	//TSubclassOf<AActor> MonsterClass;

	//Relative chance of this monster to spawn compared to others
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	//Points required by gamemode to spawn this monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	//Amount of credits rewarded to killer for killing this monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
	
	
};

UCLASS()
class ACTIONROUGUE_API APGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	FString SlotName;
	
	UPROPERTY()
	UPSaveGame* CurrentSaveGame;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Credits")
	int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="PowerUps")
	int32 DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="PowerUps")
	int32 RequiredPowerupDistance;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UDataTable* MonsterTable;
	
	//UPROPERTY(EditDefaultsOnly,Category="AI")
	//TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly,Category="PowerUps")
	TArray<TSubclassOf<AActor>> PowerUpClasses;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	float SpawnBotInterval;

	FTimerHandle TimerHandle_SpawnBot;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly,Category="PowerUps")
	UEnvQuery* PowerUpSpawnQuery;

	UFUNCTION()
	void SpawnBotTimeElapsed();

	UFUNCTION()
	void OnBotSpawnQueryCompleted( UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	UFUNCTION()
	void OnPowerUpSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerTimerElapsed(AController* Controller);

public:
	APGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	virtual void StartPlay() override;

	UFUNCTION()
	virtual void OnActorKilled(AActor* VictimActor,AActor* Killer);

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

	UFUNCTION(Exec)
	void KillAll();
	
};
