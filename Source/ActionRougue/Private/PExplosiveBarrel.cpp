// Fill out your copyright notice in the Description page of Project Settings.


#include "PExplosiveBarrel.h"

#include "PAttributeComponent.h"

// Sets default values
APExplosiveBarrel::APExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
	SetRootComponent(BarrelMesh);
	BarrelMesh->SetSimulatePhysics(true);
	BarrelMesh->SetCollisionProfileName("PhysicsActor");
	

	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(BarrelMesh);

	RadialForce->bIgnoreOwningActor = false;
	RadialForce->Radius = 750.0f;
	RadialForce->ImpulseStrength = 1500.0f;
	RadialForce->bImpulseVelChange = true;
	RadialForce->SetAutoActivate(false);
	RadialForce->AddCollisionChannelToAffect(ECC_WorldDynamic);

	bReplicates = true;
	
}




void APExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//if (BarrelMesh)
	//{
	BarrelMesh->OnComponentHit.RemoveDynamic(this, &APExplosiveBarrel::OnBarrelHit);
		BarrelMesh->OnComponentHit.AddDynamic(this, &APExplosiveBarrel::OnBarrelHit);
	//}
}

void APExplosiveBarrel::OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && DamagedActor == nullptr)
	{
		DamagedActor = OtherActor;
		RadialForce->FireImpulse();
		

		// if (DamagedActor == OtherActor)
		// {
		// 	// Already damaged this actor
		// 	return;
		// }
		
		

		//DamagedActor.Add(OtherActor);tried using TSet<AActor*>

		UPAttributeComponent* AttributeComp = UPAttributeComponent::GetAttributes(DamagedActor);
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(GetInstigator(),-50.0f);
		}

		SetLifeSpan(7.0f);

		MultiCastExplode();
		
		UE_LOG(LogTemp,Log,TEXT("OnActorHit in Explosive Barrel"));
		UE_LOG(LogTemp,Warning,TEXT("Other actor : %s at Game time : %f"), *GetNameSafe(OtherActor), GetWorld()->GetTimeSeconds());
		FString CombinedString = FString::Printf(TEXT("Hit at Location %s"), *Hit.ImpactPoint.ToString());
		DrawDebugString(GetWorld(),Hit.ImpactPoint,CombinedString,nullptr,FColor::Green,2.0f,true);
	}
}
 void APExplosiveBarrel::Explode()
 {
 	RadialForce->FireImpulse();
 	MultiCastExplode();
 }

void APExplosiveBarrel::MultiCastExplode_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}



