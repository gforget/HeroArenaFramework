// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTDecorator_FloatLowerOrEqual.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_FloatLowerOrEqual::UBTDecorator_FloatLowerOrEqual()
{
	NodeName = TEXT("Float Blackboard value <=");
	NotifyObserver = EBTBlackboardRestart::ResultChange;
}

bool UBTDecorator_FloatLowerOrEqual::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const bool bReturnValue = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey()) <= TargetValue;
	return bReturnValue;
}

EBlackboardNotificationResult UBTDecorator_FloatLowerOrEqual::OnBlackboardKeyValueChange( const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorComp = (UBehaviorTreeComponent*)Blackboard.GetBrainComponent();
	if (BehaviorComp == nullptr)
	{
		return EBlackboardNotificationResult::RemoveObserver;
	}

	if (BlackboardKey.GetSelectedKeyID() == ChangedKeyID)
	{
		// can't simply use BehaviorComp->RequestExecution(this) here, we need to support condition/value change modes

		const EBTDecoratorAbortRequest RequestMode = (NotifyObserver == EBTBlackboardRestart::ValueChange) ? EBTDecoratorAbortRequest::ConditionPassing : EBTDecoratorAbortRequest::ConditionResultChanged;
		ConditionalFlowAbort(*BehaviorComp, RequestMode);
	}

	return EBlackboardNotificationResult::ContinueObserving;
}
