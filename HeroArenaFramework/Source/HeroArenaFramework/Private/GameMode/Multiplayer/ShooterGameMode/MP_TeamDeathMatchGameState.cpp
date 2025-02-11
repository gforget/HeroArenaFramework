// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Multiplayer/ShooterGameMode/MP_TeamDeathMatchGameState.h"
#include "Actors/Multiplayer/MP_ShooterCharacter.h"
#include "Controllers/Multiplayer/MP_ShooterPlayerController.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameMode.h"

void AMP_TeamDeathMatchGameState::AddShooterCharacterCount(AMP_ShooterCharacter* ShooterCharacterToRegister)
{
	if (!TeamCount.Contains(ShooterCharacterToRegister->GetTeam()))
	{
		TeamCount.Add(ShooterCharacterToRegister->GetTeam(), 0);
	}
		
	TeamCount[ShooterCharacterToRegister->GetTeam()]++;
}

void AMP_TeamDeathMatchGameState::OnShooterCharacterDeath(ABaseShooterCharacter* DeadShooterCharacter)
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
	if (AMP_ShooterPlayerController* LocalShooterController = Cast<AMP_ShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
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
		if (AMP_ShooterGameMode* ShooterGameMode = Cast<AMP_ShooterGameMode>(GetWorld()->GetAuthGameMode()))
		{
			ShooterGameMode->CallLeaveSession();
		}
	}
}
