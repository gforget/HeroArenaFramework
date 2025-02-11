// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/SinglePlayer/SP_ShooterPlayerController.h"

#include "Actors/SinglePlayer/SP_ShooterCharacter.h"
#include "Actors/SinglePlayer/SP_ShooterSpectatorPawn.h"
#include "Blueprint/UserWidget.h"
#include "UI/PlayerHUD.h"
#include "UI/SinglePlayer/SP_OHHealthBar.h"

ASP_ShooterPlayerController::ASP_ShooterPlayerController()
{
	bShouldPerformFullTickWhenPaused = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void ASP_ShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ASP_ShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (PlayerHUD == nullptr)
	{
		PlayerHUD = Cast<UPlayerHUD>(CreateWidget(this, HUDScreenClass));
		PlayerHUD->AddToViewport();
	}
		
	if (ASP_ShooterCharacter* ShooterCharacter = Cast<ASP_ShooterCharacter>(InPawn))
	{
		PlayerHUD->OnPlayerModeEvent();
	}

	if (ASP_ShooterSpectatorPawn* ShooterSpectator = Cast<ASP_ShooterSpectatorPawn>(InPawn))
	{
		PlayerHUD->OnSpectatorModeEvent();
	}
}

void ASP_ShooterPlayerController::GameOver(TSubclassOf<UUserWidget> EndScreenClass)
{
	Super::GameOver(EndScreenClass);
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}