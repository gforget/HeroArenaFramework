// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SP_BaseGameMode.h"
#include "SP_TeamDeathMatchGameMode.generated.h"

/**
 * 
 */
class UGameModeHUD;
UCLASS()
class HEROARENAFRAMEWORK_API ASP_TeamDeathMatchGameMode : public ASP_BaseGameMode
{
	GENERATED_BODY()
	
public:
	ASP_TeamDeathMatchGameMode();
	
	void OnShooterCharacterDeath(ABaseHeroCharacter* DeadShooterCharacter) override;
	void AddShooterCharacterCount(ABaseHeroCharacter* ShooterCharacterToRegister) override;

	UPROPERTY(BlueprintReadOnly)
	TMap<TEnumAsByte<ETeam>, int> TeamCount;
	
protected:
	void BeginPlay();
	
private:
	void EndGame(ETeam TeamWin);

	ETeam PlayerTeam = ETeam::BlueTeam;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameModeHUD> GameModeHUDClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WinScreenClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoseScreenClass;
};
