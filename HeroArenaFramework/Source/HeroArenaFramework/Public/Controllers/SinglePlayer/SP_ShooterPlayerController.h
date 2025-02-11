// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/BaseShooterPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "SP_ShooterPlayerController.generated.h"

/**
 * 
 */
class ASP_ShooterCharacter;
class UPlayerHUD;
class UGameModeHUD;

UCLASS()
class HEROARENAFRAMEWORK_API ASP_ShooterPlayerController : public ABaseShooterPlayerController
{
	GENERATED_BODY()
	
public:
	ASP_ShooterPlayerController();
	
	virtual void GameOver(TSubclassOf<UUserWidget> EndScreenClass) override;

protected:
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	
	FTimerHandle RestartTimer;
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.0f;
	
};
