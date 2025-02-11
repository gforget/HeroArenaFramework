// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/SinglePlayer/SP_ShooterCharacter.h"
#include "Actors/SinglePlayer/SP_Gun.h"
#include "GameMode/SinglePlayer/SP_ShooterGameMode.h"
#include "PlayMontageCallbackProxy.h"
#include "Actors/SinglePlayer/SP_ShooterSpectatorPawn.h"
#include "Controllers/SinglePlayer/SP_ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASP_ShooterCharacter::ASP_ShooterCharacter()
{
	
}

// Called when the game starts or when spawned
void ASP_ShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (ASP_ShooterGameMode* ShooterGameMode = Cast<ASP_ShooterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		ShooterGameMode->RegisterEvent(this);
		ShooterGameMode->AddShooterCharacterCount(this);

		if (ShooterGameMode->bBasicOHHealthBarAssociation)
		{
			if (ASP_ShooterPlayerController* PlayerController = Cast<ASP_ShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
			{
				if (Cast<ASP_ShooterCharacter>(PlayerController->GetPawn()) != this)
				{
					PlayerController->AddOHHealthBar(this);
				}
			}
		}
	}
}

// Called every frame
void ASP_ShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASP_ShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) //overriden
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("SelfDamage"), IE_Pressed, this, &ASP_ShooterCharacter::SelfDamage);
	PlayerInputComponent->BindAction(TEXT("ToggleDebugSpectatorMode"), IE_Pressed, this, &ASP_ShooterCharacter::ActivateDebugSpectatorMode);
}

float ASP_ShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	
	float DamageToApply =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//Health is modified in BaseShooterCharacter::TakeDamage
	if (Health <= 0.0f)
	{
		Death();
	}
	
	return DamageToApply;
}

void ASP_ShooterCharacter::PullTrigger() //overriden
{
	if (Gun != nullptr)
	{
		Gun->PullTrigger();
	}
}

void ASP_ShooterCharacter::ReleaseTrigger() //overriden
{
	if (Gun != nullptr)
	{
		Gun->ReleaseTrigger();	
	}
}

void ASP_ShooterCharacter::Reload() //overriden
{	
	if (!IsReloading && AmmoReserve > 0 && Gun->GetAmmoPercent() < 1.0f )
	{
		ProxyReloadPlayMontage = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
			GetMesh(),
			ReloadMontage
		);
		
		ProxyReloadPlayMontage->OnCompleted.AddDynamic(this, &ABaseShooterCharacter::OnReloadAnimationCompleted);
		IsReloading = true;
	}
	else
	{
		//TODO: clip sound
	}
}

void ASP_ShooterCharacter::ActivateDebugSpectatorMode()
{
	ASP_ShooterPlayerController* ShooterPlayerController = Cast<ASP_ShooterPlayerController>(GetController());
	if (ShooterPlayerController != nullptr)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		GEngine->Exec(GetWorld(), TEXT("r.MotionBlurQuality 0"));
		
		ASP_ShooterSpectatorPawn* ShooterSpectatorPawn = GetWorld()->SpawnActor<ASP_ShooterSpectatorPawn>(
			ShooterSpectatorPawnClass,
			GetActorLocation(),
			GetActorRotation()
		);
		
		UnPossessed();
		ShooterSpectatorPawn->SetPlayerShooterCharacter(this);
		ShooterPlayerController->SetTickableWhenPaused(true);
		ShooterSpectatorPawn->SetTickableWhenPaused(true);
		ShooterPlayerController->Possess(ShooterSpectatorPawn);
	}
}

void ASP_ShooterCharacter::SelfDamage()
{
	Health -= 10.0f;
}