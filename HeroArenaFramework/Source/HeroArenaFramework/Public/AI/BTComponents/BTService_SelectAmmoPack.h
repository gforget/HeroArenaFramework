// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SelectAmmoPack.generated.h"

class ABaseAmmoPack;
/**
 * 
 */
class ASP_AmmoPack;
UCLASS()
class HEROARENAFRAMEWORK_API UBTService_SelectAmmoPack : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_SelectAmmoPack();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompPtr;

	UPROPERTY()
	UWorld* CurrentWorldPtr;
	
	ABaseAmmoPack* GetClosestAmmoPack();
};
