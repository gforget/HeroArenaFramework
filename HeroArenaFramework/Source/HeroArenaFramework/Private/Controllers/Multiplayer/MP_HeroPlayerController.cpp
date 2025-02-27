// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/Multiplayer/MP_HeroPlayerController.h"

#include "Actors/Multiplayer/MP_HeroCharacter.h"
#include "Actors/Multiplayer/MP_SpectatorPawn.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_BaseGameState.h"
#include "UI/GameModeHUD.h"
#include "UI/Multiplayer/MP_OHHealthBar.h"
#include "UI/PlayerHUD.h"

AMP_HeroPlayerController::AMP_HeroPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AMP_HeroPlayerController::CallOnPossess_Implementation()
{
	DelayedPossessCheck();
}

void AMP_HeroPlayerController::BeginPlay()
{
	Super::BeginPlay();
	InstantiateHUD(GetPawn());
}

void AMP_HeroPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	InstantiateHUD(InPawn);
	if (HasAuthority())
	{
		CallOnPossess();
	}
}

void AMP_HeroPlayerController::DelayedPossessCheck()
{
	if (GetPawn())
	{
		InstantiateHUD(GetPawn());
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(DelayedPossessTimerHandle, this, &AMP_HeroPlayerController::DelayedPossessCheck, 0.1f, false);
	}
}

void AMP_HeroPlayerController::InstantiateHUD(APawn* InPawn)
{
	if (IsLocalController())
	{
		AMP_HeroCharacter* ShooterCharacter = Cast<AMP_HeroCharacter>(InPawn);
		AMP_SpectatorPawn* ShooterSpectator = Cast<AMP_SpectatorPawn>(InPawn);

		if (ShooterCharacter != nullptr || ShooterSpectator != nullptr)
		{
			if (CharacterHUD == nullptr)
			{
				CharacterHUD = Cast<UPlayerHUD>(CreateWidget(this, ShooterCharacter->CharacterHUDScreenClass));
				if (CharacterHUD != nullptr)
				{
					CharacterHUD->AddToViewport();
				}
			}
		
			if (CharacterHUD != nullptr)
			{
				if (ShooterCharacter != nullptr)
				{
					AssignedShooterCharacter = ShooterCharacter;
					CharacterHUD->OnPlayerModeEvent();
				}
				else if (ShooterSpectator != nullptr)
				{
					CharacterHUD->OnSpectatorModeEvent();
				}
			}

			if (GameModeHUD == nullptr)
			{
				if (AMP_BaseGameState* ShooterGameState = Cast<AMP_BaseGameState>(GetWorld()->GetGameState()))
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


