// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateHealthInfo.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UBTService_UpdateHealthInfo : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_UpdateHealthInfo();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompPtr;
	
	float GetHealthPercent();
};
