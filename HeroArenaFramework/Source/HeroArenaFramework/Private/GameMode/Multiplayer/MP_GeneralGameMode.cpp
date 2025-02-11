// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Multiplayer/MP_GeneralGameMode.h"

AMP_GeneralGameMode::AMP_GeneralGameMode()
{
	bUseSeamlessTravel = true;
}

void AMP_GeneralGameMode::StartMultiplayerGame(FString LevelName)
{
	if (HasAuthority())
	{
		FString LevelPath = "/Game/Levels/Multiplayer/";
		LevelPath.Append(LevelName);
		LevelPath.Append("?listen");
		
		GetWorld()->ServerTravel(LevelPath);
	}
}
