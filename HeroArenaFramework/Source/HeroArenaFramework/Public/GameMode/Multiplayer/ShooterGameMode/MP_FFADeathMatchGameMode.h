// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MP_ShooterGameMode.h"
#include "MP_FFADeathMatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API AMP_FFADeathMatchGameMode : public AMP_ShooterGameMode
{
	GENERATED_BODY()
	
public:
	AMP_FFADeathMatchGameMode();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};
