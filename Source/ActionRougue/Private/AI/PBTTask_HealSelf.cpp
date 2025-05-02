// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PBTTask_HealSelf.h"

#include "AIController.h"
#include "PAttributeComponent.h"

EBTNodeResult::Type UPBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	UPAttributeComponent* AttributeComp = UPAttributeComponent::GetAttributes(AIPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(AIPawn,AttributeComp->GetMaxHealth());
	}

	return EBTNodeResult::Succeeded;
	
}
