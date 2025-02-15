// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/SinglePlayer/SP_HeroPlayerController.h"

#include "Actors/SinglePlayer/SP_HeroCharacter.h"
#include "Actors/SinglePlayer/SP_SpectatorPawn.h"
#include "Blueprint/UserWidget.h"
#include "UI/PlayerHUD.h"
#include "UI/SinglePlayer/SP_OHHealthBar.h"

ASP_HeroPlayerController::ASP_HeroPlayerController()
{
	bShouldPerformFullTickWhenPaused = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void ASP_HeroPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ASP_HeroPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (PlayerHUD == nullptr)
	{
		PlayerHUD = Cast<UPlayerHUD>(CreateWidget(this, HUDScreenClass));
		PlayerHUD->AddToViewport();
	}
		
	if (ASP_HeroCharacter* ShooterCharacter = Cast<ASP_HeroCharacter>(InPawn))
	{
		PlayerHUD->OnPlayerModeEvent();
	}

	if (ASP_SpectatorPawn* ShooterSpectator = Cast<ASP_SpectatorPawn>(InPawn))
	{
		PlayerHUD->OnSpectatorModeEvent();
	}
}

void ASP_HeroPlayerController::GameOver(TSubclassOf<UUserWidget> EndScreenClass)
{
	Super::GameOver(EndScreenClass);
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}