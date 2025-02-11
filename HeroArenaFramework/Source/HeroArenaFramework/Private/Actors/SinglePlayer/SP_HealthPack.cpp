// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/SinglePlayer/SP_HealthPack.h"
#include "GameMode/SinglePlayer/SP_ShooterGameMode.h"

// Called when the game starts or when spawned
void ASP_HealthPack::BeginPlay()
{
	Super::BeginPlay();

	ASP_ShooterGameMode* GameModeBase = GetWorld()->GetAuthGameMode<ASP_ShooterGameMode>();
	if (GameModeBase != nullptr)
	{
		GameModeBase->AddHealthPack(this);
	}
}
