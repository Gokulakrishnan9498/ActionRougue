// Fill out your copyright notice in the Description page of Project Settings.


#include "PBlackHoleProjectile.h"

#include "PHeroCharacter.h"
#include "PProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"



APBlackHoleProjectile::APBlackHoleProjectile()
{
	SphereComp->SetSphereRadius(60.0f);
	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(RootComponent);
	RadialForce->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	RadialForce->Radius = 1000.0f;
	RadialForce->ForceStrength = -20000000.0f;

	ForceComp->InitialSpeed = 4000.0f;

	InitialLifeSpan = 4.8f;
	//SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//SphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
}

void APBlackHoleProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&APBlackHoleProjectile::BlackHoleOverlap);
}

void APBlackHoleProjectile::BlackHoleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("OTHER Actor is %s"), *GetNameSafe(OtherActor));
	
	if (OtherComp->IsSimulatingPhysics())
	{
		if (OtherActor != GetInstigator())
		{
			UE_LOG(LogTemp,Warning,TEXT("OTHER Actor is %s"), *GetNameSafe(OtherActor));
			OtherActor->Destroy();
		}
		
	}
}
	

