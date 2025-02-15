// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/SinglePlayer/SP_HeroAIController.h"
#include "Actors/SinglePlayer/SP_HeroCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Stimuli/TouchStimuli/HurtStimuli.h"

// Sets default values
ASP_HeroAIController::ASP_HeroAIController()
{
}

void ASP_HeroAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AIBehavior != nullptr)
	{
		HurtStimuli = NewObject<UHurtStimuli>(GetTransientPackage(), UHurtStimuli::StaticClass());
		HurtStimuli->Initialize(this, Cast<ABaseHeroCharacter>(InPawn));
		
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), InPawn->GetActorLocation());
	}
}