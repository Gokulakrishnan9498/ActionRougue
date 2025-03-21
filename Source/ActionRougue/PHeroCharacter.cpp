// Fill out your copyright notice in the Description page of Project Settings.


#include "PHeroCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APHeroCharacter::APHeroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	
	

}

// Called when the game starts or when spawned
void APHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APHeroCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() , Value);
}

void APHeroCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() , Value);
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

}

