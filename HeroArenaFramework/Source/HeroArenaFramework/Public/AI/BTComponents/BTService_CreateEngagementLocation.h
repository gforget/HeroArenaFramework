// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CreateEngagementLocation.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UBTService_CreateEngagementLocation : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CreateEngagementLocation();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	int offset = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	int nbHorizontalPoints = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	int nbVerticalPoints = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	float distanceBetweenPoints = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	bool bDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	float MinDistanceToTarget = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	float GroundZOffset = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	float MaxRandomBonusScore = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desired Distance Considerations Properties")
	float DesiredXYDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desired Distance Considerations Properties")
	float ThresholdDistance = 750.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desired Distance Considerations Properties")
	float WeightFactorCurrentMaxDistanceToEnemy = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desired Distance Considerations Properties")
	FRuntimeFloatCurve AggregatedDistanceToEnemyCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Higher Ground Distance Considerations Properties")
	float HigherGroundDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Higher Ground Distance Considerations Properties")
	FRuntimeFloatCurve HigherGroundDistanceCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Min Distance From Current Position Considerations Properties")
	float MinDistanceFromCurrentPosition = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Min Distance From Current Position Considerations Properties")
	FRuntimeFloatCurve MinDistanceFromCurrentPositionCurve;
	
private:
	
	float ScoreAggregation(int NbConsiderations, float OriginalScore);
	
};
