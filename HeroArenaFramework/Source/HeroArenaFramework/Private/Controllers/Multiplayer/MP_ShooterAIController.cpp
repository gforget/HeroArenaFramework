// Fill out your copyright notice in the Description page of Project Settings.
#include "Controllers/Multiplayer/MP_ShooterAIController.h"

#include "Actors/BaseShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Stimuli/TouchStimuli/HurtStimuli.h"

// Sets default values
AMP_ShooterAIController::AMP_ShooterAIController()
{

}

void AMP_ShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AIBehavior != nullptr && HasAuthority())
	{
		HurtStimuli = NewObject<UHurtStimuli>(GetTransientPackage(), UHurtStimuli::StaticClass());
		HurtStimuli->Initialize(this, Cast<ABaseShooterCharacter>(InPawn));
		
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), InPawn->GetActorLocation());
	}
}

