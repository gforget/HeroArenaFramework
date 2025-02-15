// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MP_BaseGameMode.h"
#include "MP_TeamDeathMatchGameMode.generated.h"

/**
 * 
 */
class UGameModeHUD;
class AMP_HeroPlayerController;
UCLASS()
class HEROARENAFRAMEWORK_API AMP_TeamDeathMatchGameMode : public AMP_BaseGameMode
{
	GENERATED_BODY()

public:
	AMP_TeamDeathMatchGameMode();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
};
