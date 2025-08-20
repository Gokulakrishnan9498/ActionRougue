// Fill out your copyright notice in the Description page of Project Settings.


#include "PHeroPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void APHeroPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		// Single-player only
		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, false);
		}
		
		return;
	}
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
		bShowMouseCursor = true;
		//SetInputMode(FInputModeUIOnly());

		
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(PauseMenuInstance->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);

		// Single-player only
		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, true);
		}
	}
}

void APHeroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//InputComponent->BindAction("PauseMenu", IE_Pressed, this, &APHeroPlayerController::TogglePauseMenu);
	//InputComponent->BindAction("AnyKey", IE_Pressed, this, &APHeroPlayerController::AnyKeyInput);

	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(PauseMenu, ETriggerEvent::Started, this, &APHeroPlayerController::TogglePauseMenu);
		EnhancedInput->BindAction(AnyKey, ETriggerEvent::Started, this, &APHeroPlayerController::AnyKeyInput);
	}
}

void APHeroPlayerController::AnyKeyInput(const FInputActionValue& Value)
{
	bIsUsingGamepad = Value.Get<bool>();
}

void APHeroPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void APHeroPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void APHeroPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateReceived.Broadcast(PlayerState);
}
