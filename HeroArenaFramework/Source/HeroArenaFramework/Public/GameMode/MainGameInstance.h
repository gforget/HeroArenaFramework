// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/TeamManager.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

class AMP_ShooterPlayerState;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerStateData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ETeam> Team = ETeam::BlueTeam;

	FPlayerStateData(){};
	
	FPlayerStateData(TEnumAsByte<ETeam> _Team)
	{
		Team = _Team;
	}
};

USTRUCT(BlueprintType)
struct FBotData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ETeam> Team = ETeam::BlueTeam;

	FBotData(){};
	
	FBotData(TEnumAsByte<ETeam> _Team)
	{
		Team = _Team;
	}
};

UCLASS()
class HEROARENAFRAMEWORK_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EAllianceMode> AllianceMode;

	//Multiplayer Data
	//FString is the UniqueNedId convert into a string
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, FPlayerStateData> PlayerStateDataStructs;

	UPROPERTY(BlueprintReadWrite)
	TArray<FBotData> BotDataStructs;
	
	//Single Player Data
	UPROPERTY()
	int NbRedBots = 0;
	
	UPROPERTY()
	int NbBlueBots = 0;
	
	UFUNCTION(BlueprintCallable)
	void FeedPlayerStateDataAndBotData(TArray<AMP_ShooterPlayerState*> PlayerStateArray, TArray<FBotData> BotDataArray);

	//TODO: will have to create session implementation in C++ entirely later
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Main Events")
	void CallBPLeaveSession();
};
