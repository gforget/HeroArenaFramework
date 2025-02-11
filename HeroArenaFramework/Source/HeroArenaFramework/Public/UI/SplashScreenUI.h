// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/TeamManager.h"
#include "Blueprint/UserWidget.h"
#include "SplashScreenUI.generated.h"

class UVerticalBox;
/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API USplashScreenUI : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Main Functions")
	void CallSinglePlayerMap(TEnumAsByte<EAllianceMode> AllianceMode, int NbRedBots, int NbBlueBots, FName LevelName);

	UFUNCTION(BlueprintCallable, Category = "Main Functions")
	void VerticalBoxReplaceChildAt(UVerticalBox* VerticalBox, int32 Index, UWidget* NewWidget);

	UFUNCTION(BlueprintCallable, Category="Main Functions")
	ETeam GetTeamEnumFromString(FString StringValue);

	UFUNCTION(BlueprintCallable, Category="Main Functions")
	EAllianceMode GetAllianceModeEnumFromString(FString StringValue);
	
	UFUNCTION(BlueprintCallable, Category="Main Functions")
	bool ValidateTeamMatchUp(TArray<AMP_ShooterPlayerState*> PlayerStateArray, TArray<FBotData> BotDataArray);
};
