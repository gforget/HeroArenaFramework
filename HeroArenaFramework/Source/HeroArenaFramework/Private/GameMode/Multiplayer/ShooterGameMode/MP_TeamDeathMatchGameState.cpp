// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Multiplayer/ShooterGameMode/MP_TeamDeathMatchGameState.h"
#include "Actors/Multiplayer/MP_HeroCharacter.h"
#include "Controllers/Multiplayer/MP_HeroPlayerController.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_BaseGameMode.h"

void AMP_TeamDeathMatchGameState::AddShooterCharacterCount(AMP_HeroCharacter* ShooterCharacterToRegister)
{
	if (!TeamCount.Contains(ShooterCharacterToRegister->GetTeam()))
	{
		TeamCount.Add(ShooterCharacterToRegister->GetTeam(), 0);
	}
		
	TeamCount[ShooterCharacterToRegister->GetTeam()]++;
}

void AMP_TeamDeathMatchGameState::OnShooterCharacterDeath(ABaseHeroCharacter* DeadShooterCharacter)
{
	Super::OnShooterCharacterDeath(DeadShooterCharacter);
	
	TeamCount[DeadShooterCharacter->GetTeam()]--;
	if (TeamCount[DeadShooterCharacter->GetTeam()] == 0)
	{
		const ETeam WinningTeam = DeadShooterCharacter->GetTeam() == ETeam::RedTeam ? ETeam::BlueTeam : ETeam::RedTeam; 
		EndGame(WinningTeam);
	}
}

void AMP_TeamDeathMatchGameState::EndGame(ETeam WinningTeam)
{
	if (AMP_HeroPlayerController* LocalShooterController = Cast<AMP_HeroPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (WinningTeam == ETeam::BlueTeam)
		{
			LocalShooterController->GameOver(BlueWinScreenClass);
		}
		else
		{
			LocalShooterController->GameOver(RedWinScreenClass);
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
