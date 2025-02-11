// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ChooseWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UBTTask_ChooseWaypoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ChooseWaypoint();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
