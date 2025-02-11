// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SinglePlayer/SP_TeamDeathMatchGameMode.h"

#include "Actors/BaseSpawningPoint.h"
#include "Actors/SinglePlayer/SP_ShooterCharacter.h"
#include "Controllers/SinglePlayer/SP_ShooterPlayerController.h"
#include "GameMode/MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

ASP_TeamDeathMatchGameMode::ASP_TeamDeathMatchGameMode()
{
	FactionManagerComponent->AllianceMode = EAllianceMode::Team;
}

void ASP_TeamDeathMatchGameMode::OnShooterCharacterDeath(ABaseShooterCharacter* DeadShooterCharacter)
{
	Super::OnShooterCharacterDeath(DeadShooterCharacter);
	
	TeamCount[DeadShooterCharacter->GetTeam()]--;
	if (TeamCount[DeadShooterCharacter->GetTeam()] == 0)
	{
		const ETeam WinningTeam = DeadShooterCharacter->GetTeam() == ETeam::RedTeam ? ETeam::BlueTeam : ETeam::RedTeam; 
		EndGame(WinningTeam);
	}
}

void ASP_TeamDeathMatchGameMode::AddShooterCharacterCount(ABaseShooterCharacter* ShooterCharacterToRegister)
{
	Super::AddShooterCharacterCount(ShooterCharacterToRegister);
	
	if (!TeamCount.Contains(ShooterCharacterToRegister->GetTeam()))
	{
		TeamCount.Add(ShooterCharacterToRegister->GetTeam(), 0);
	}
		
	TeamCount[ShooterCharacterToRegister->GetTeam()]++;
}

void ASP_TeamDeathMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	//Gather all spawn point
	UWorld* WorldPtr = GetWorld();
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(WorldPtr,AActor::StaticClass(),AllActors);

	if (WorldPtr && AllActors.Num() > 0)
	{
		for (int i=0; i<AllActors.Num(); i++)
		{
			if (ABaseSpawningPoint* SpawningPoint = Cast<ABaseSpawningPoint>(AllActors[i]))
			{
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
	
	if (const UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		// Spawn a blue player + number of blue bots where there are spawn point
		for (int i=0; i<(1+MainGameInstance->NbBlueBots); i++)
		{
			const int SpawnIndex = FMath::RandRange(0, AllBlueSpawnPoints.Num()-1);
			const ABaseSpawningPoint* CurrentSpawningPoint = AllBlueSpawnPoints[SpawnIndex];
			
			if (i==0) // put the spawned player at a spawn position
			{
				ASP_ShooterCharacter* ShooterCharacter = WorldPtr->SpawnActor<ASP_ShooterCharacter>(
					CurrentSpawningPoint->BlueTeamShooterCharacterClass,
					CurrentSpawningPoint->GetActorLocation(),
					CurrentSpawningPoint->GetActorRotation()
				);
				
				if (ASP_ShooterPlayerController* PlayerController = Cast<ASP_ShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					PlayerController->Possess(ShooterCharacter);
					PlayerController->InstantiateGameModeHUD(GameModeHUDClass);
				}
			}
			else
			{
				ASP_ShooterCharacter* ShooterCharacter = WorldPtr->SpawnActor<ASP_ShooterCharacter>(
					CurrentSpawningPoint->BlueTeamShooterCharacterClass,
					CurrentSpawningPoint->GetActorLocation(),
					CurrentSpawningPoint->GetActorRotation()
				);
				
				if (ASP_ShooterPlayerController* PlayerController = Cast<ASP_ShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					PlayerController->AddOHHealthBar(ShooterCharacter);
				}
			}
				
			AllBlueSpawnPoints.RemoveAt(SpawnIndex);
		}

		//Spawn Enemy Team
		for (int i=0; i<MainGameInstance->NbRedBots; i++)
		{
			const int SpawnIndex = FMath::RandRange(0, AllRedSpawnPoints.Num()-1);
			const ABaseSpawningPoint* CurrentSpawningPoint = AllRedSpawnPoints[SpawnIndex];

			ASP_ShooterCharacter* ShooterCharacter = WorldPtr->SpawnActor<ASP_ShooterCharacter>(
				CurrentSpawningPoint->RedTeamShooterCharacterClass,
				CurrentSpawningPoint->GetActorLocation(),
				CurrentSpawningPoint->GetActorRotation()
			);
				
			if (ASP_ShooterPlayerController* PlayerController = Cast<ASP_ShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
			{
				PlayerController->AddOHHealthBar(ShooterCharacter);
			}
			
			AllRedSpawnPoints.RemoveAt(SpawnIndex);
		}
	}
}

void ASP_TeamDeathMatchGameMode::EndGame(ETeam TeamWin)
{
	if (ASP_ShooterPlayerController* PlayerController = Cast<ASP_ShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (PlayerTeam == TeamWin)
		{
			PlayerController->GameOver(WinScreenClass);
		}
		else
		{
			PlayerController->GameOver(LoseScreenClass);
		}
	}
}
