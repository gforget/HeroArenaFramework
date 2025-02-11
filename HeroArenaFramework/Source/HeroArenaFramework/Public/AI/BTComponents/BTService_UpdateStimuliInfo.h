// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateStimuliInfo.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UBTService_UpdateStimuliInfo : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateStimuliInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Visual Stimuli")
	float MaxTimeSeenAnEnemy = 10.0f;
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompPtr;
};
