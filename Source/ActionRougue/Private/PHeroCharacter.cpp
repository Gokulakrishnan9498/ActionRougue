// Fill out your copyright notice in the Description page of Project Settings.


#include "PHeroCharacter.h"

#include "PActionComponent.h"
#include "PAttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

void APHeroCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this,"PrimaryAttack");
}

void APHeroCharacter::DashAttack()
{
	ActionComp->StartActionByName(this,"Dash");
}

void APHeroCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this,"BlackHole");
}

void APHeroCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
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
