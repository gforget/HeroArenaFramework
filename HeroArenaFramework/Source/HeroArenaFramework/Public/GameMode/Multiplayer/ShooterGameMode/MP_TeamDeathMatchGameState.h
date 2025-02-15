// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/TeamManager.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_BaseGameState.h"
#include "MP_TeamDeathMatchGameState.generated.h"

/**
 * 
 */
class UGameModeHUD;
UCLASS()
class HEROARENAFRAMEWORK_API AMP_TeamDeathMatchGameState : public AMP_BaseGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Wdiget Classes")
	TSubclassOf<UUserWidget> BlueWinScreenClass;

	UPROPERTY(EditDefaultsOnly, Category="Wdiget Classes")
	TSubclassOf<UUserWidget> RedWinScreenClass;
	
	void AddShooterCharacterCount(AMP_HeroCharacter* ShooterCharacterToRegister) override;
	void OnShooterCharacterDeath(ABaseHeroCharacter* DeadShooterCharacter) override;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<TEnumAsByte<ETeam>, int> TeamCount;

private:
	void EndGame(ETeam WinningTeam);
};
