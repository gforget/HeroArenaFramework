// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMode/SinglePlayer/SP_BaseGameMode.h"

#include "Actors/SinglePlayer/SP_AmmoPack.h"
#include "Actors/SinglePlayer/SP_SpawningPoint.h"
#include "Actors/SinglePlayer/SP_HeroCharacter.h"

ASP_BaseGameMode::ASP_BaseGameMode()
{
	FactionManagerComponent = CreateDefaultSubobject<UTeamManager>(TEXT("Faction Manager Component"));
}

void ASP_BaseGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASP_BaseGameMode::OnShooterCharacterDeath(ABaseHeroCharacter* DeadShooterCharacter)
{
	
}

void ASP_BaseGameMode::AddShooterCharacterCount(ABaseHeroCharacter* ShooterCharacterToRegister)
{
}

TArray<ABaseWaypoint*> ASP_BaseGameMode::GetAllWayPoints()
{
	return AllWayPoints;
}

void ASP_BaseGameMode::AddWayPoint(ABaseWaypoint* WayPoint)
{
	AllWayPoints.Add(WayPoint);
}

TArray<ABaseAmmoPack*> ASP_BaseGameMode::GetAllAmmoPacks()
{
	return AllAmmoPacks;
}

void ASP_BaseGameMode::AddAmmoPack(ABaseAmmoPack* AmmoPack)
{
	AllAmmoPacks.Add(AmmoPack);
}

TArray<ABaseHealthPack*> ASP_BaseGameMode::GetAllHealthPacks()
{
	return AllHealthPacks;
}

void ASP_BaseGameMode::AddHealthPack(ABaseHealthPack* HealthPack)
{
	AllHealthPacks.Add(HealthPack);
}

void ASP_BaseGameMode::RegisterEvent(ABaseHeroCharacter* ShooterCharacterRef)
{
	ShooterCharacterRef->OnDeadEvent.AddDynamic(this, &ASP_BaseGameMode::OnShooterCharacterDeath);
}



