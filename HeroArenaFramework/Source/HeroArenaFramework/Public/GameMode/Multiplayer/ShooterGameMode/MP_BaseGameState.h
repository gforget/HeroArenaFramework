// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MP_BaseGameState.generated.h"

class UGameModeHUD;
class AMP_HeroCharacter;
class AMP_ShooterPlayerState;
/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API AMP_BaseGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Wdiget Classes")
	TSubclassOf<UGameModeHUD> GameModeHUDClass;
	
	UFUNCTION()
	virtual void OnShooterCharacterDeath(ABaseHeroCharacter* DeadShooterCharacter);
	
	virtual void AddShooterCharacterCount(AMP_HeroCharacter* ShooterCharacterToRegister);

	void RegisterEvent (AMP_HeroCharacter* ShooterCharacterRef);
	
};
