// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/Multiplayer/MP_ShooterPlayerController.h"

#include "Actors/Multiplayer/MP_ShooterCharacter.h"
#include "Actors/Multiplayer/MP_ShooterSpectatorPawn.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameState.h"
#include "UI/GameModeHUD.h"
#include "UI/Multiplayer/MP_OHHealthBar.h"
#include "UI/PlayerHUD.h"

AMP_ShooterPlayerController::AMP_ShooterPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AMP_ShooterPlayerController::CallOnPossess_Implementation()
{
	DelayedPossessCheck();
}

void AMP_ShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	InstantiateHUD(GetPawn());
}

void AMP_ShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	InstantiateHUD(InPawn);
	if (HasAuthority())
	{
		CallOnPossess();
	}
}

void AMP_ShooterPlayerController::DelayedPossessCheck()
{
	if (GetPawn())
	{
		InstantiateHUD(GetPawn());
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(DelayedPossessTimerHandle, this, &AMP_ShooterPlayerController::DelayedPossessCheck, 0.1f, false);
	}
}

void AMP_ShooterPlayerController::InstantiateHUD(APawn* InPawn)
{
	if (IsLocalController())
	{
		AMP_ShooterCharacter* ShooterCharacter = Cast<AMP_ShooterCharacter>(InPawn);
		AMP_ShooterSpectatorPawn* ShooterSpectator = Cast<AMP_ShooterSpectatorPawn>(InPawn);

		if (ShooterCharacter != nullptr || ShooterSpectator != nullptr)
		{
			if (PlayerHUD == nullptr)
			{
				PlayerHUD = Cast<UPlayerHUD>(CreateWidget(this, HUDScreenClass));
				if (PlayerHUD != nullptr)
				{
					PlayerHUD->AddToViewport();
				}
			}
		
			if (PlayerHUD != nullptr)
			{
				if (ShooterCharacter != nullptr)
				{
					AssignedShooterCharacter = ShooterCharacter;
					PlayerHUD->OnPlayerModeEvent();
				}
				else if (ShooterSpectator != nullptr)
				{
					PlayerHUD->OnSpectatorModeEvent();
				}
			}

			if (GameModeHUD == nullptr)
			{
				if (AMP_ShooterGameState* ShooterGameState = Cast<AMP_ShooterGameState>(GetWorld()->GetGameState()))
				{
					if (ShooterGameState->GameModeHUDClass != nullptr)
					{
						GameModeHUD = Cast<UGameModeHUD>(CreateWidget(this, ShooterGameState->GameModeHUDClass));
						if (GameModeHUD != nullptr)
						{
							GameModeHUD->AddToViewport();
						}
					}
				}
			}
		}
	}
}


