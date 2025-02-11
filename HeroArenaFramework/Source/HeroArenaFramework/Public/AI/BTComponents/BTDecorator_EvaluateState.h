// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTService_UMMaster.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_EvaluateState.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UBTDecorator_EvaluateState : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EAIStateEnum> StateEvaluated = EAIStateEnum::Default;
	
	UBTDecorator_EvaluateState();

protected:
	
	UPROPERTY(Category=FlowControl, EditAnywhere)
	TEnumAsByte<EBTBlackboardRestart::Type> NotifyObserver;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;
};
