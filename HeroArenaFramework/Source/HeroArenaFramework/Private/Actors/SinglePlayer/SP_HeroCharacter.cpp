// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/SinglePlayer/SP_HeroCharacter.h"
#include "GameMode/SinglePlayer/SP_BaseGameMode.h"
#include "PlayMontageCallbackProxy.h"
#include "ActorComponents/BaseHeroAbility.h"
#include "Actors/SinglePlayer/SP_SpectatorPawn.h"
#include "Controllers/SinglePlayer/SP_HeroPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASP_HeroCharacter::ASP_HeroCharacter()
{
	
}

// Called when the game starts or when spawned
void ASP_HeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (ASP_BaseGameMode* ShooterGameMode = Cast<ASP_BaseGameMode>(GetWorld()->GetAuthGameMode()))
	{
		ShooterGameMode->RegisterEvent(this);
		ShooterGameMode->AddShooterCharacterCount(this);

		if (ShooterGameMode->bBasicOHHealthBarAssociation)
		{
			if (ASP_HeroPlayerController* PlayerController = Cast<ASP_HeroPlayerController>(GetWorld()->GetFirstPlayerController()))
			{
				if (Cast<ASP_HeroCharacter>(PlayerController->GetPawn()) != this)
				{
					PlayerController->AddOHHealthBar(this);
				}
			}
		}
	}
}

// Called every frame
void ASP_HeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASP_HeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) //overriden
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("SelfDamage"), IE_Pressed, this, &ASP_HeroCharacter::SelfDamage);
	PlayerInputComponent->BindAction(TEXT("ToggleDebugSpectatorMode"), IE_Pressed, this, &ASP_HeroCharacter::ActivateDebugSpectatorMode);
}

float ASP_HeroCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	
	float DamageToApply =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//Health is modified in BaseHeroCharacter::TakeDamage
	if (Health <= 0.0f)
	{
		Death();
	}
	
	return DamageToApply;
}

void ASP_HeroCharacter::Reload() //overriden
{
	if (!IsReloading)
	{
		bool bNeedReloading = false;
		for (int32 i = 0; i < AllAmmoAbilities.Num(); i++)
		{
			bNeedReloading = AllAmmoAbilities[i]->GetAmmoPercent() < 1.0f;
			if (bNeedReloading) break;
		}

		if (bNeedReloading)
		{
			ProxyReloadPlayMontage = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
				GetMesh(),
				ReloadMontage
			);
		
			ProxyReloadPlayMontage->OnInterrupted.AddDynamic(this, &ABaseHeroCharacter::OnReloadAnimationInterrupted);
			ProxyReloadPlayMontage->OnCompleted.AddDynamic(this, &ABaseHeroCharacter::OnReloadAnimationCompleted);
			IsReloading = true;
		}
		else
		{
			//TODO: clip sound
		}
	}
}

void ASP_HeroCharacter::ActivateDebugSpectatorMode()
{
	ASP_HeroPlayerController* ShooterPlayerController = Cast<ASP_HeroPlayerController>(GetController());
	if (ShooterPlayerController != nullptr)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		GEngine->Exec(GetWorld(), TEXT("r.MotionBlurQuality 0"));
		
		ASP_SpectatorPawn* ShooterSpectatorPawn = GetWorld()->SpawnActor<ASP_SpectatorPawn>(
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

void ASP_HeroCharacter::SelfDamage()
{
	Health -= 10.0f;
}