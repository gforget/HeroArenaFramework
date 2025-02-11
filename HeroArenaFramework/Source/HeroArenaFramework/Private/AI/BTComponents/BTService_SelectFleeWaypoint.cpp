// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTService_SelectFleeWaypoint.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "Actors/BaseWaypoint.h"
#include "Actors/SinglePlayer/SP_ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameMode.h"
#include "GameMode/SinglePlayer/SP_ShooterGameMode.h"
#include "Utility/NavMeshUtility.h"

UBTService_SelectFleeWaypoint::UBTService_SelectFleeWaypoint()
{
	NodeName = TEXT("Select Flee Waypoint");
}

void UBTService_SelectFleeWaypoint::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (CurrentWorldPtr == nullptr) CurrentWorldPtr = GetWorld();
	OwnerCompPtr = &OwnerComp;
	
	if (OwnerCompPtr != nullptr && CurrentWorldPtr != nullptr)
	{
		OwnerCompPtr->GetBlackboardComponent()->SetValueAsObject(FName("SelectedFleePoint"), GetClosestValidFleePoint());
	}
}

ABaseWaypoint* UBTService_SelectFleeWaypoint::GetClosestValidFleePoint()
{
	ABaseWaypoint* SelectedFleePoint = nullptr;
	const bool bLastKnownEnemyLocationIsSet = OwnerCompPtr->GetBlackboardComponent()->IsVectorValueSet(FName("LastKnownEnemyLocation"));

	if (!bLastKnownEnemyLocationIsSet)
	{
		return SelectedFleePoint;
	}

	const FVector CharLocation = OwnerCompPtr->GetAIOwner()->GetPawn()->GetActorLocation();
	const FVector2D CharLocation2D = FVector2D(CharLocation.X, CharLocation.Y);
	
	const FVector LastKnownEnemyLocation = OwnerCompPtr->GetBlackboardComponent()->GetValueAsVector(FName("LastKnownEnemyLocation"));
	const FVector2D LastKnownEnemyLocation2D = FVector2D(LastKnownEnemyLocation.X, LastKnownEnemyLocation.Y);

	//Since SP and MP GameMode do not inherit from the same base, we need to check both
	ASP_ShooterGameMode* SP_GameMode = GetWorld()->GetAuthGameMode<ASP_ShooterGameMode>();
	AMP_ShooterGameMode* MP_GameMode = GetWorld()->GetAuthGameMode<AMP_ShooterGameMode>();

	if (SP_GameMode == nullptr && MP_GameMode == nullptr)
	{
		return nullptr;
	}
	
	TArray<ABaseWaypoint*> ConsideredWaypoints = SP_GameMode != nullptr ? SP_GameMode->GetAllWayPoints() : MP_GameMode->GetAllWayPoints();

	float HighestDistance = 0.0f;
	int NbValidFleePoint = 0;
	
	for (int i=0; i<ConsideredWaypoints.Num(); i++)
	{
		const ABaseShooterCharacter* AICharacter = Cast<ABaseShooterCharacter>(OwnerCompPtr->GetAIOwner()->GetPawn());

		if (AICharacter != nullptr)
		{
			UNavigationPath* TPath = AICharacter->NavMeshUtility->GetPath(CharLocation, ConsideredWaypoints[i]->GetActorLocation(), CurrentWorldPtr);
			if (TPath != nullptr)
			{
				if (TPath->PathPoints.Num() > 0)
				{
					const FVector FirstPointLocation = TPath->PathPoints[1];
					FVector2D FirstPointLocation2D = FVector2D(FirstPointLocation.X, FirstPointLocation.Y);
			
					FVector2D CharToFirstPoint = FirstPointLocation2D - CharLocation2D;
					CharToFirstPoint.Normalize();

					FVector2D CharToEnemy = LastKnownEnemyLocation2D - CharLocation2D;
					CharToEnemy.Normalize();
			
					//Is the first point on the opposite side of the last known position of the enemy
					const float DotProduct = CharToFirstPoint.Dot(CharToEnemy);
					if (DotProduct < 0.0f)
					{
						NbValidFleePoint++;
						const float CurrentPathDistanceFromEnemyPosition = AICharacter->NavMeshUtility->GetPathLength(LastKnownEnemyLocation, ConsideredWaypoints[i]->GetActorLocation(), CurrentWorldPtr);
						if (CurrentPathDistanceFromEnemyPosition > HighestDistance)
						{
							SelectedFleePoint = ConsideredWaypoints[i];
							HighestDistance = CurrentPathDistanceFromEnemyPosition;
						}
					}
				}
			}
		}
	}
	
	return NbValidFleePoint > 1 ? SelectedFleePoint : nullptr;
}
