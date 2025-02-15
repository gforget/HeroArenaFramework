// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Multiplayer/ShooterGameMode/MP_FFADeathMatchGameState.h"

#include "Actors/Multiplayer/MP_HeroCharacter.h"
#include "Controllers/Multiplayer/MP_HeroPlayerController.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_BaseGameMode.h"

void AMP_FFADeathMatchGameState::AddShooterCharacterCount(AMP_HeroCharacter* ShooterCharacterToRegister)
{
	Super::AddShooterCharacterCount(ShooterCharacterToRegister);
	ShooterCount++;
}

void AMP_FFADeathMatchGameState::OnShooterCharacterDeath(ABaseHeroCharacter* DeadShooterCharacter)
{
	Super::OnShooterCharacterDeath(DeadShooterCharacter);
	ShooterCount--;
	if (ShooterCount <= 1)
	{
		EndGame();
	}
}

void AMP_FFADeathMatchGameState::EndGame()
{
	if (AMP_HeroPlayerController* LocalShooterController = Cast<AMP_HeroPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (!LocalShooterController->AssignedShooterCharacter->IsDead())
		{
			LocalShooterController->GameOver(PlayerWinScreenClass);
		}
		else
		{
			LocalShooterController->GameOver(PlayerLoseScreenClass);
		}
	}
	
	if (HasAuthority())
	{
		if (AMP_BaseGameMode* ShooterGameMode = Cast<AMP_BaseGameMode>(GetWorld()->GetAuthGameMode()))
		{
			ShooterGameMode->CallLeaveSession();
		}
	}
}
