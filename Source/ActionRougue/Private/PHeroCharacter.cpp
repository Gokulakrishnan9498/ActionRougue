// Fill out your copyright notice in the Description page of Project Settings.


#include "PHeroCharacter.h"

#include "PActionComponent.h"
#include "PAttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APHeroCharacter::APHeroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UPInteractionComponent>(TEXT("InteractionComp"));

	AttributeComp = CreateDefaultSubobject<UPAttributeComponent>(TEXT("AttributeComp"));

	ActionComp = CreateDefaultSubobject<UPActionComponent>(TEXT("ActionComp"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	AnimDelay = 0.2f;

	HandSocketName = "Muzzle_01";

	TimeToHitParamName = "TimeToHit";
	
}

void APHeroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this,&APHeroCharacter::OnHealthChanged);
}

FVector APHeroCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called when the game starts or when spawned
void APHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APHeroCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector() , Value);
}

void APHeroCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//X = Forward (Red) : Y = Right (Green) : Z = Up (Blue)
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector , Value);
}

void APHeroCharacter::StartSprint()
{
	ActionComp->StartActionByName(this,"Sprint");
}

void APHeroCharacter::StopSprint()
{
	ActionComp->StopActionByName(this,"Sprint");
}

void APHeroCharacter::StartAttackEffects()
{
	PlayAnimMontage(AttackAnim);
	UGameplayStatics::SpawnEmitterAttached(CastingEffect,GetMesh(),HandSocketName,FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::SnapToTarget);
}

void APHeroCharacter::PrimaryAttack()
{
	StartAttackEffects();
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this,&APHeroCharacter::PrimaryAttack_TimeElapsed,AnimDelay);
}

void APHeroCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void APHeroCharacter::DashAttack()
{
	StartAttackEffects();
	GetWorldTimerManager().SetTimer(TimerHandle_DashAttack,this,&APHeroCharacter::DashAttack_TimeElapsed,AnimDelay);
}

void APHeroCharacter::DashAttack_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void APHeroCharacter::BlackHoleAttack()
{
	StartAttackEffects();
	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack,this,&APHeroCharacter::BlackHoleAttack_TimeElapsed,AnimDelay);
}

void APHeroCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void APHeroCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void APHeroCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		//Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FVector TraceStart = CameraComp->GetComponentLocation();
		//EndPoint far into the look-at distance(not too far,still adjust somewhat towards crosshair on a miss)
		FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000.0f);

		FHitResult Hit;

		//returns if we get a blocking hit
		if (GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,ObjectQueryParams,Shape,Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		//find new direction/rotation from hand pointing to impact point in world
		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		FTransform SpawnTM = FTransform(ProjectileRotation,HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
	
		GetWorld()->SpawnActor<AActor>(ClassToSpawn,SpawnTM,SpawnParams);

		DrawDebugLine(GetWorld(),HandLocation,TraceEnd,FColor::Green,false,2.0f,0,2.0f);
	}
}


// Called every frame
void APHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&APHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&APHeroCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump" , IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&APHeroCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract",IE_Pressed,this,&APHeroCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("DashAttack",IE_Pressed,this,&APHeroCharacter::DashAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack",IE_Pressed,this,&APHeroCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&APHeroCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&APHeroCharacter::StopSprint);

}

void APHeroCharacter::HealSelf(float Amount /*=100*/)
{
	AttributeComp->ApplyHealthChange(this,Amount);
}

void APHeroCharacter::OnHealthChanged(AActor* InstigatorActor, UPAttributeComponent* OwningComp, float NewHealth,float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName,GetWorld()->TimeSeconds);
	}
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}
