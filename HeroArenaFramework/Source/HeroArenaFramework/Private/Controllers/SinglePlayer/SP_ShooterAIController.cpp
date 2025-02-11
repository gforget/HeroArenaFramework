// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/SinglePlayer/SP_ShooterAIController.h"
#include "Actors/SinglePlayer/SP_ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Stimuli/TouchStimuli/HurtStimuli.h"

// Sets default values
ASP_ShooterAIController::ASP_ShooterAIController()
{
}

void ASP_ShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AIBehavior != nullptr)
	{
		HurtStimuli = NewObject<UHurtStimuli>(GetTransientPackage(), UHurtStimuli::StaticClass());
		HurtStimuli->Initialize(this, Cast<ABaseShooterCharacter>(InPawn));
		
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), InPawn->GetActorLocation());
	}
}