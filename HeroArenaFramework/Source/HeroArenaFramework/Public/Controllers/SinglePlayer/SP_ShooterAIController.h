// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/BaseShooterAIController.h"
#include "SP_ShooterAIController.generated.h"

class UHurtStimuli;

UCLASS()
class HEROARENAFRAMEWORK_API ASP_ShooterAIController : public ABaseShooterAIController
{
	GENERATED_BODY()

public:
	
	// Sets default values for this actor's properties
	ASP_ShooterAIController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
