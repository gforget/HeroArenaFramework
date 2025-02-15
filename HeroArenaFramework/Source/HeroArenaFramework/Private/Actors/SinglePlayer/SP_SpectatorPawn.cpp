// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/SinglePlayer/SP_SpectatorPawn.h"
#include "Actors/SinglePlayer/SP_HeroCharacter.h"
#include "Controllers/SinglePlayer/SP_HeroPlayerController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ASP_SpectatorPawn::ASP_SpectatorPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

// Called when the game starts or when spawned
void ASP_SpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	if (MovementComponent)
	{
		MovementComponent->PrimaryComponentTick.bTickEvenWhenPaused = true;
	}
}

// Called to bind functionality to input
void ASP_SpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("ToggleDebugSpectatorMode"), IE_Pressed, this, &ASP_SpectatorPawn::ReturnToPlayerMode);
	PlayerInputComponent->BindAction(TEXT("SkipFrame"), IE_Pressed, this, &ASP_SpectatorPawn::SkipFrame);
}

void ASP_SpectatorPawn::ReturnToPlayerMode()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	
		GEngine->Exec(GetWorld(), TEXT("r.MotionBlurQuality 0.5"));
		ASP_HeroPlayerController* ShooterPlayerController = Cast<ASP_HeroPlayerController>(GetController());
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
		ASP_HeroPlayerController* ShooterPlayerController = Cast<ASP_HeroPlayerController>(GetController());
		if (ShooterPlayerController != nullptr)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			GEngine->Exec(GetWorld(), TEXT("r.MotionBlurQuality 0"));
			ShooterPlayerController->SetTickableWhenPaused(true);
			SetTickableWhenPaused(true);
		}
	}
}

void ASP_SpectatorPawn::SkipFrame()
{
	StartFrameTimer();
}

void ASP_SpectatorPawn::StartFrameTimer()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASP_SpectatorPawn::PauseGame);
}

void ASP_SpectatorPawn::PauseGame()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		return;
	}
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ASP_SpectatorPawn::SetPlayerShooterCharacter(ASP_HeroCharacter* PlayerShooterCharacterRef)
{
	PlayerShooterCharacter = PlayerShooterCharacterRef;
}

