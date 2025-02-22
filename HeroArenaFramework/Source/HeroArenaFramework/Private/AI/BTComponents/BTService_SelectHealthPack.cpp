﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTService_SelectHealthPack.h"

#include "AIController.h"
#include "Actors/BaseHealthPack.h"
#include "Actors/BaseHeroCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_BaseGameMode.h"
#include "GameMode/SinglePlayer/SP_BaseGameMode.h"
#include "Utility/NavMeshUtility.h"

UBTService_SelectHealthPack::UBTService_SelectHealthPack()
{
	NodeName = TEXT("Select Health Pack");
}

void UBTService_SelectHealthPack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (CurrentWorldPtr == nullptr) CurrentWorldPtr = GetWorld();
	OwnerCompPtr = &OwnerComp;

	if (OwnerCompPtr != nullptr && CurrentWorldPtr != nullptr)
	{
		OwnerCompPtr->GetBlackboardComponent()->SetValueAsObject(FName("SelectedHealthPack"), GetClosestHealthPack());
	}
}

ABaseHealthPack* UBTService_SelectHealthPack::GetClosestHealthPack()
{
	ABaseHealthPack* SelectedHealthPack = nullptr;
	
	//Since SP and MP GameMode do not inherit from the same base, we need to check both
	ASP_BaseGameMode* SP_GameMode = GetWorld()->GetAuthGameMode<ASP_BaseGameMode>();
	AMP_BaseGameMode* MP_GameMode = GetWorld()->GetAuthGameMode<AMP_BaseGameMode>();

	if (SP_GameMode == nullptr && MP_GameMode == nullptr)
	{
		return nullptr;
	}

	TArray<ABaseHealthPack*> ConsideredHealthPacks = SP_GameMode != nullptr ? SP_GameMode->GetAllHealthPacks() : MP_GameMode->GetAllHealthPacks();
	const FVector CharLocation = OwnerCompPtr->GetAIOwner()->GetPawn()->GetActorLocation(); 

	float HighestDistance = 999999999999.99f;
	
	for (int i=0; i<ConsideredHealthPacks.Num(); i++)
	{
		if (!ConsideredHealthPacks[i]->IsRecharging())
		{
			const ABaseHeroCharacter* AICharacter = Cast<ABaseHeroCharacter>(OwnerCompPtr->GetAIOwner()->GetPawn());
			const float CurrentDistance = AICharacter->NavMeshUtility->GetPathLength(CharLocation, ConsideredHealthPacks[i]->GetActorLocation(), CurrentWorldPtr);
			
			if (CurrentDistance < HighestDistance)
			{
				SelectedHealthPack = ConsideredHealthPacks[i];
				HighestDistance = CurrentDistance;
			}
		}
	}

	return SelectedHealthPack;
}