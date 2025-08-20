// Fill out your copyright notice in the Description page of Project Settings.


#include "PHeroCharacter.h"

#include "PActionComponent.h"
#include "PAttributeComponent.h"
#include "PHeroPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PHeroPlayerController.h"

// Sets default values
APHeroCharacter::APHeroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	// We control the rotation of spring arm with pawn control rotation already, this disables a subtle side effect
	// where rotating our CapsuleComponent (eg. caused by bOrientRotationToMovement in Character Movement) will rotate our spring arm until it self corrects later in the update
	// This may cause unwanted effects when using CameraLocation during Tick as it may be slightly offset from our final camera position.
	SpringArmComp->SetUsingAbsoluteRotation(true);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UPInteractionComponent>(TEXT("InteractionComp"));

	AttributeComp = CreateDefaultSubobject<UPAttributeComponent>(TEXT("AttributeComp"));

	ActionComp = CreateDefaultSubobject<UPActionComponent>(TEXT("ActionComp"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	// Skip performing overlap queries on the Physics Asset after animation (7 queries in case of our Gideon mesh)
	GetMesh()->bUpdateOverlapsOnAnimationFinalize = false;
	
	// Enabled on mesh to react to incoming projectiles
	GetMesh()->SetGenerateOverlapEvents(true);
	// Disable on capsule collision to avoid double-dipping and receiving 2 overlaps when entering trigger zones etc.
	// Once from the mesh, and 2nd time from capsule
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

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

void APHeroCharacter::FindCrosshairTarget()
{
	// Ignore if not using GamePad
	APHeroPlayerController* PC = Cast<APHeroPlayerController>(GetController());

	// Only use aim assist when currently controlled and using gamepad
	// Note: you *may* always want to line trace if using this result for other things like coloring crosshair or re-using this hit data for aim adjusting during projectile attacks
	if (PC == nullptr || !PC->IsUsingGamepad())
	{
		bHasPawnTarget = false;
		return;
	}

	FVector EyeLocation;
	FRotator EyeRotation;
	GetActorEyesViewPoint(EyeLocation, EyeRotation);

	const float AimAssistDistance = 5000.f;
	const FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * AimAssistDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionShape Shape;
	Shape.SetSphere(50.f);

	// Called next frame when the trace has completed
	FTraceDelegate Delegate = FTraceDelegate::CreateUObject(this, &APHeroCharacter::CrosshairTraceComplete);
	
	TraceHandle = GetWorld()->AsyncSweepByChannel(EAsyncTraceType::Single, EyeLocation, TraceEnd, FQuat::Identity, ECC_Pawn, Shape, Params, FCollisionResponseParams::DefaultResponseParam, &Delegate);
}

void APHeroCharacter::CrosshairTraceComplete(const FTraceHandle& InTraceHandle, FTraceDatum& InTraceDatum)
{
	// at most expect one hit
	if (InTraceDatum.OutHits.IsValidIndex(0))
	{
		FHitResult Hit = InTraceDatum.OutHits[0];
		// Figure out if dealing with a Pawn, may want aim assist on other 'things', which requires a different check
		bHasPawnTarget = Hit.IsValidBlockingHit() && Hit.GetActor()->IsA(APawn::StaticClass());

		//UE_LOG(LogGame, Log, TEXT("has pawn target: %s"), bHasPawnTarget ? TEXT("true") : TEXT("false"));
	}
}

// Called when the game starts or when spawned
void APHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

/*void APHeroCharacter::MoveForward(float Value)
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
}*/

void APHeroCharacter::Move(const FInputActionInstance& Instance)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// Get value from input (combined value from WASD keys or single Gamepad stick) and convert to Vector (x,y)
	const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

	// Move forward/back
	AddMovementInput(ControlRot.Vector(), AxisValue.Y);

	// Move Right/Left strafe
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, AxisValue.X);

	// Alternative Copied from Lyra
	/*
	{
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}*/
}

void APHeroCharacter::LookMouse(const FInputActionInstance& InputValue)
{
	const FVector2D Value = InputValue.GetValue().Get<FVector2D>();
	
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void APHeroCharacter::LookStick(const FInputActionInstance& InputValue)
{
	FVector2D Value = InputValue.GetValue().Get<FVector2D>();

	// Track negative as we'll lose this during the conversion
	bool XNegative = Value.X < 0.f;
	bool YNegative = Value.Y < 0.f;

	// Can further modify with 'sensitivity' settings
	static const float LookYawRate = 100.0f;
	static const float LookPitchRate = 50.0f;

	// non-linear to make aiming a little easier
	Value = Value * Value;

	if (XNegative)
	{
		Value.X *= -1.f;
	}
	if (YNegative)
	{
		Value.Y *= -1.f;
	}

	// Aim assist
	// todo: may need to ease this out and/or change strength based on distance to target
	float RateMultiplier = 1.0f;
	if (bHasPawnTarget)
	{
		RateMultiplier = 0.5f;
	}

	AddControllerYawInput(Value.X * (LookYawRate * RateMultiplier) * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(Value.Y * (LookPitchRate * RateMultiplier) * GetWorld()->GetDeltaSeconds());
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

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	// Add mappings for our game, more complex games may have multiple Contexts that are added/removed at runtime
	Subsystem->AddMappingContext(DefaultInputMapping, 0);

	// New Enhanced Input system
	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	/*InputComp->BindAxis("MoveForward",this,&APHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&APHeroCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump" , IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&APHeroCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract",IE_Pressed,this,&APHeroCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("DashAttack",IE_Pressed,this,&APHeroCharacter::DashAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack",IE_Pressed,this,&APHeroCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&APHeroCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&APHeroCharacter::StopSprint);*/

	// General
	InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &APHeroCharacter::Move);
	InputComp->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &APHeroCharacter::Jump);
	InputComp->BindAction(Input_Interact, ETriggerEvent::Triggered, this, &APHeroCharacter::PrimaryInteract);

	// Sprint while key is held
	InputComp->BindAction(Input_Sprint, ETriggerEvent::Started, this, &APHeroCharacter::StartSprint);
	InputComp->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &APHeroCharacter::StopSprint);

	// MKB
	InputComp->BindAction(Input_LookMouse, ETriggerEvent::Triggered, this, &APHeroCharacter::LookMouse);
	// Gamepad
	InputComp->BindAction(Input_LookStick, ETriggerEvent::Triggered, this, &APHeroCharacter::LookStick);

	// Abilities
	InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &APHeroCharacter::PrimaryAttack);
	InputComp->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this, &APHeroCharacter::BlackHoleAttack);
	InputComp->BindAction(Input_Dash, ETriggerEvent::Triggered, this, &APHeroCharacter::DashAttack);

	// Special
	InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &APHeroCharacter::PrimaryAttack);

}

void APHeroCharacter::HealSelf(float Amount /*=100*/)
{
	AttributeComp->ApplyHealthChange(this,Amount);
}

void APHeroCharacter::OnHealthChanged(AActor* InstigatorActor, UPAttributeComponent* OwningComp, float NewHealth,float Delta)
{
	//Damaged
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName,GetWorld()->TimeSeconds);
		// Rage added equal to damage received (Abs to turn into positive rage number)
		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);
	}
	//Died
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		
		SetLifeSpan(5.0f); // Destroy after 5 seconds
	}
}
