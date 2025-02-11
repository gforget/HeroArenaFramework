// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UBTService_Shoot : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Shoot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBeforeStartingShooting = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimAdjustmentSpeed = 150.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimOffset = 15000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPullTrigger = true;
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	
	UPROPERTY(EditAnywhere)
	bool bDebug;
};
