// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/BaseHeroAIController.h"
#include "MP_HeroAIController.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API AMP_HeroAIController : public ABaseHeroAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_HeroAIController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
