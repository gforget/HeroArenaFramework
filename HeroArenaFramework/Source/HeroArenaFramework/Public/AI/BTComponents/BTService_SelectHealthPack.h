// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SelectHealthPack.generated.h"

/**
 * 
 */
class ABaseHealthPack;

UCLASS()
class HEROARENAFRAMEWORK_API UBTService_SelectHealthPack : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SelectHealthPack();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompPtr;

	UPROPERTY()
	UWorld* CurrentWorldPtr;
	
	ABaseHealthPack* GetClosestHealthPack();
	
};
