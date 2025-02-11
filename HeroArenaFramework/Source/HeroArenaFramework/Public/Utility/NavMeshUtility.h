// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NavMeshUtility.generated.h"

/**
 * 
 */
class UNavigationPath;
UCLASS()
class HEROARENAFRAMEWORK_API UNavMeshUtility : public UObject
{
	GENERATED_BODY()

public :
	float GetPathLength(const FVector& Start, const FVector& End, UWorld* WorldContext) const;
	UNavigationPath* GetPath(const FVector& Start, const FVector& End, UWorld* WorldContext) const;
	bool IsPointOnNavmesh(const FVector& Position, UWorld* WorldContext) const;

	bool FindValidNavmeshPosition(
		const FVector& Position,
		UWorld* WorldContext,
		FVector& ValidPosition,
		const FVector& SearchExtent = FVector(500.0f,500.0f,500.0f)
		);
};
