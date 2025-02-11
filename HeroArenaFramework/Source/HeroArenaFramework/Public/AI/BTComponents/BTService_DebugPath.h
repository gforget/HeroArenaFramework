// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DebugPath.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UBTService_DebugPath : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_DebugPath();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float GetPathLength(UObject* WorldContextObject, const FVector& Start, const FVector& End);
};
