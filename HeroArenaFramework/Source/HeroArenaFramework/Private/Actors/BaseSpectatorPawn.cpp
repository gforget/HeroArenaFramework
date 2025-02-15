// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseSpectatorPawn.h"


// Sets default values
ABaseSpectatorPawn::ABaseSpectatorPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ETeam ABaseSpectatorPawn::GetTeam()
{
	return Team;
}

void ABaseSpectatorPawn::SetTeam(ETeam TeamValue)
{
	Team = TeamValue;
}
