// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/BaseShooterPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "MP_ShooterPlayerController.generated.h"

/**
 * 
 */

class UGameModeHUD;
class UPlayerHUD;
class AMP_ShooterCharacter;

UCLASS()
class HEROARENAFRAMEWORK_API AMP_ShooterPlayerController : public ABaseShooterPlayerController
{
	GENERATED_BODY()

public:
	AMP_ShooterPlayerController();

	UFUNCTION(Client, Reliable)
	void CallOnPossess();

	UPROPERTY()
	AMP_ShooterCharacter* AssignedShooterCharacter;
		
protected:
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	
private:
	
	void InstantiateHUD(APawn* InPawn);

	UPROPERTY()
	FTimerHandle DelayedPossessTimerHandle;

	void DelayedPossessCheck();
};
