// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "PActionComponent.h"
#include "PAttributeComponent.h"
#include "PWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
APAICharacter::APAICharacter()
{

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AttributeComp = CreateDefaultSubobject<UPAttributeComponent>(TEXT("AttributeComp"));

	ActionComp = CreateDefaultSubobject<UPActionComponent>(TEXT("ActionComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Disabled on capsule to let projectiles pass through capsule and hit mesh instead
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
	TargetActorKey = "TargetActor";

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
		//Died
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

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

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
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey,NewTarget);
	}
}

AActor* APAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());

	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}
	return nullptr;
}

void APAICharacter::OnPawnSeen(APawn* Pawn)
{
	//Ignore if Target is already set
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);
		MulticastPawnSeen();	
	}
	// DrawDebugString(GetWorld(),GetActorLocation(),"PLAYER_SPOTTED",nullptr,FColor::White,4.0f,true);
}

void APAICharacter::MulticastPawnSeen_Implementation()
{
	UPWorldUserWidget* NewWidget = CreateWidget<UPWorldUserWidget>(GetWorld(),SpottedWidgetClass);
	if (NewWidget)
	{
		NewWidget->AttachedActor = this;

		// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
		// May end up behind the minion health bar otherwise.
		NewWidget->AddToViewport(10);
	}
}




