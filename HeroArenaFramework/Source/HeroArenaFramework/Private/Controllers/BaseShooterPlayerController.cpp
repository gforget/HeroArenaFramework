// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BaseShooterPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "UI/BaseOHHealthBar.h"
#include "UI/GameModeHUD.h"
#include "UI/PlayerHUD.h"

void ABaseShooterPlayerController::InstantiateGameModeHUD(TSubclassOf<UGameModeHUD> GameModeHUDClass)
{
	GameModeHUD = Cast<UGameModeHUD>(CreateWidget(this, GameModeHUDClass));
	GameModeHUD->AddToViewport();
}

void ABaseShooterPlayerController::GameOver(TSubclassOf<UUserWidget> EndScreenClass)
{
	GameHasEnded(GetPawn());
	
	PlayerHUD->RemoveFromParent();
	GameModeHUD->RemoveFromParent();

	UUserWidget* EndScreenWidget = CreateWidget(this, EndScreenClass);
	if (EndScreenWidget != nullptr)
	{
		EndScreenWidget->AddToViewport();
	}
}

void ABaseShooterPlayerController::AddOHHealthBar(ABaseShooterCharacter* AssignedCharacter)
{
	UBaseOHHealthBar* OHHealthBar = Cast<UBaseOHHealthBar>(CreateWidget(this, OHHealthBarClass));
	OHHealthBar->AddToViewport();
	OHHealthBar->InitializeAssignedCharacterAndPlayerController(AssignedCharacter);
}
