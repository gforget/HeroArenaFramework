// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Multiplayer/ShooterGameMode/MP_BaseGameState.h"

#include "Actors/Multiplayer/MP_HeroCharacter.h"

void AMP_BaseGameState::OnShooterCharacterDeath(ABaseHeroCharacter* DeadShooterCharacter)
{
}

void AMP_BaseGameState::AddShooterCharacterCount(AMP_HeroCharacter* ShooterCharacterToRegister)
{
}

void AMP_BaseGameState::RegisterEvent(AMP_HeroCharacter* ShooterCharacterRef)
{
	ShooterCharacterRef->OnDeadEvent.AddDynamic(this, &AMP_BaseGameState::OnShooterCharacterDeath);
}
