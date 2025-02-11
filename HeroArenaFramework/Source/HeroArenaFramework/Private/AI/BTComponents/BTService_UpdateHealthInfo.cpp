// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTService_UpdateHealthInfo.h"

#include "AIController.h"
#include "Actors/BaseShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateHealthInfo::UBTService_UpdateHealthInfo()
{
	NodeName = TEXT("Update Health Info");
}

void UBTService_UpdateHealthInfo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	OwnerCompPtr = &OwnerComp;
	if (OwnerCompPtr != nullptr)
	{
		OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("SelfHealthPercent"), GetHealthPercent());
	}
}

float UBTService_UpdateHealthInfo::GetHealthPercent()
{
	const ABaseShooterCharacter* AICharacter = Cast<ABaseShooterCharacter>(OwnerCompPtr->GetAIOwner()->GetPawn());
	if (AICharacter == nullptr)
	{
		return 0.0f;
	}
	
	return AICharacter->GetHealthPercent();
}
