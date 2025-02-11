// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTTask_ChooseWaypoint.h"

#include "Actors/BaseWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameMode.h"
#include "GameMode/SinglePlayer/SP_ShooterGameMode.h"

UBTTask_ChooseWaypoint::UBTTask_ChooseWaypoint()
{
	NodeName = TEXT("Choose Waypoint");
}

EBTNodeResult::Type UBTTask_ChooseWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ASP_ShooterGameMode* SP_GameMode = GetWorld()->GetAuthGameMode<ASP_ShooterGameMode>();
	AMP_ShooterGameMode* MP_GameMode = GetWorld()->GetAuthGameMode<AMP_ShooterGameMode>();

	if (SP_GameMode == nullptr && MP_GameMode == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	TArray<ABaseWaypoint*> ConsideredWaypoints = SP_GameMode != nullptr ? SP_GameMode->GetAllWayPoints() : MP_GameMode->GetAllWayPoints();
	if (ConsideredWaypoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}
	
	ABaseWaypoint* LastSelectedPoint = Cast<ABaseWaypoint>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (LastSelectedPoint != nullptr)
	{
		ConsideredWaypoints.Remove(LastSelectedPoint);
	}
	
	TArray<ABaseWaypoint*> AllWayPoints = SP_GameMode != nullptr ? SP_GameMode->GetAllWayPoints() : MP_GameMode->GetAllWayPoints();
	ABaseWaypoint* Waypoint = AllWayPoints[FMath::RandRange(0, ConsideredWaypoints.Num()-1)];
	if (Waypoint == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Waypoint);
	
	return EBTNodeResult::Succeeded;
}
