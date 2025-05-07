// Fill out your copyright notice in the Description page of Project Settings.


#include "PMagicProjectile.h"

#include "PAttributeComponent.h"
#include "PGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
APMagicProjectile::APMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionProfileName(TEXT("Projectile"));
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&APMagicProjectile::OnActorOverlap);
	SphereComp->OnComponentHit.AddDynamic(this,&APMagicProjectile::OnActorHit);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(SphereComp);

	MovementComp->InitialSpeed = 3000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	DamageAmount = 20.0f;

	ImpactShakeInnerRadius = 250.0f;
	ImpactShakeOuterRadius = 2500.0f;

	InitialLifeSpan = 10.0f;

}

// Called when the game starts or when spawned
void APMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	AActor* InstigatorActor = GetInstigator();
	if (InstigatorActor)
	{
		SphereComp->IgnoreActorWhenMoving(InstigatorActor,true);	
	}
}

void APMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		// UPAttributeComponent* AttributeComp =UPAttributeComponent::GetAttributes(OtherActor);
		// if (AttributeComp)
		// {
		// 	AttributeComp->ApplyHealthChange(GetInstigator(),-DamageAmount);
		// 	
		// 	Destroy();
		// }
		if (UPGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(),OtherActor,DamageAmount,SweepResult))
		{
			Destroy();
		}
		
	}
	
}

void APMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),GetActorRotation());
		UGameplayStatics::PlayWorldCameraShake(this,ImpactShake,GetActorLocation(),ImpactShakeInnerRadius,ImpactShakeOuterRadius);

		EffectComp->DeactivateSystem();
		MovementComp->StopMovementImmediately();
		SetActorEnableCollision(false);
		Destroy();
	}
	
}

// Called every frame
void APMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

