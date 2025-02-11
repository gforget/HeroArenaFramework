// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/Multiplayer/ShooterGameMode/MP_TeamDeathMatchGameMode.h"

#include "Actors/BaseSpawningPoint.h"
#include "Actors/Multiplayer/MP_ShooterCharacter.h"
#include "Controllers/Multiplayer/MP_ShooterAIController.h"
#include "Controllers/Multiplayer/MP_ShooterPlayerController.h"
#include "GameMode/MainGameInstance.h"
#include "GameMode/Multiplayer/MP_ShooterPlayerState.h"

AMP_TeamDeathMatchGameMode::AMP_TeamDeathMatchGameMode()
{
	FactionManagerComponent->AllianceMode = EAllianceMode::Team;
}

void AMP_TeamDeathMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	//Instantiate the bots
	UWorld* WorldPtr = GetWorld();
	if (UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		for (FBotData BotData : GameInstance->BotDataStructs)
		{
			AMP_ShooterCharacter* ShooterCharacter = nullptr;
			if (BotData.Team == ETeam::BlueTeam)
			{
				const int SpawnIndex = FMath::RandRange(0, AllBlueSpawnPoints.Num()-1);
				const ABaseSpawningPoint* CurrentSpawningPoint = AllBlueSpawnPoints[SpawnIndex];
		
				ShooterCharacter = WorldPtr->SpawnActor<AMP_ShooterCharacter>(
					CurrentSpawningPoint->BlueTeamShooterCharacterClass,
					CurrentSpawningPoint->GetActorLocation(),
					CurrentSpawningPoint->GetActorRotation()
				);
			
				AllBlueSpawnPoints.RemoveAt(SpawnIndex);
			}
			else if (BotData.Team == ETeam::RedTeam)
			{
				const int SpawnIndex = FMath::RandRange(0, AllRedSpawnPoints.Num()-1);
				const ABaseSpawningPoint* CurrentSpawningPoint = AllRedSpawnPoints[SpawnIndex];
		
				ShooterCharacter = WorldPtr->SpawnActor<AMP_ShooterCharacter>(
					CurrentSpawningPoint->RedTeamShooterCharacterClass,
					CurrentSpawningPoint->GetActorLocation(),
					CurrentSpawningPoint->GetActorRotation()
				);
			
				AllRedSpawnPoints.RemoveAt(SpawnIndex);
			}
			
			if (ShooterCharacter != nullptr)
			{
				//Need to create an MP version of AIController
				AMP_ShooterAIController* BotController = WorldPtr->SpawnActor<AMP_ShooterAIController>(
					ShooterAIController,
					FVector::Zero(),
					FRotator::ZeroRotator
					);
			
				if (BotController)
				{
					BotController->Possess(ShooterCharacter);
				}
			}
		}
	}
}

void AMP_TeamDeathMatchGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	UWorld* WorldPtr = GetWorld();
	if (UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		AMP_ShooterPlayerState* PlayerState = NewPlayer->GetPlayerState<AMP_ShooterPlayerState>();
		FPlayerStateData PlayerStateData = GameInstance->PlayerStateDataStructs[PlayerState->GetUniqueId().ToString()];
		PlayerState->Team = PlayerStateData.Team;

		AMP_ShooterCharacter* ShooterCharacter = nullptr;
		
		if (PlayerState->Team == ETeam::BlueTeam)
		{
			const int SpawnIndex = FMath::RandRange(0, AllBlueSpawnPoints.Num()-1);
			const ABaseSpawningPoint* CurrentSpawningPoint = AllBlueSpawnPoints[SpawnIndex];
		
			ShooterCharacter = WorldPtr->SpawnActor<AMP_ShooterCharacter>(
				CurrentSpawningPoint->BlueTeamShooterCharacterClass,
				CurrentSpawningPoint->GetActorLocation(),
				CurrentSpawningPoint->GetActorRotation()
			);
			
			AllBlueSpawnPoints.RemoveAt(SpawnIndex);
		}
		else if (PlayerState->Team == ETeam::RedTeam)
		{
			const int SpawnIndex = FMath::RandRange(0, AllRedSpawnPoints.Num()-1);
			const ABaseSpawningPoint* CurrentSpawningPoint = AllRedSpawnPoints[SpawnIndex];
		
			ShooterCharacter = WorldPtr->SpawnActor<AMP_ShooterCharacter>(
				CurrentSpawningPoint->RedTeamShooterCharacterClass,
				CurrentSpawningPoint->GetActorLocation(),
				CurrentSpawningPoint->GetActorRotation()
			);
			
			AllRedSpawnPoints.RemoveAt(SpawnIndex);
		}

		if (ShooterCharacter != nullptr)
		{
			if (AMP_ShooterPlayerController* ShooterPlayerController = Cast<AMP_ShooterPlayerController>(NewPlayer))
			{
				ShooterPlayerController->Possess(ShooterCharacter);
			}
		}
	}
}

void AMP_TeamDeathMatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
