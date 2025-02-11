// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CreateEngagementCircleLocation.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UBTService_CreateEngagementCircleLocation : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CreateEngagementCircleLocation();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
