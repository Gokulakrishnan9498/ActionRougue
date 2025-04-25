// Fill out your copyright notice in the Description page of Project Settings.


#include "PDashProjectile.h"
#include "PProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


APDashProjectile::APDashProjectile()
{
	DetonateDelay = 0.2f;
	TeleportDelay = 0.2f;
	ForceComp->InitialSpeed = 6000.0f;
}

void APDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate,this,&APDashProjectile::Explode,DetonateDelay);
}

void APDashProjectile::Explode_Implementation()
{
	//Super::Explode_Implementation();
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
	EffectComp->DeactivateSystem();
	ForceComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport,this,&APDashProjectile::TeleportInstigator,TeleportDelay);
	
}

void APDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(),ActorToTeleport->GetActorRotation(),false,false);
	}
	Destroy();
}
