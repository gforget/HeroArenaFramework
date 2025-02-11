// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeamManager.generated.h"

UENUM(BlueprintType)
enum ETeam
{
	BlueTeam = 0,
	RedTeam = 1,
	NoTeam = 2
};

UENUM(BlueprintType)
enum ETeamRelation
{
	Ally,
	Enemy
};

UENUM(BlueprintType)
enum EAllianceMode
{
	FFA,
	Team,
	None
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEROARENAFRAMEWORK_API UTeamManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTeamManager();

	UFUNCTION(BlueprintCallable, Category="Main Functions")
	ETeamRelation GetTeamRelation(ETeam TeamA, ETeam TeamB);
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<EAllianceMode> AllianceMode = EAllianceMode::FFA;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};
