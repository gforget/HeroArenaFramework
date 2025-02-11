// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SplashScreenUI.h"

#include "Components/VerticalBox.h"
#include "GameMode/MainGameInstance.h"
#include "GameMode/Multiplayer/MP_ShooterPlayerState.h"
#include "Kismet/GameplayStatics.h"


void USplashScreenUI::CallSinglePlayerMap(TEnumAsByte<EAllianceMode>  AllianceMode, int NbRedBots, int NbBlueBots, FName LevelName)
{
	if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		MainGameInstance->AllianceMode = AllianceMode;
		MainGameInstance->NbRedBots = NbRedBots;
		MainGameInstance->NbBlueBots = NbBlueBots;
		
		UGameplayStatics::OpenLevel(GetWorld(), LevelName, true);
	}
}

void USplashScreenUI::VerticalBoxReplaceChildAt(UVerticalBox* VerticalBox, int32 Index, UWidget* NewWidget)
{
    if (!VerticalBox || !NewWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid VerticalBox or NewWidget!"));
        return;
    }

    // Store all current widgets in a temporary array
    TArray<UWidget*> CurrentWidgets;
    const int32 ChildCount = VerticalBox->GetChildrenCount();
    for (int32 i = 0; i < ChildCount; i++)
    {
	    if (UWidget* ChildWidget = VerticalBox->GetChildAt(i))
        {
            CurrentWidgets.Add(ChildWidget);
        }
    }

    // Clear the VerticalBox (remove all children)
    VerticalBox->ClearChildren();

    // Rebuild the VerticalBox by adding widgets up to the target index
    for (int32 i = 0; i < CurrentWidgets.Num(); i++)
    {
		if (i==Index)
		{
			VerticalBox->AddChildToVerticalBox(NewWidget);
		}
		else
		{
			VerticalBox->AddChildToVerticalBox(CurrentWidgets[i]);
		}
    }

    // Refresh the layout of the VerticalBox to ensure proper rendering
    VerticalBox->InvalidateLayoutAndVolatility();

    UE_LOG(LogTemp, Warning, TEXT("New widget added at index %d."), Index);
}

ETeam USplashScreenUI::GetTeamEnumFromString(FString StringValue)
{
	if (StringValue == "Blue Team")
	{
		return ETeam::BlueTeam;
	}

	if (StringValue == "Red Team")
	{
		return ETeam::RedTeam;
	}

	return ETeam::NoTeam;
}

EAllianceMode USplashScreenUI::GetAllianceModeEnumFromString(FString StringValue)
{
	if (StringValue == "FFA")
	{
		return EAllianceMode::FFA;
	}

	if (StringValue == "Team")
	{
		return EAllianceMode::Team;
	}

	return EAllianceMode::None;
}

bool USplashScreenUI::ValidateTeamMatchUp(TArray<AMP_ShooterPlayerState*> PlayerStateArray, TArray<FBotData> BotDataArray)
{
	bool bFoundBlueMember = false;
	bool bFoundRedMember = false;
	
	for (int i=0; i < PlayerStateArray.Num(); i++)
	{
		if (!bFoundBlueMember)
		{
			bFoundBlueMember = PlayerStateArray[i]->Team == ETeam::BlueTeam;
		}

		if (!bFoundRedMember)
		{
			bFoundRedMember = PlayerStateArray[i]->Team == ETeam::RedTeam;
		}

		if (bFoundBlueMember && bFoundRedMember) break;
	}

	for (int i=0; i < BotDataArray.Num(); i++)
	{
		if (!bFoundBlueMember)
		{
			bFoundBlueMember = BotDataArray[i].Team == ETeam::BlueTeam;
		}

		if (!bFoundRedMember)
		{
			bFoundRedMember = BotDataArray[i].Team == ETeam::RedTeam;
		}
		
		if (bFoundBlueMember && bFoundRedMember) break;
	}
	
	return bFoundBlueMember && bFoundRedMember;
}

