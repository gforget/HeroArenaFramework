// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseShooterSpectatorPawn.h"


// Sets default values
ABaseShooterSpectatorPawn::ABaseShooterSpectatorPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ETeam ABaseShooterSpectatorPawn::GetTeam()
{
	return Team;
}

void ABaseShooterSpectatorPawn::SetTeam(ETeam TeamValue)
{
	Team = TeamValue;
}
