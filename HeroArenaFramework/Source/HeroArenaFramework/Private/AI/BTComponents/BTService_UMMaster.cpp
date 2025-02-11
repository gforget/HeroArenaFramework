// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTService_UMMaster.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Actors/BaseAmmoPack.h"
#include "Actors/BaseHealthPack.h"
#include "Actors/BaseShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_UMMaster::UBTService_UMMaster()
{
	NodeName = TEXT("UM Master");
}

void UBTService_UMMaster::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (CurrentWorldPtr == nullptr) CurrentWorldPtr = GetWorld();
	OwnerCompPtr = &OwnerComp;

	if (OwnerCompPtr != nullptr && CurrentWorldPtr != nullptr)
	{
		if (DefaultEnumState == EAIStateEnum::Default)
		{
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsEnum(FName("State"), ChooseState());
		}
		else
		{
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsEnum(FName("State"), DefaultEnumState);
		}
	}
}

TEnumAsByte<EAIStateEnum> UBTService_UMMaster::ChooseState()
{

	if (ABaseShooterCharacter* ShooterCharacter = Cast<ABaseShooterCharacter>(OwnerCompPtr->GetAIOwner()->GetPawn()))
	{
		if (ShooterCharacter->GetCharacterMovement()->IsFalling())
		{
			return EAIStateEnum::Inactive;
		}
	}
	
	const int STATE_ARRAY_SIZE = 5;
	float StateScoreArray[STATE_ARRAY_SIZE];

	//Flee
	StateScoreArray[EAIStateEnum::Flee] = ScoreAggregation(3,
		1.0f *
		FleeC_HealthPercent() *
		FleeC_HaveAValidFleePoint() *
		FleeC_TimeSeenEnemy()
		);

	//LookForHealthPack 
	StateScoreArray[EAIStateEnum::LookForHealthPack] = ScoreAggregation(2,
		1.0f *
		LookForHealthPackC_HealthPercent() *
		LookForHealthPackC_HealthPackDistance()
		);
	
	//LookForAmmo
	StateScoreArray[EAIStateEnum::LookForAmmo] = ScoreAggregation(2,
		1.0f *
		LookForAmmoPackC_AmmoReservePercent() *
		LookForAmmoPackC_AmmoPackDistance()
		);
	
	//EngagePlayer
	StateScoreArray[EAIStateEnum::EngageEnemy] = ScoreAggregation(4,
		1.0f *
		EngageEnemyC_IsLastKnownEnemyLocationIsSet() *
		EngageEnemyC_TimeSenseAnEnemy() *
		EngageEnemyC_AmmoInTotalPercent() *
		EngageEnemyC_HealthPercent()
		);

	//Explore
	StateScoreArray[EAIStateEnum::Explore] = ScoreAggregation(4,
		1.0f *
		ExploreC_Have100PercentHP()*
		ExploreC_Have100PercentTotalAmmo()*
		ExploreC_EnemyInSight() *
		ExploreC_TimeSeenEnemy()
		);
	
	int HighestScoreIndex = 0;
	float CurrentHighestScore = StateScoreArray[HighestScoreIndex];

	for (int i=1; i<STATE_ARRAY_SIZE; i++)
	{
		if (StateScoreArray[i] > CurrentHighestScore)
		{
			CurrentHighestScore = StateScoreArray[i];
			HighestScoreIndex = i;
		}
	}

	//Debug
	if (bDebugActive)
	{
		for (int i=0; i<STATE_ARRAY_SIZE;i++)
		{
			const FString StateString = UEnum::GetValueAsString(static_cast<EAIStateEnum>(i));
			const FString StateScore = FString::Printf(TEXT("%f"), StateScoreArray[i]);
		
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange,*FString::Printf(TEXT("%s : %s"), *StateString, *StateScore));
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("%s : %s"), *StateString, *StateScore));
		}
	
		const FString WiningStateString = UEnum::GetValueAsString(static_cast<EAIStateEnum>(HighestScoreIndex));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, *FString::Printf(TEXT("wining state is : %s"), *WiningStateString));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("wining state is : %s"), *WiningStateString));
	
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("-------------------"));
		UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT("-------------------"));
	}

	//delete[] &StateScoreArray; // no need to delete it, it is on the stack
	
	//return state
	return static_cast<EAIStateEnum>(HighestScoreIndex);
}

float UBTService_UMMaster::ScoreAggregation(int NbConsiderations, float OriginalScore)
{
	const float modFactor = 1 - (1 / NbConsiderations);
	const float makeupValue = (1 - OriginalScore) * modFactor;
	const float FinalScore = OriginalScore + (makeupValue * OriginalScore);
	
	return FinalScore;
}

float UBTService_UMMaster::FleeC_HealthPercent()
{
	return FlC_HealthPercentCurve.GetRichCurveConst()->Eval(OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("SelfHealthPercent")));
}

float UBTService_UMMaster::FleeC_HaveAValidFleePoint()
{
	return OwnerCompPtr->GetBlackboardComponent()->GetValueAsObject(FName("SelectedFleePoint")) != nullptr ? FlC_HaveAValidFleePointBool.X:FlC_HaveAValidFleePointBool.Y;
}

float UBTService_UMMaster::FleeC_TimeSeenEnemy()
{
	const float TimeSeenAnEnemy = OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("TimeSeenAnEnemy"));
	return FlC_TimeSeenEnemyCurve.GetRichCurveConst()->Eval(FMath::Min(TimeSeenAnEnemy, EEC_MaxTimeSenseAnEnemy)/EEC_MaxTimeSenseAnEnemy);
}

float UBTService_UMMaster::LookForHealthPackC_HealthPercent()
{
	return LFHPC_HealthPercentCurve.GetRichCurveConst()->Eval(OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("SelfHealthPercent")));
}

float UBTService_UMMaster::LookForHealthPackC_HealthPackDistance()
{
	if (OwnerCompPtr->GetAIOwner() == nullptr)
	{
		return 0.0f;
	}

	const ABaseHealthPack* SelectedHealthPack = Cast<ABaseHealthPack>(OwnerCompPtr->GetBlackboardComponent()->GetValueAsObject(FName("SelectedHealthPack")));
	if (SelectedHealthPack == nullptr)
	{
		return 0.0f;
	}
	
	const FVector CharLocation = OwnerCompPtr->GetAIOwner()->GetPawn()->GetActorLocation();
	const float CalculatedDistance = FMath::Min(GetPathLength(SelectedHealthPack->GetActorLocation(),CharLocation), MaxDistanceHealthPack);
	
	return LFHPC_HealthPercentCurve.GetRichCurveConst()->Eval(CalculatedDistance/MaxDistanceHealthPack);
}

float UBTService_UMMaster::LookForAmmoPackC_AmmoReservePercent()
{
	return LFAPC_AmmoReservePercentCurve.GetRichCurveConst()->Eval(OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("AmmoInReservePercent")));
}

 float UBTService_UMMaster::LookForAmmoPackC_AmmoPackDistance()
{
	if (OwnerCompPtr->GetAIOwner() == nullptr)
	{
		return 0.0f;
	}

	const ABaseAmmoPack* SelectedAmmoPack = Cast<ABaseAmmoPack>(OwnerCompPtr->GetBlackboardComponent()->GetValueAsObject(FName("SelectedAmmoPack")));
	
	if (SelectedAmmoPack == nullptr)
	{
		return 0.0f;
	}
	
	const FVector CharLocation = OwnerCompPtr->GetAIOwner()->GetPawn()->GetActorLocation(); 
	const float CalculatedDistance = FMath::Min(GetPathLength(SelectedAmmoPack->GetActorLocation(),CharLocation), MaxDistanceAmmoPack);

	return LFAPC_AmmoPackDistanceCurve.GetRichCurveConst()->Eval(CalculatedDistance/MaxDistanceAmmoPack);
}

float UBTService_UMMaster::EngageEnemyC_IsLastKnownEnemyLocationIsSet()
{
	const bool bIsSet = OwnerCompPtr->GetBlackboardComponent()->IsVectorValueSet(FName("LastKnownEnemyLocation")) || OwnerCompPtr->GetBlackboardComponent()->IsVectorValueSet(FName("SoundHeardLocation"));
	return bIsSet ? EEC_IsLastKnownEnemyLocationOrSoundHeardLocationIsSetBool.X : EEC_IsLastKnownEnemyLocationOrSoundHeardLocationIsSetBool.Y;
}

float UBTService_UMMaster::EngageEnemyC_TimeSenseAnEnemy()
{
	const float TimeSeenAnEnemy = OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("TimeSeenAnEnemy"));
	const float TimeHeardSomething = OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("TimeHeardSomething"));
	const float TimeHurt = OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("TimeGotHurt"));
	
	const float TimeMin = FMath::Min(FMath::Min(TimeSeenAnEnemy, TimeHeardSomething), TimeHurt);
	
	return EEC_TimeSenseEnemyCurve.GetRichCurve()->Eval(FMath::Min(TimeMin, EEC_MaxTimeSenseAnEnemy)/EEC_MaxTimeSenseAnEnemy);
}

float UBTService_UMMaster::EngageEnemyC_AmmoInTotalPercent()
{
	return EEC_AmmoInTotalPercentCurve.GetRichCurveConst()->Eval(OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("AmmoInTotalPercent")));
}

float UBTService_UMMaster::EngageEnemyC_HealthPercent()
{
	return EEC_HealthPercentCurve.GetRichCurveConst()->Eval(OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("SelfHealthPercent")));
}

float UBTService_UMMaster::ExploreC_EnemyInSight()
{
	const AActor* EnemyInSight = Cast<AActor>(OwnerCompPtr->GetBlackboardComponent()->GetValueAsObject(FName("EnemyInSight")));
	return EnemyInSight != nullptr ? ExC_EnemyInSightBool.X : ExC_EnemyInSightBool.Y;
}

 float UBTService_UMMaster::ExploreC_TimeSeenEnemy()
{
	const float TimeSeenAnEnemy = OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("TimeSeenAnEnemy"));
	return ExC_TimeSeenEnemyCurve.GetRichCurve()->Eval(FMath::Min(TimeSeenAnEnemy, ExC_MaxTimeSeenAnEnemy)/ExC_MaxTimeSeenAnEnemy);
}

float UBTService_UMMaster::ExploreC_Have100PercentHP()
{
	const float HealthPercent = OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("SelfHealthPercent"));
	return HealthPercent == 1.0 ? ExC_Have100PercentHP.X : ExC_Have100PercentHP.Y;
}

float UBTService_UMMaster::ExploreC_Have100PercentTotalAmmo()
{
	const float AmmoInTotalPercent = OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("AmmoInTotalPercent"));
	return AmmoInTotalPercent == 1.0 ? ExC_Have100PercentTotalAmmo.X : ExC_Have100PercentTotalAmmo.Y;
}

float UBTService_UMMaster::GetPathLength(const FVector& Start, const FVector& End) const
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(CurrentWorldPtr);
	if (!NavSys) return -1.0f;

	const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (!NavData) return -1.0f;

	FNavLocation StartLocation, EndLocation;
	if (NavSys->ProjectPointToNavigation(Start, StartLocation, FVector(500, 500, 500), NavData)
		&& NavSys->ProjectPointToNavigation(End, EndLocation, FVector(500, 500, 500), NavData))
	{
		float PathLength;
		if (NavSys->GetPathLength(StartLocation.Location, EndLocation.Location, PathLength, NavData))
		{
			return PathLength;
		}
	}

	return -1.0f;
}

UNavigationPath* UBTService_UMMaster::GetPath(const FVector& Start, const FVector& End) const
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return nullptr;

	const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (!NavData) return nullptr;
	
	return NavSys->FindPathToLocationSynchronously(GetWorld(), Start, End);
}
