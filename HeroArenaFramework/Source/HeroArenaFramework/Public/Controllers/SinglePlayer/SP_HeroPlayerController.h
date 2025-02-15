// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/BaseHeroPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "SP_HeroPlayerController.generated.h"

/**
 * 
 */
class ASP_HeroCharacter;
class UPlayerHUD;
class UGameModeHUD;

UCLASS()
class HEROARENAFRAMEWORK_API ASP_HeroPlayerController : public ABaseHeroPlayerController
{
	GENERATED_BODY()
	
public:
	ASP_HeroPlayerController();
	
	virtual void GameOver(TSubclassOf<UUserWidget> EndScreenClass) override;

protected:
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	
	FTimerHandle RestartTimer;
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.0f;
	
};
