// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Multiplayer/MP_HealthPack.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameMode.h"

// Called when the game starts or when spawned
void AMP_HealthPack::BeginPlay()
{
	Super::BeginPlay();
	AMP_ShooterGameMode* GameModeBase = GetWorld()->GetAuthGameMode<AMP_ShooterGameMode>();
	if (GameModeBase != nullptr)
	{
		GameModeBase->AddHealthPack(this);
	}
}