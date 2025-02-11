// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/Multiplayer/ShooterGameMode/MP_FFADeathMatchGameMode.h"

#include "Actors/BaseSpawningPoint.h"
#include "Actors/Multiplayer/MP_ShooterCharacter.h"
#include "Controllers/Multiplayer/MP_ShooterAIController.h"
#include "Controllers/Multiplayer/MP_ShooterPlayerController.h"
#include "GameMode/MainGameInstance.h"

AMP_FFADeathMatchGameMode::AMP_FFADeathMatchGameMode()
{
	FactionManagerComponent->AllianceMode = EAllianceMode::FFA;
}

void AMP_FFADeathMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	//Instantiate the bots
	UWorld* WorldPtr = GetWorld();
	if (UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		for (FBotData BotData : GameInstance->BotDataStructs)
		{
			AMP_ShooterCharacter* ShooterCharacter = nullptr;

			const int SpawnIndex = FMath::RandRange(0, AllSpawnPoints.Num()-1);
			const ABaseSpawningPoint* CurrentSpawningPoint = AllSpawnPoints[SpawnIndex];
	
			ShooterCharacter = WorldPtr->SpawnActor<AMP_ShooterCharacter>(
				CurrentSpawningPoint->RedTeamShooterCharacterClass,
				CurrentSpawningPoint->GetActorLocation(),
				CurrentSpawningPoint->GetActorRotation()
			);
		
			AllSpawnPoints.RemoveAt(SpawnIndex);
			
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

void AMP_FFADeathMatchGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	UWorld* WorldPtr = GetWorld();
	if (UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		const int SpawnIndex = FMath::RandRange(0, AllSpawnPoints.Num()-1);
		const ABaseSpawningPoint* CurrentSpawningPoint = AllSpawnPoints[SpawnIndex];
	
		AMP_ShooterCharacter* ShooterCharacter = WorldPtr->SpawnActor<AMP_ShooterCharacter>(
			CurrentSpawningPoint->RedTeamShooterCharacterClass,
			CurrentSpawningPoint->GetActorLocation(),
			CurrentSpawningPoint->GetActorRotation()
		);
		
		if (ShooterCharacter != nullptr)
		{
			if (AMP_ShooterPlayerController* ShooterPlayerController = Cast<AMP_ShooterPlayerController>(NewPlayer))
			{
				ShooterPlayerController->Possess(ShooterCharacter);
			}
		}
		
		AllSpawnPoints.RemoveAt(SpawnIndex);
	}
}

void AMP_FFADeathMatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
