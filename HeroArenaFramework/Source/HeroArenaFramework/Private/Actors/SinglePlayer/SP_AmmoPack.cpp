// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/SinglePlayer/SP_AmmoPack.h"
#include "GameMode/SinglePlayer/SP_BaseGameMode.h"

// Called when the game starts or when spawned
void ASP_AmmoPack::BeginPlay()
{
	Super::BeginPlay();
	ASP_BaseGameMode* GameModeBase = GetWorld()->GetAuthGameMode<ASP_BaseGameMode>();
	if (GameModeBase != nullptr)
	{
		GameModeBase->AddAmmoPack(this);
	}
}
