// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/BaseShooterAIController.h"
#include "MP_ShooterAIController.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API AMP_ShooterAIController : public ABaseShooterAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_ShooterAIController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
