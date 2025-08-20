// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PInteractionComponent.generated.h"


class UPWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGUE_API UPInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPInteractionComponent();

	void PrimaryInteract();

protected:
	//Reliable - the function will always be called on the server (or) will always arrive, eventually . Request will be resent unless an acknowledgement was recieved.
	//UnReliable - the function may not be called on the server (or) Not guaranteed, packet can get lost and won't worry.
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	

	void FindBestInteractable();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<UPWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UPWorldUserWidget* DefaultWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
