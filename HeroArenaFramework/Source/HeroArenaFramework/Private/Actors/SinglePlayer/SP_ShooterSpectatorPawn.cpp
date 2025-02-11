// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/SinglePlayer/SP_ShooterSpectatorPawn.h"
#include "Actors/SinglePlayer/SP_ShooterCharacter.h"
#include "Controllers/SinglePlayer/SP_ShooterPlayerController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ASP_ShooterSpectatorPawn::ASP_ShooterSpectatorPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

// Called when the game starts or when spawned
void ASP_ShooterSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	if (MovementComponent)
	{
		MovementComponent->PrimaryComponentTick.bTickEvenWhenPaused = true;
	}
}

// Called to bind functionality to input
void ASP_ShooterSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("ToggleDebugSpectatorMode"), IE_Pressed, this, &ASP_ShooterSpectatorPawn::ReturnToPlayerMode);
	PlayerInputComponent->BindAction(TEXT("SkipFrame"), IE_Pressed, this, &ASP_ShooterSpectatorPawn::SkipFrame);
}

void ASP_ShooterSpectatorPawn::ReturnToPlayerMode()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	
		GEngine->Exec(GetWorld(), TEXT("r.MotionBlurQuality 0.5"));
		ASP_ShooterPlayerController* ShooterPlayerController = Cast<ASP_ShooterPlayerController>(GetController());
		if (ShooterPlayerController != nullptr)
		{
			if (PlayerShooterCharacter)
			{
				UnPossessed();
				ShooterPlayerController->Possess(PlayerShooterCharacter);
				ShooterPlayerController->SetTickableWhenPaused(false);
				Destroy();
			}
			else
			{
				ShooterPlayerController->SetTickableWhenPaused(false);
			}
		}
	}
	else
	{
		ASP_ShooterPlayerController* ShooterPlayerController = Cast<ASP_ShooterPlayerController>(GetController());
		if (ShooterPlayerController != nullptr)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			GEngine->Exec(GetWorld(), TEXT("r.MotionBlurQuality 0"));
			ShooterPlayerController->SetTickableWhenPaused(true);
			SetTickableWhenPaused(true);
		}
	}
}

void ASP_ShooterSpectatorPawn::SkipFrame()
{
	StartFrameTimer();
}

void ASP_ShooterSpectatorPawn::StartFrameTimer()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASP_ShooterSpectatorPawn::PauseGame);
}

void ASP_ShooterSpectatorPawn::PauseGame()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		return;
	}
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ASP_ShooterSpectatorPawn::SetPlayerShooterCharacter(ASP_ShooterCharacter* PlayerShooterCharacterRef)
{
	PlayerShooterCharacter = PlayerShooterCharacterRef;
}

