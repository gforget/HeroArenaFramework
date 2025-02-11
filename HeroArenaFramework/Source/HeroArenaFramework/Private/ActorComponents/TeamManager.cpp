// Fill out your copyright notice in the Description page of Project Settings.
#include "ActorComponents/TeamManager.h"

// Sets default values for this component's properties
UTeamManager::UTeamManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTeamManager::BeginPlay()
{
	Super::BeginPlay();
}

ETeamRelation UTeamManager::GetTeamRelation(ETeam TeamA, ETeam TeamB)
{
	if (AllianceMode == EAllianceMode::Team)
	{
		if (TeamA == TeamB)
		{
			return Ally;
		}
		else
		{
			return Enemy;
		}
	}
	else
	{
		return Enemy;
	}
}



