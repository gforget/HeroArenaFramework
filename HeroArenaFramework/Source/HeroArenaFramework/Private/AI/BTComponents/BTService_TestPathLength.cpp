// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTService_TestPathLength.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_TestPathLength::UBTService_TestPathLength()
{
	NodeName = TEXT("Test Path Length");
}

void UBTService_TestPathLength::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBehaviorTreeComponent* OwnerCompPtr = &OwnerComp;

	if (OwnerCompPtr != nullptr)
	{
		AActor* SelectedWayPoint = Cast<AActor>(OwnerCompPtr->GetBlackboardComponent()->GetValueAsObject("SelectedWaypoint"));
		const float DistanceToWaypoint = GetPathLength(Cast<UObject>(SelectedWayPoint), OwnerCompPtr->GetAIOwner()->GetPawn()->GetActorLocation(), SelectedWayPoint->GetActorLocation());
		
		const FString StateScore = FString::Printf(TEXT("%f"), DistanceToWaypoint);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange,*FString::Printf(TEXT("%s"), *StateScore));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("%s"), *StateScore));
	}
}

float UBTService_TestPathLength::GetPathLength(UObject* WorldContextObject, const FVector& Start, const FVector& End)
{
	if (!WorldContextObject) return -1.0f;
	UWorld* World = WorldContextObject->GetWorld();
	
	if (!World) return -1.0f;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(World);
	if (!NavSys) return -1.0f;

	const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (!NavData) return -1.0f;

	FNavLocation StartLocation, EndLocation;
	if (NavSys->ProjectPointToNavigation(Start, StartLocation, FVector(100, 100, 100), NavData)
		&& NavSys->ProjectPointToNavigation(End, EndLocation, FVector(100, 100, 100), NavData))
	{
		float PathLength;
		if (NavSys->GetPathLength(StartLocation.Location, EndLocation.Location, PathLength, NavData))
		{
			return PathLength;
		}
	}

	return -1.0f;
}
