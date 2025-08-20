// Fill out your copyright notice in the Description page of Project Settings.


#include "PAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UPAction_ProjectileAttack::UPAction_ProjectileAttack()
{
	AnimDelay = 0.2f;

	HandSocketName = "Muzzle_01";
}

void UPAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect,Character->GetMesh(),HandSocketName,FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::SnapToTarget);

		if (Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this,"AttackDelay_Elapsed",Character);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate,AnimDelay,false);
		}
	}
}

void UPAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		//Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		//EndPoint far into the look-at distance (not too far,still adjust somewhat towards crosshair on a miss)
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000.0f);

		FHitResult Hit;

		//returns if we get a blocking hit
		if (GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,ObjectQueryParams,Shape,Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		//find new direction/rotation from hand pointing to impact point in 'world'
		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		FTransform SpawnTM = FTransform(ProjectileRotation,HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
	
		GetWorld()->SpawnActor<AActor>(ProjectileClass,SpawnTM,SpawnParams);

		//DrawDebugLine(GetWorld(),HandLocation,TraceEnd,FColor::Green,false,2.0f,0,2.0f);
	}

	StopAction(InstigatorCharacter);
	
}