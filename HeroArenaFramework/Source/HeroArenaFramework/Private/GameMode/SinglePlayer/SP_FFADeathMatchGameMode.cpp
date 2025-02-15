// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SinglePlayer/SP_FFADeathMatchGameMode.h"

#include "Actors/BaseSpawningPoint.h"
#include "Actors/SinglePlayer/SP_HeroCharacter.h"
#include "Actors/SinglePlayer/SP_SpectatorPawn.h"
#include "Controllers/SinglePlayer/SP_HeroPlayerController.h"
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
				ASP_HeroCharacter* ShooterCharacter = WorldPtr->SpawnActor<ASP_HeroCharacter>(
					CurrentSpawningPoint->BlueTeamShooterCharacterClass,
					CurrentSpawningPoint->GetActorLocation(),
					CurrentSpawningPoint->GetActorRotation()
				);
				
				if (ASP_HeroPlayerController* PlayerController = Cast<ASP_HeroPlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					PlayerController->Possess(ShooterCharacter);
					PlayerController->InstantiateGameModeHUD(GameModeHUDClass);
				}
			}
			else
			{
				ASP_HeroCharacter* ShooterCharacter = WorldPtr->SpawnActor<ASP_HeroCharacter>(
					CurrentSpawningPoint->RedTeamShooterCharacterClass,
					CurrentSpawningPoint->GetActorLocation(),
					CurrentSpawningPoint->GetActorRotation()
				);
				
				if (ASP_HeroPlayerController* PlayerController = Cast<ASP_HeroPlayerController>(GetWorld()->GetFirstPlayerController()))
				{
					PlayerController->AddOHHealthBar(ShooterCharacter);
				}
			}
				
			AllSpawnPoints.RemoveAt(SpawnIndex);
		}
	}
}

void ASP_FFADeathMatchGameMode::OnShooterCharacterDeath(ABaseHeroCharacter* DeadShooterCharacter)
{
	Super::OnShooterCharacterDeath(DeadShooterCharacter);

	NbShooterAlive--;
	if (NbShooterAlive == 1)
	{
		EndGame();
	}
}

void ASP_FFADeathMatchGameMode::AddShooterCharacterCount(ABaseHeroCharacter* ShooterCharacterToRegister)
{
	Super::AddShooterCharacterCount(ShooterCharacterToRegister);
	NbShooterAlive++;
}

void ASP_FFADeathMatchGameMode::EndGame()
{
	if (ASP_HeroPlayerController* PlayerController = Cast<ASP_HeroPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ASP_HeroCharacter* ShooterCharacter = Cast<ASP_HeroCharacter>(PlayerController->GetPawn()))
		{
			PlayerController->GameOver(WinScreenClass);
		}

		if (ASP_SpectatorPawn* ShooterSpectator = Cast<ASP_SpectatorPawn>(PlayerController->GetPawn()))
		{
			PlayerController->GameOver(LoseScreenClass);
		}
	}
}
