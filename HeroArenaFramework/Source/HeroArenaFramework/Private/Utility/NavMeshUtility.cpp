// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/NavMeshUtility.h"
#include "NavigationSystem.h"

float UNavMeshUtility::GetPathLength(const FVector& Start, const FVector& End, UWorld* WorldContext) const
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(WorldContext);
	if (!NavSys) return -1.0f;

	const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (!NavData) return -1.0f;

	FNavLocation StartLocation, EndLocation;
	if (NavSys->ProjectPointToNavigation(Start, StartLocation, FVector(500, 500, 500), NavData)
		&& NavSys->ProjectPointToNavigation(End, EndLocation, FVector(500, 500, 500), NavData))
	{
		float PathLength;
		if (NavSys->GetPathLength(StartLocation.Location, EndLocation.Location, PathLength, NavData))
		{
			return PathLength;
		}
	}

	return -1.0f;
}

UNavigationPath* UNavMeshUtility::GetPath(const FVector& Start, const FVector& End, UWorld* WorldContext) const
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(WorldContext);
	if (!NavSys) return nullptr;

	const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (!NavData) return nullptr;
	
	return NavSys->FindPathToLocationSynchronously(WorldContext, Start, End);
}

bool UNavMeshUtility::IsPointOnNavmesh(const FVector& Position, UWorld* WorldContext) const
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(WorldContext);
	if (!NavSys) return false;

	FNavLocation NavLocation;
	return NavSys->ProjectPointToNavigation(Position, NavLocation);
}

bool UNavMeshUtility::FindValidNavmeshPosition(
	const FVector& Position,
	UWorld* WorldContext,
	FVector& ValidPosition,
	const FVector& SearchExtent
	)
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(WorldContext);
	if (!NavSys)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nav System not avaialble"));
		return false;
	} 

	FNavLocation NavLocation;
	const bool ValidPositionFound = NavSys->ProjectPointToNavigation(Position, NavLocation, SearchExtent);
	
	if (ValidPositionFound)
	{
		ValidPosition = NavLocation.Location;
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid position found, extend the search"));
		return false;
	}
}
