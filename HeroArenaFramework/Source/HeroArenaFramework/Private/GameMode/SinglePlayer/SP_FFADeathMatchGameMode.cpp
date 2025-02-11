// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SinglePlayer/SP_FFADeathMatchGameMode.h"

#include "Actors/BaseSpawningPoint.h"
#include "Actors/SinglePlayer/SP_ShooterCharacter.h"
#include "Actors/SinglePlayer/SP_ShooterSpectatorPawn.h"
#include "Controllers/SinglePlayer/SP_ShooterPlayerController.h"
#include "GameMode/MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

ASP_FFADeathMatchGameMode::ASP_FFADeathMatchGameMode()
{
	FactionManagerComponent->AllianceMode = EAllianceMode::FFA;
}

void ASP_FFADeathMatchGameMode::BeginPlay()
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
				AllSpawnPoints.Add(SpawningPoint);
			}
		}
	}
	
	if (const UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		// Spawn a blue player + number of red bots where there are spawn point
		for (int i=0; i<(1+MainGameInstance->NbRedBots); i++)
		{
			const int SpawnIndex = FMath::RandRange(0, AllSpawnPoints.Num()-1);
			const ABaseSpawningPoint* CurrentSpawningPoint = AllSpawnPoints[SpawnIndex];
			
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
					CurrentSpawningPoint->RedTeamShooterCharacterClass,
					CurrentSpawningPoint->GetActorLocation(),
					CurrentSpawningPoint->GetActorRotation()
				);
				
				if (ASP_ShooterPlayerController* PlayerController = Cast<ASP_ShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					PlayerController->AddOHHealthBar(ShooterCharacter);
				}
			}
				
			AllSpawnPoints.RemoveAt(SpawnIndex);
		}
	}
}

void ASP_FFADeathMatchGameMode::OnShooterCharacterDeath(ABaseShooterCharacter* DeadShooterCharacter)
{
	Super::OnShooterCharacterDeath(DeadShooterCharacter);

	NbShooterAlive--;
	if (NbShooterAlive == 1)
	{
		EndGame();
	}
}

void ASP_FFADeathMatchGameMode::AddShooterCharacterCount(ABaseShooterCharacter* ShooterCharacterToRegister)
{
	Super::AddShooterCharacterCount(ShooterCharacterToRegister);
	NbShooterAlive++;
}

void ASP_FFADeathMatchGameMode::EndGame()
{
	if (ASP_ShooterPlayerController* PlayerController = Cast<ASP_ShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ASP_ShooterCharacter* ShooterCharacter = Cast<ASP_ShooterCharacter>(PlayerController->GetPawn()))
		{
			PlayerController->GameOver(WinScreenClass);
		}

		if (ASP_ShooterSpectatorPawn* ShooterSpectator = Cast<ASP_ShooterSpectatorPawn>(PlayerController->GetPawn()))
		{
			PlayerController->GameOver(LoseScreenClass);
		}
	}
}
