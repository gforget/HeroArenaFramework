﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTTask_Shoot.h"

#include "AIController.h"
#include "Actors/BaseHeroCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	ABaseHeroCharacter* Character = Cast<ABaseHeroCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	//Character->PullTrigger();
	return EBTNodeResult::Succeeded;
}
