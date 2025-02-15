// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/Multiplayer/ShooterGameMode/MP_TeamDeathMatchGameMode.h"

#include "Actors/BaseSpawningPoint.h"
#include "Actors/Multiplayer/MP_HeroCharacter.h"
#include "Controllers/Multiplayer/MP_HeroAIController.h"
#include "Controllers/Multiplayer/MP_HeroPlayerController.h"
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
			AMP_HeroCharacter* ShooterCharacter = nullptr;
			if (BotData.Team == ETeam::BlueTeam)
			{
				const int SpawnIndex = FMath::RandRange(0, AllBlueSpawnPoints.Num()-1);
				const ABaseSpawningPoint* CurrentSpawningPoint = AllBlueSpawnPoints[SpawnIndex];
		
				ShooterCharacter = WorldPtr->SpawnActor<AMP_HeroCharacter>(
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
		
				ShooterCharacter = WorldPtr->SpawnActor<AMP_HeroCharacter>(
					CurrentSpawningPoint->RedTeamShooterCharacterClass,
					CurrentSpawningPoint->GetActorLocation(),
					CurrentSpawningPoint->GetActorRotation()
				);
			
				AllRedSpawnPoints.RemoveAt(SpawnIndex);
			}
			
			if (ShooterCharacter != nullptr)
			{
				//Need to create an MP version of AIController
				AMP_HeroAIController* BotController = WorldPtr->SpawnActor<AMP_HeroAIController>(
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

		AMP_HeroCharacter* ShooterCharacter = nullptr;
		
		if (PlayerState->Team == ETeam::BlueTeam)
		{
			const int SpawnIndex = FMath::RandRange(0, AllBlueSpawnPoints.Num()-1);
			const ABaseSpawningPoint* CurrentSpawningPoint = AllBlueSpawnPoints[SpawnIndex];
		
			ShooterCharacter = WorldPtr->SpawnActor<AMP_HeroCharacter>(
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
		
			ShooterCharacter = WorldPtr->SpawnActor<AMP_HeroCharacter>(
				CurrentSpawningPoint->RedTeamShooterCharacterClass,
				CurrentSpawningPoint->GetActorLocation(),
				CurrentSpawningPoint->GetActorRotation()
			);
			
			AllRedSpawnPoints.RemoveAt(SpawnIndex);
		}

		if (ShooterCharacter != nullptr)
		{
			if (AMP_HeroPlayerController* ShooterPlayerController = Cast<AMP_HeroPlayerController>(NewPlayer))
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
