// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/BTComponents/BTService_DebugPath.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_DebugPath::UBTService_DebugPath()
{
	NodeName = TEXT("Debug Path");
}

void UBTService_DebugPath::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return;

	const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	UNavigationPath *tpath;
	
	AActor* SelectedWayPoint = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelectedWaypoint"));
	tpath = NavSys->FindPathToLocationSynchronously(GetWorld(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), SelectedWayPoint->GetActorLocation());

	if (tpath != nullptr)
	{
		for (int i = 0; i < tpath->PathPoints.Num(); i++)
		{
			DrawDebugSphere(GetWorld(), tpath->PathPoints[i], 10.0f, 12, FColor(255, 0, 0), false, 5.0f);
		}
	}
}
