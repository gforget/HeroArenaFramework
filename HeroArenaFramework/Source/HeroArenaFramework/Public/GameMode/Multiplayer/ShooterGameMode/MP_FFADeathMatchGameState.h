// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_BaseGameState.h"
#include "MP_FFADeathMatchGameState.generated.h"

/**
 * 
 */
class UGameModeHUD;
UCLASS()
class HEROARENAFRAMEWORK_API AMP_FFADeathMatchGameState : public AMP_BaseGameState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category="Wdiget Classes")
	TSubclassOf<UUserWidget> PlayerWinScreenClass;

	UPROPERTY(EditDefaultsOnly, Category="Wdiget Classes")
	TSubclassOf<UUserWidget> PlayerLoseScreenClass;
	
	void AddShooterCharacterCount(AMP_HeroCharacter* ShooterCharacterToRegister) override;
	void OnShooterCharacterDeath(ABaseHeroCharacter* DeadShooterCharacter) override;
	
	UPROPERTY(BlueprintReadOnly)
	int ShooterCount;

private:
	void EndGame();
};
