// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameState.h"

#include "Actors/Multiplayer/MP_ShooterCharacter.h"

void AMP_ShooterGameState::OnShooterCharacterDeath(ABaseShooterCharacter* DeadShooterCharacter)
{
}

void AMP_ShooterGameState::AddShooterCharacterCount(AMP_ShooterCharacter* ShooterCharacterToRegister)
{
}

void AMP_ShooterGameState::RegisterEvent(AMP_ShooterCharacter* ShooterCharacterRef)
{
	ShooterCharacterRef->OnDeadEvent.AddDynamic(this, &AMP_ShooterGameState::OnShooterCharacterDeath);
}
