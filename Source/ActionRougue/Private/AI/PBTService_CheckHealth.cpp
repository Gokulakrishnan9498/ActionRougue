// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PBTService_CheckHealth.h"

#include "AIController.h"
#include "PAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


UPBTService_CheckHealth::UPBTService_CheckHealth()
{
	LowHealthFraction = 0.3f;
}


void UPBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
		APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
		if (ensure(AIPawn))
		{
			UPAttributeComponent* AttributeComp = Cast<UPAttributeComponent>(AIPawn->GetComponentByClass(UPAttributeComponent::StaticClass()));
			if (ensure(AttributeComp))
			{
				bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetMaxHealth()) < LowHealthFraction;

				UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
				BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName , bLowHealth);
			}
		}
	
}


