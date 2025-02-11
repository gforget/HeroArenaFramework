// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SelectFleeWaypoint.generated.h"

/**
 * 
 */

class ABaseWaypoint;
UCLASS()
class HEROARENAFRAMEWORK_API UBTService_SelectFleeWaypoint : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_SelectFleeWaypoint();
	
private:
	ABaseWaypoint* GetClosestValidFleePoint();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	UWorld* CurrentWorldPtr;
	
	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompPtr;
};
