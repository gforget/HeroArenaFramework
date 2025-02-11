// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/BTComponents/BTService_Shoot.h"

#include "AIController.h"
#include "Actors/BaseShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_Shoot::UBTService_Shoot()
{
	NodeName = TEXT("Aim and Shoot");
}

void UBTService_Shoot::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}
	
	ABaseShooterCharacter* Character = Cast<ABaseShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return;
	}

	const ABaseShooterCharacter* EnemyShooterTarget = Cast<ABaseShooterCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("EnemyInSight")));
	
	if (EnemyShooterTarget != nullptr)
	{
		if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("JustSawEnemy")))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("CurrentAimPosition", EnemyShooterTarget->GetActorLocation() + EnemyShooterTarget->BodyPositionAnchor);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("JustSawEnemy"), true);
		}
		
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(
			FName("TimerBeforeShooting"),
			OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("TimerBeforeShooting")) + DeltaSeconds);

		if (OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("TimerBeforeShooting")) > TimeBeforeStartingShooting)
		{
			OwnerComp.GetAIOwner()->SetFocalPoint(OwnerComp.GetBlackboardComponent()->GetValueAsVector("CurrentAimPosition"));
			if (bPullTrigger) Character->PullTrigger();
		}
		else
		{
			Character->ReleaseTrigger();
		}
	
		const FVector AimOffsetPosition = FMath::VRand()*AimOffset;
		
		FVector NewCurrentAimPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector("CurrentAimPosition");
		NewCurrentAimPosition = UKismetMathLibrary::VInterpTo_Constant(
			NewCurrentAimPosition,
			EnemyShooterTarget->GetActorLocation() + EnemyShooterTarget->BodyPositionAnchor + AimOffsetPosition,
			DeltaSeconds,
			AimAdjustmentSpeed
			);

		if (bDebug) DrawDebugSphere(GetWorld(), NewCurrentAimPosition, 5.0f, 12, FColor::Cyan, false, 0.01f, 0, 1.0);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("CurrentAimPosition", NewCurrentAimPosition);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("JustSawEnemy"), false);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("TimerBeforeShooting"), 0.0f);
		
		if (OwnerComp.GetBlackboardComponent()->IsVectorValueSet(FName("LastKnownEnemyLocation")))
		{
			OwnerComp.GetAIOwner()->SetFocalPoint(OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("LastKnownEnemyLocation")));
		}
		else
		{
			OwnerComp.GetAIOwner()->SetFocus(nullptr);
		}
		
		Character->ReleaseTrigger();
	}
}
