// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTTask_ChooseHealthPack.h"

#include "AIController.h"
#include "Actors/BaseHealthPack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameMode.h"
#include "GameMode/SinglePlayer/SP_ShooterGameMode.h"

class AMP_ShooterGameMode;

UBTTask_ChooseHealthPack::UBTTask_ChooseHealthPack()
{
	NodeName = TEXT("Choose HealthPack");
}

EBTNodeResult::Type UBTTask_ChooseHealthPack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	TArray<ABaseHealthPack*> ConsideredHealthPacks = SP_GameMode!= nullptr ? SP_GameMode->GetAllHealthPacks() : MP_GameMode->GetAllHealthPacks() ;
	FVector CharLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(); 

	ConsideredHealthPacks.Sort([CharLocation](const ABaseHealthPack& A, const ABaseHealthPack& B) {
		return FVector::DistSquared(A.GetActorLocation(), CharLocation) < FVector::DistSquared(B.GetActorLocation(), CharLocation);
	});

	ABaseHealthPack* SelectedHealthPack = nullptr;
	for (int i=0; i<ConsideredHealthPacks.Num(); i++)
	{
		if (!ConsideredHealthPacks[i]->IsRecharging())
		{
			SelectedHealthPack = ConsideredHealthPacks[i];
			break;
		}
	}
	
	if (SelectedHealthPack == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), SelectedHealthPack);
	
	return EBTNodeResult::Succeeded;
}
