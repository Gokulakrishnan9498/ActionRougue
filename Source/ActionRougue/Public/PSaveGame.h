// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PSaveGame.generated.h"

/**
 * 
 */

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()
public:
	//Identifier for which actor this data belongs to
	UPROPERTY()
	FString ActorName;

	//For movable actors, we save their transform(location, rotation, scale)
	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;
};
UCLASS()
class ACTIONROUGUE_API UPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
	
};
