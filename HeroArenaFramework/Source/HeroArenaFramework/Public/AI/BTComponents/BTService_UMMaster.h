// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UMMaster.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum EAIStateEnum
{
	Flee = 0,
	LookForHealthPack = 1,
	EngageEnemy = 2,
	Explore = 3,
	Default = 4,
	Inactive = 5
};

UCLASS()
class HEROARENAFRAMEWORK_API UBTService_UMMaster : public UBTService
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugActive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	TEnumAsByte<EAIStateEnum> DefaultEnumState = EAIStateEnum::Default;

	//Flee considerations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flee Properties")
	FRuntimeFloatCurve FlC_HealthPercentCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flee Properties")
	FVector2D FlC_HaveAValidFleePointBool = FVector2D(1.0f, 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flee Properties")
	FRuntimeFloatCurve FlC_TimeSeenEnemyCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Flee Properties")
	float FlC_MaxTimeSeenAnEnemy = 10.0f;
	
	//Look for health pack properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look For Health Pack Properties")
	FRuntimeFloatCurve LFHPC_HealthPercentCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look For Health Pack Properties")
	FRuntimeFloatCurve LFHPC_HealthPackDistanceCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Look For Health Pack Properties")
	float MaxDistanceHealthPack;
	
	//Engage Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engage Enemy Properties")
	FVector2D EEC_IsLastKnownEnemyLocationOrSoundHeardLocationIsSetBool = FVector2D(1.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engage Enemy Properties")
	FRuntimeFloatCurve EEC_TimeSenseEnemyCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engage Enemy Properties")
	FRuntimeFloatCurve EEC_AmmoInTotalPercentCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engage Enemy Properties")
	FRuntimeFloatCurve EEC_HealthPercentCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Engage Enemy Properties")
	float EEC_MaxTimeSenseAnEnemy = 10.0f;
	
	//Explore properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explore Properties")
	FVector2D ExC_EnemyInSightBool = FVector2D(1.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explore Properties")
	FVector2D ExC_Have100PercentHP = FVector2D(1.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explore Properties")
	FVector2D ExC_Have100PercentTotalAmmo = FVector2D(1.0f, 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explore Properties")
	FRuntimeFloatCurve ExC_TimeSeenEnemyCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Explore Properties")
	float ExC_MaxTimeSeenAnEnemy = 10.0f;
	
	UBTService_UMMaster();
	
	//Utility method
	float GetPathLength(const FVector& Start, const FVector& End) const;
	UNavigationPath* GetPath(const FVector& Start, const FVector& End) const;
	
protected:
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	UPROPERTY()
	UWorld* CurrentWorldPtr;
	
	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompPtr;

	//UM Methods
	TEnumAsByte<EAIStateEnum> ChooseState();
	float ScoreAggregation(int NbConsiderations, float OriginalScore);
	
	//Flee considerations
	float FleeC_HealthPercent();
	float FleeC_HaveAValidFleePoint();
	float FleeC_TimeSeenEnemy();
	
	//Look for health pack considerations
	float LookForHealthPackC_HealthPercent();
	float LookForHealthPackC_HealthPackDistance();
	
	//Engage Player considerations
	float EngageEnemyC_IsLastKnownEnemyLocationIsSet();
	float EngageEnemyC_TimeSenseAnEnemy();
	float EngageEnemyC_HealthPercent();
	
	//Explore
	float ExploreC_EnemyInSight();
	float ExploreC_TimeSeenEnemy();
	float ExploreC_Have100PercentHP();
	float ExploreC_Have100PercentTotalAmmo();
};
