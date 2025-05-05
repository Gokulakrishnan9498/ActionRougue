// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "PAttributeComponent.h"
#include "PWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
APAICharacter::APAICharacter()
{

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AttributeComp = CreateDefaultSubobject<UPAttributeComponent>(TEXT("AttributeComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";

}

void APAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this,&APAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this,&APAICharacter::OnHealthChanged);
}

void APAICharacter::OnHealthChanged(AActor* InstigatorActor, UPAttributeComponent* OwningComp, float NewHealth,float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UPWorldUserWidget>(GetWorld(),HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName , GetWorld()->TimeSeconds);
		if (NewHealth <= 0.0f)
		{
			//stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			//Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//Set Lifespan
			SetLifeSpan(10.0f);
		}
	}
}

void APAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor",NewTarget);
	}
}

void APAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);

	DrawDebugString(GetWorld(),GetActorLocation(),"PLAYER_SPOTTED",nullptr,FColor::White,4.0f,true);
}




