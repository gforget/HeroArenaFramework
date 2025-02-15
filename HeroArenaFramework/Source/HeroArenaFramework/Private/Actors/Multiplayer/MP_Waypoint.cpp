// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Multiplayer/MP_Waypoint.h"

#include "GameMode/Multiplayer/ShooterGameMode/MP_BaseGameMode.h"


// Sets default values
AMP_Waypoint::AMP_Waypoint()
{
}

// Called when the game starts or when spawned
void AMP_Waypoint::BeginPlay()
{
	Super::BeginPlay();
	AMP_BaseGameMode* GameModeBase = GetWorld()->GetAuthGameMode<AMP_BaseGameMode>();
	if (GameModeBase != nullptr)
	{
		GameModeBase->AddWayPoint(this);
	}
}

