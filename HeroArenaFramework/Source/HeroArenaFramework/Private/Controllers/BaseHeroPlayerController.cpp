// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BaseHeroPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "UI/BaseOHHealthBar.h"
#include "UI/GameModeHUD.h"
#include "UI/PlayerHUD.h"

void ABaseHeroPlayerController::InstantiateGameModeHUD(TSubclassOf<UGameModeHUD> GameModeHUDClass)
{
	GameModeHUD = Cast<UGameModeHUD>(CreateWidget(this, GameModeHUDClass));
	GameModeHUD->AddToViewport();
}

void ABaseHeroPlayerController::GameOver(TSubclassOf<UUserWidget> EndScreenClass)
{
	GameHasEnded(GetPawn());
	
	CharacterHUD->RemoveFromParent();
	GameModeHUD->RemoveFromParent();

	UUserWidget* EndScreenWidget = CreateWidget(this, EndScreenClass);
	if (EndScreenWidget != nullptr)
	{
		EndScreenWidget->AddToViewport();
	}
}

void ABaseHeroPlayerController::AddOHHealthBar(ABaseHeroCharacter* AssignedCharacter)
{
	UBaseOHHealthBar* OHHealthBar = Cast<UBaseOHHealthBar>(CreateWidget(this, OHHealthBarClass));
	OHHealthBar->AddToViewport();
	OHHealthBar->InitializeAssignedCharacterAndPlayerController(AssignedCharacter);
}
