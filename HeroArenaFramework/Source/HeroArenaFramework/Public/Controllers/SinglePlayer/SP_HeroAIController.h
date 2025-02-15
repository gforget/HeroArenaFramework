// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/BaseHeroAIController.h"
#include "SP_HeroAIController.generated.h"

class UHurtStimuli;

UCLASS()
class HEROARENAFRAMEWORK_API ASP_HeroAIController : public ABaseHeroAIController
{
	GENERATED_BODY()

public:
	
	// Sets default values for this actor's properties
	ASP_HeroAIController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
