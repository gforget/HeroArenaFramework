// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMode/SinglePlayer/SP_ShooterGameMode.h"

#include "Actors/SinglePlayer/SP_AmmoPack.h"
#include "Actors/SinglePlayer/SP_SpawningPoint.h"
#include "Actors/SinglePlayer/SP_ShooterCharacter.h"

ASP_ShooterGameMode::ASP_ShooterGameMode()
{
	FactionManagerComponent = CreateDefaultSubobject<UTeamManager>(TEXT("Faction Manager Component"));
}

void ASP_ShooterGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASP_ShooterGameMode::OnShooterCharacterDeath(ABaseShooterCharacter* DeadShooterCharacter)
{
	
}

void ASP_ShooterGameMode::AddShooterCharacterCount(ABaseShooterCharacter* ShooterCharacterToRegister)
{
}

TArray<ABaseWaypoint*> ASP_ShooterGameMode::GetAllWayPoints()
{
	return AllWayPoints;
}

void ASP_ShooterGameMode::AddWayPoint(ABaseWaypoint* WayPoint)
{
	AllWayPoints.Add(WayPoint);
}

TArray<ABaseAmmoPack*> ASP_ShooterGameMode::GetAllAmmoPacks()
{
	return AllAmmoPacks;
}

void ASP_ShooterGameMode::AddAmmoPack(ABaseAmmoPack* AmmoPack)
{
	AllAmmoPacks.Add(AmmoPack);
}

TArray<ABaseHealthPack*> ASP_ShooterGameMode::GetAllHealthPacks()
{
	return AllHealthPacks;
}

void ASP_ShooterGameMode::AddHealthPack(ABaseHealthPack* HealthPack)
{
	AllHealthPacks.Add(HealthPack);
}

void ASP_ShooterGameMode::RegisterEvent(ABaseShooterCharacter* ShooterCharacterRef)
{
	ShooterCharacterRef->OnDeadEvent.AddDynamic(this, &ASP_ShooterGameMode::OnShooterCharacterDeath);
}



