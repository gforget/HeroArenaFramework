// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Multiplayer/ShooterGameMode/MP_BaseGameMode.h"
#include "Actors/Multiplayer/MP_AmmoPack.h"
#include "Actors/Multiplayer/MP_SpawningPoint.h"
#include "Actors/Multiplayer/MP_HeroCharacter.h"
#include "GameMode/MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

AMP_BaseGameMode::AMP_BaseGameMode()
{
	FactionManagerComponent = CreateDefaultSubobject<UTeamManager>(TEXT("Faction Manager Component"));
}

void AMP_BaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	FillSpawningPoints();
}

void AMP_BaseGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	FillSpawningPoints();
}

void AMP_BaseGameMode::CallLeaveSession()
{
	GetWorldTimerManager().SetTimer(RestartTimer, this, &AMP_BaseGameMode::LeaveSession, RestartDelay);
}

void AMP_BaseGameMode::FillSpawningPoints()
{
	if (!SpawningPointsHaveBeenFilled)
	{
		//Gather all spawn point
		UWorld* WorldPtr = GetWorld();
		TArray<AActor*> AllActors;
		UGameplayStatics::GetAllActorsOfClass(WorldPtr,AActor::StaticClass(),AllActors);

		if (WorldPtr && AllActors.Num() > 0)
		{
			for (int i=0; i<AllActors.Num(); i++)
			{
				if (AMP_SpawningPoint* SpawningPoint = Cast<AMP_SpawningPoint>(AllActors[i]))
				{
					AllSpawnPoints.Add(SpawningPoint);
					if (SpawningPoint->Team == ETeam::RedTeam)
					{
						AllRedSpawnPoints.Add(SpawningPoint);
					}
					else
					{
						AllBlueSpawnPoints.Add(SpawningPoint);
					}
				}
			}
		}
		SpawningPointsHaveBeenFilled = true;
	}
}

void AMP_BaseGameMode::LeaveSession()
{
	if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance()))
	{
		MainGameInstance->CallBPLeaveSession();
	}
}

void AMP_BaseGameMode::OnShooterCharacterDeath(ABaseHeroCharacter* DeadShooterCharacter)
{
	
}

TArray<ABaseWaypoint*> AMP_BaseGameMode::GetAllWayPoints()
{
	return AllWayPoints;
}

void AMP_BaseGameMode::AddWayPoint(ABaseWaypoint* Waypoint)
{
	AllWayPoints.Add(Waypoint);
}

TArray<ABaseAmmoPack*> AMP_BaseGameMode::GetAllAmmoPacks()
{
	return AllAmmoPacks;
}

void AMP_BaseGameMode::AddAmmoPack(ABaseAmmoPack* AmmoPack)
{
	AllAmmoPacks.Add(AmmoPack);
}

TArray<ABaseHealthPack*> AMP_BaseGameMode::GetAllHealthPacks()
{
	return AllHealthPacks;
}

void AMP_BaseGameMode::AddHealthPack(ABaseHealthPack* HealthPack)
{
	AllHealthPacks.Add(HealthPack);
}

void AMP_BaseGameMode::RegisterEvent(ABaseHeroCharacter* ShooterCharacterRef)
{
	ShooterCharacterRef->OnDeadEvent.AddDynamic(this, &AMP_BaseGameMode::OnShooterCharacterDeath);
}