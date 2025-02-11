// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SP_ShooterGameMode.h"
#include "SP_FFADeathMatchGameMode.generated.h"

/**
 * 
 */
class UGameModeHUD;
UCLASS()
class HEROARENAFRAMEWORK_API ASP_FFADeathMatchGameMode : public ASP_ShooterGameMode
{
	GENERATED_BODY()
	
public:
	ASP_FFADeathMatchGameMode();
	
	void OnShooterCharacterDeath(ABaseShooterCharacter* DeadShooterCharacter) override;
	void AddShooterCharacterCount(ABaseShooterCharacter* ShooterCharacterToRegister) override;

	UPROPERTY(BlueprintReadOnly)
	int NbShooterAlive;
	
protected:
	void BeginPlay();
	
private:
	void EndGame();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameModeHUD> GameModeHUDClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WinScreenClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoseScreenClass;
};


