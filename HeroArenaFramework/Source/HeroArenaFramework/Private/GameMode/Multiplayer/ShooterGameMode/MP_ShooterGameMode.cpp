// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameMode.h"
#include "Actors/Multiplayer/MP_AmmoPack.h"
#include "Actors/Multiplayer/MP_SpawningPoint.h"
#include "Actors/Multiplayer/MP_ShooterCharacter.h"
#include "GameMode/MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

AMP_ShooterGameMode::AMP_ShooterGameMode()
{
	FactionManagerComponent = CreateDefaultSubobject<UTeamManager>(TEXT("Faction Manager Component"));
}

void AMP_ShooterGameMode::BeginPlay()
{
	Super::BeginPlay();
	FillSpawningPoints();
}

void AMP_ShooterGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	FillSpawningPoints();
}

void AMP_ShooterGameMode::CallLeaveSession()
{
	GetWorldTimerManager().SetTimer(RestartTimer, this, &AMP_ShooterGameMode::LeaveSession, RestartDelay);
}

void AMP_ShooterGameMode::FillSpawningPoints()
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

void AMP_ShooterGameMode::LeaveSession()
{
	if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance()))
	{
		MainGameInstance->CallBPLeaveSession();
	}
}

void AMP_ShooterGameMode::OnShooterCharacterDeath(ABaseShooterCharacter* DeadShooterCharacter)
{
	
}

TArray<ABaseWaypoint*> AMP_ShooterGameMode::GetAllWayPoints()
{
	return AllWayPoints;
}

void AMP_ShooterGameMode::AddWayPoint(ABaseWaypoint* Waypoint)
{
	AllWayPoints.Add(Waypoint);
}

TArray<ABaseAmmoPack*> AMP_ShooterGameMode::GetAllAmmoPacks()
{
	return AllAmmoPacks;
}

void AMP_ShooterGameMode::AddAmmoPack(ABaseAmmoPack* AmmoPack)
{
	AllAmmoPacks.Add(AmmoPack);
}

TArray<ABaseHealthPack*> AMP_ShooterGameMode::GetAllHealthPacks()
{
	return AllHealthPacks;
}

void AMP_ShooterGameMode::AddHealthPack(ABaseHealthPack* HealthPack)
{
	AllHealthPacks.Add(HealthPack);
}

void AMP_ShooterGameMode::RegisterEvent(ABaseShooterCharacter* ShooterCharacterRef)
{
	ShooterCharacterRef->OnDeadEvent.AddDynamic(this, &AMP_ShooterGameMode::OnShooterCharacterDeath);
}