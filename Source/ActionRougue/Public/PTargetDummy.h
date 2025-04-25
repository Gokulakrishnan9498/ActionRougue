// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "PTargetDummy.generated.h"

class UPAttributeComponent;
UCLASS()
class ACTIONROUGUE_API APTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	APTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(VisibleAnywhere)
	UPAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UPAttributeComponent* OwningComp, float NewHealth, float Delta);
public:	


};
