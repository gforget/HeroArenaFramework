// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MainGameInstance.h"
#include "GameMode/Multiplayer/MP_ShooterPlayerState.h"

void UMainGameInstance::FeedPlayerStateDataAndBotData(
	TArray<AMP_ShooterPlayerState*> PlayerStateArray,
	TArray<FBotData> BotDataArray)
{
	for (int i = 0; i < PlayerStateArray.Num(); i++)
	{
		if (AMP_ShooterPlayerState* PlayerState = PlayerStateArray[i])
		{
			FString UniqueNetId = PlayerState->GetUniqueId().ToString();
			ETeam Team = PlayerState->Team;
			
			FPlayerStateData PlayerStateData(Team);
			PlayerStateDataStructs.Add(UniqueNetId, PlayerStateData);
		}
	}

	BotDataStructs = BotDataArray;
}

void UMainGameInstance::CallBPLeaveSession_Implementation()
{
}
