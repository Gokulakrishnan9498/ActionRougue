// Fill out your copyright notice in the Description page of Project Settings.


#include "PExplosiveBarrel.h"

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
}

void APExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BarrelMesh->OnComponentHit.AddDynamic(this,&APExplosiveBarrel::OnBarrelHit);
}

void APExplosiveBarrel::OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		RadialForce->FireImpulse();
		
		UE_LOG(LogTemp,Log,TEXT("OnActorHit in Explosive Barrel"));
		UE_LOG(LogTemp,Warning,TEXT("Other actor : %s at Game time : %f"), *GetNameSafe(OtherActor), GetWorld()->GetTimeSeconds());
		FString CombinedString = FString::Printf(TEXT("Hit at Location %s"), *Hit.ImpactPoint.ToString());
		DrawDebugString(GetWorld(),Hit.ImpactPoint,CombinedString,nullptr,FColor::Green,2.0f,true);
	}
}


void APExplosiveBarrel::Explode()
{
	RadialForce->FireImpulse();
}

