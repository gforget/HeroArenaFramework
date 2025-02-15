// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/BaseHeroPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "MP_HeroPlayerController.generated.h"

/**
 * 
 */

class UGameModeHUD;
class UPlayerHUD;
class AMP_HeroCharacter;

UCLASS()
class HEROARENAFRAMEWORK_API AMP_HeroPlayerController : public ABaseHeroPlayerController
{
	GENERATED_BODY()

public:
	AMP_HeroPlayerController();

	UFUNCTION(Client, Reliable)
	void CallOnPossess();

	UPROPERTY()
	AMP_HeroCharacter* AssignedShooterCharacter;
		
protected:
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	
private:
	
	void InstantiateHUD(APawn* InPawn);

	UPROPERTY()
	FTimerHandle DelayedPossessTimerHandle;

	void DelayedPossessCheck();
};
