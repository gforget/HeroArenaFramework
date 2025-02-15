// Fill out your copyright notice in the Description page of Project Settings.
#include "Controllers/Multiplayer/MP_HeroAIController.h"

#include "Actors/BaseHeroCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Stimuli/TouchStimuli/HurtStimuli.h"

// Sets default values
AMP_HeroAIController::AMP_HeroAIController()
{

}

void AMP_HeroAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AIBehavior != nullptr && HasAuthority())
	{
		HurtStimuli = NewObject<UHurtStimuli>(GetTransientPackage(), UHurtStimuli::StaticClass());
		HurtStimuli->Initialize(this, Cast<ABaseHeroCharacter>(InPawn));
		
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), InPawn->GetActorLocation());
	}
}

