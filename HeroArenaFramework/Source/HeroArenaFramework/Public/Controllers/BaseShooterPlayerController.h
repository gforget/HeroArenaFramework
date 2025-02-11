// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseShooterPlayerController.generated.h"

/**
 * 
 */

class ABaseShooterCharacter;
class UGameModeHUD;
class UPlayerHUD;

UCLASS()
class HEROARENAFRAMEWORK_API ABaseShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public :
	void InstantiateGameModeHUD(TSubclassOf<UGameModeHUD> GameModeHUDClass);

	virtual void GameOver(TSubclassOf<UUserWidget> EndScreenClass);
	void AddOHHealthBar(ABaseShooterCharacter* AssignedCharacter);
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUD> HUDScreenClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OHHealthBarClass;

	UPROPERTY()
	UPlayerHUD* PlayerHUD;
	
	UPROPERTY()
	UGameModeHUD* GameModeHUD;
};
