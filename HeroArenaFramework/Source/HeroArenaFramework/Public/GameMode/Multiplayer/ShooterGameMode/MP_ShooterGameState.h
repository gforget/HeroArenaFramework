// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MP_ShooterGameState.generated.h"

class UGameModeHUD;
class AMP_ShooterCharacter;
class AMP_ShooterPlayerState;
/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API AMP_ShooterGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Wdiget Classes")
	TSubclassOf<UGameModeHUD> GameModeHUDClass;
	
	UFUNCTION()
	virtual void OnShooterCharacterDeath(ABaseShooterCharacter* DeadShooterCharacter);
	
	virtual void AddShooterCharacterCount(AMP_ShooterCharacter* ShooterCharacterToRegister);

	void RegisterEvent (AMP_ShooterCharacter* ShooterCharacterRef);
	
};
