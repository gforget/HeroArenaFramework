// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTService_SelectAmmoPack.h"

#include "AIController.h"
#include "Actors/BaseAmmoPack.h"
#include "Actors/BaseShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameMode.h"
#include "GameMode/SinglePlayer/SP_ShooterGameMode.h"
#include "Utility/NavMeshUtility.h"

UBTService_SelectAmmoPack::UBTService_SelectAmmoPack()
{
	NodeName = TEXT("Select Ammo Pack");
}

void UBTService_SelectAmmoPack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (CurrentWorldPtr == nullptr) CurrentWorldPtr = GetWorld();
	OwnerCompPtr = &OwnerComp;

	if (OwnerCompPtr != nullptr && CurrentWorldPtr != nullptr)
	{
		OwnerCompPtr->GetBlackboardComponent()->SetValueAsObject(FName("SelectedAmmoPack"), GetClosestAmmoPack());
	}
}

ABaseAmmoPack* UBTService_SelectAmmoPack::GetClosestAmmoPack()
{
	ABaseAmmoPack* SelectedAmmoPack = nullptr;
	
	//Since SP and MP GameMode do not inherit from the same base, we need to check both
	ASP_ShooterGameMode* SP_GameMode = GetWorld()->GetAuthGameMode<ASP_ShooterGameMode>();
	AMP_ShooterGameMode* MP_GameMode = GetWorld()->GetAuthGameMode<AMP_ShooterGameMode>();
	
	if (SP_GameMode == nullptr && MP_GameMode == nullptr)
	{
		return SelectedAmmoPack;
	}

	TArray<ABaseAmmoPack*> ConsideredAmmoPacks = SP_GameMode != nullptr ? SP_GameMode->GetAllAmmoPacks() : MP_GameMode->GetAllAmmoPacks();
	const FVector CharLocation = OwnerCompPtr->GetAIOwner()->GetPawn()->GetActorLocation(); 

	float HighestDistance = 999999999999.99f;
	
	for (int i=0; i<ConsideredAmmoPacks.Num(); i++)
	{
		if (!ConsideredAmmoPacks[i]->IsRecharging())
		{
			const ABaseShooterCharacter* AICharacter = Cast<ABaseShooterCharacter>(OwnerCompPtr->GetAIOwner()->GetPawn());
			const float CurrentDistance = AICharacter->NavMeshUtility->GetPathLength(CharLocation, ConsideredAmmoPacks[i]->GetActorLocation(), CurrentWorldPtr);
			if (CurrentDistance < HighestDistance)
			{
				SelectedAmmoPack = ConsideredAmmoPacks[i];
				HighestDistance = CurrentDistance;
			}
		}
	}

	return SelectedAmmoPack;
}