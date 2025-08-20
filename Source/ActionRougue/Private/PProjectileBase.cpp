// Fill out your copyright notice in the Description page of Project Settings.


#include "PProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
APProjectileBase::APProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionProfileName(TEXT("Projectile"));
	SphereComp->OnComponentHit.AddDynamic(this,&APProjectileBase::OnActorHit);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
	EffectComp->SetupAttachment(SphereComp);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	ForceComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ForceComp"));
	ForceComp->bRotationFollowsVelocity = true;
	ForceComp->ProjectileGravityScale = 0.0f;
	ForceComp->bInitialVelocityInLocalSpace = true;
	ForceComp->InitialSpeed = 8000.0f;

	ImpactShakeInnerRadius = 250.0f;
	ImpactShakeOuterRadius = 2500.0f;

	bReplicates = true;
	
}

void APProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void APProjectileBase::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
		UGameplayStatics::SpawnSoundAtLocation(this,ImpactSound,GetActorLocation(),GetActorRotation());
		UGameplayStatics::PlayWorldCameraShake(this,ImpactShake,GetActorLocation(),ImpactShakeInnerRadius,ImpactShakeOuterRadius);
		
		EffectComp->DeactivateSystem();
		ForceComp->StopMovementImmediately();
		SetActorEnableCollision(false);
		Destroy();
	}
}

// Called when the game starts or when spawned
void APProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void APProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

