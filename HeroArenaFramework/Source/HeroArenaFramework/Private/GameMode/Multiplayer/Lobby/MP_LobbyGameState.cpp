// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Multiplayer/Lobby/MP_LobbyGameState.h"
#include "GameMode/MainGameInstance.h"
#include "GameMode/Multiplayer/MP_ShooterPlayerState.h"

TArray<AMP_ShooterPlayerState*> AMP_LobbyGameState::ReplaceTeamInPlayerState(TArray<AMP_ShooterPlayerState*> AllPlayerStates,
                                                                             int IndexPlayerState, TEnumAsByte<ETeam> NewTeam)
{
	TArray<AMP_ShooterPlayerState*> NewPlayerStates = AllPlayerStates;
	NewPlayerStates[IndexPlayerState]->Team = NewTeam;
	
	return NewPlayerStates;
}

TArray<FBotData> AMP_LobbyGameState::ReplaceTeamInBotDatasArray(TArray<FBotData> BotDatasArray, int IndexBot,
	TEnumAsByte<ETeam> NewTeam)
{
	TArray<FBotData> NewBotsArray = BotDatasArray;
	NewBotsArray[IndexBot].Team = NewTeam;
	
	return NewBotsArray;
}
