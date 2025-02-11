// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTTask_ChooseAmmoPack.h"

#include "AIController.h"
#include "Actors/BaseAmmoPack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameMode.h"
#include "GameMode/SinglePlayer/SP_ShooterGameMode.h"

UBTTask_ChooseAmmoPack::UBTTask_ChooseAmmoPack()
{
	NodeName = TEXT("Choose AmmoPack");
}

EBTNodeResult::Type UBTTask_ChooseAmmoPack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	//Since SP and MP GameMode do not inherit from the same base, we need to check both
	ASP_ShooterGameMode* SP_GameMode = GetWorld()->GetAuthGameMode<ASP_ShooterGameMode>();
	AMP_ShooterGameMode* MP_GameMode = GetWorld()->GetAuthGameMode<AMP_ShooterGameMode>();

	if (SP_GameMode == nullptr && MP_GameMode == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	TArray<ABaseAmmoPack*> ConsideredAmmoPacks = SP_GameMode != nullptr ? SP_GameMode->GetAllAmmoPacks() : MP_GameMode->GetAllAmmoPacks();
	FVector CharLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(); 

	ConsideredAmmoPacks.Sort([CharLocation](const ABaseAmmoPack& A, const ABaseAmmoPack& B) {
		return FVector::DistSquared(A.GetActorLocation(), CharLocation) < FVector::DistSquared(B.GetActorLocation(), CharLocation);
	});

	ABaseAmmoPack* SelectedAmmoPack = nullptr;
	for (int i=0; i<ConsideredAmmoPacks.Num(); i++)
	{
		if (!ConsideredAmmoPacks[i]->IsRecharging())
		{
			SelectedAmmoPack = ConsideredAmmoPacks[i];
			break;
		}
	}
	
	if (SelectedAmmoPack == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), SelectedAmmoPack);
	
	return EBTNodeResult::Succeeded;
}
