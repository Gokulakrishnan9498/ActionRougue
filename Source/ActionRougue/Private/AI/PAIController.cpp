// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void APAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! ,Please assign Behavior tree in AIController.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	
	

	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if (MyPawn != nullptr)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation",MyPawn->GetActorLocation());
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor",MyPawn);
	// }
}
