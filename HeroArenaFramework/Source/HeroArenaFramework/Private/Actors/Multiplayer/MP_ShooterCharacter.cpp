// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Multiplayer/MP_ShooterCharacter.h"

#include "PlayMontageCallbackProxy.h"
#include "Actors/Multiplayer/MP_Gun.h"
#include "Components/SphereComponent.h"
#include "Controllers/Multiplayer/MP_ShooterPlayerController.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMP_ShooterCharacter::AMP_ShooterCharacter()
{
	bReplicates = true;
}

// Called when the game starts or when spawned
void AMP_ShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	//TODO: add overhead healtbar for bot, they are locally controlled by the server so that condition is flawed
	if (!IsLocallyControlled())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		Cast<AMP_ShooterPlayerController>(PlayerController)->AddOHHealthBar(this);
	}
	
	if (AMP_ShooterGameState* GameState = Cast<AMP_ShooterGameState>(GetWorld()->GetGameState()))
	{
		GameState->AddShooterCharacterCount(this);
		GameState->RegisterEvent(this);
	}
	
	// Set initial collision sphere size
	HeadCollision->SetSphereRadius(HeadshotRadius-5.0f);
	UpdateHeadCollision();
}

// Called every frame
void AMP_ShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		if (Controller)
		{
			FVector NewLocation;
			FRotator NewRotation;
			Controller->GetPlayerViewPoint(NewLocation, NewRotation);

			if (!ShooterViewPointLocation.Equals(NewLocation, 0.01f) || !ShooterViewPointRotation.Equals(NewRotation, 0.1f))
			{
				ShooterViewPointLocation = NewLocation;
				ShooterViewPointRotation = NewRotation;
			}
		}
	}
}

// Called to bind functionality to input
void AMP_ShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMP_ShooterCharacter::LookUp);
}

//---- Look UP RPC ----/
void AMP_ShooterCharacter::LookUp(float AxisValue)
{
	if (Controller && AxisValue != 0.0f)
	{
		// Locally modify the rotation for immediate response
		FRotator NewRotation = Controller->GetControlRotation();
		NewRotation.Pitch -= AxisValue;
		
		UpdateControlRotation(NewRotation);

		// Send the rotation to the server
		if (!HasAuthority())
		{
			ServerSetControlRotation(NewRotation);
		}
	}
}

void AMP_ShooterCharacter::ServerSetControlRotation_Implementation(const FRotator& NewRotation)
{
	UpdateControlRotation(NewRotation); // Update the server's rotation
}

bool AMP_ShooterCharacter::ServerSetControlRotation_Validate(const FRotator& NewRotation)
{
	return true;
}

void AMP_ShooterCharacter::UpdateControlRotation(const FRotator& NewRotation)
{
	ReplicatedControlRotation = NewRotation;

	if (Controller)
	{
		Controller->SetControlRotation(NewRotation);
	}
}

void AMP_ShooterCharacter::OnRep_ControlRotation()
{
	if (!IsLocallyControlled())
	{
		UpdateControlRotation(ReplicatedControlRotation);
	}
}

//---- [END] Look UP RPC ----/

float AMP_ShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	
	//Health is modified in BaseShooterCharacter::TakeDamage
	float DamageToApply =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Health <= 0.0f)
	{
		MulticastDeath();
	}
	
	return DamageToApply;
}

void AMP_ShooterCharacter::MulticastDeath_Implementation()
{
	Death();
}

//---- Pull Trigger ----/
void AMP_ShooterCharacter::PullTrigger()
{
	if (HasAuthority())
	{
		MulticastPullTrigger();
	}
	else
	{
		ServerPullTrigger();
	}
}

bool AMP_ShooterCharacter::ServerPullTrigger_Validate()
{
	// Optional: Add validation logic here (e.g., preventing cheating)
	return true;
}

void AMP_ShooterCharacter::ServerPullTrigger_Implementation()
{
	MulticastPullTrigger();
}

void AMP_ShooterCharacter::MulticastPullTrigger_Implementation()
{
	// Perform the shooting logic on all clients
	PerformPullTrigger();
}

void AMP_ShooterCharacter::PerformPullTrigger()
{
	if (Gun != nullptr)
	{
		Gun->PullTrigger();
	}
}

//---- Release Trigger ----/
void AMP_ShooterCharacter::ReleaseTrigger()
{
	if (HasAuthority())
	{
		MulticastReleaseTrigger();
	}
	else
	{
		ServerReleaseTrigger();
	}
}

bool AMP_ShooterCharacter::ServerReleaseTrigger_Validate()
{
	// Optional: Add validation logic here (e.g., preventing cheating)
	return true;
}

void AMP_ShooterCharacter::ServerReleaseTrigger_Implementation()
{
	MulticastReleaseTrigger();
}

void AMP_ShooterCharacter::MulticastReleaseTrigger_Implementation()
{
	// Perform the shooting logic on all clients
	PerformReleaseTrigger();
}

void AMP_ShooterCharacter::PerformReleaseTrigger()
{
	if (Gun != nullptr)
	{
		Gun->ReleaseTrigger();	
	}
}

//---- Reload ----/
void AMP_ShooterCharacter::Reload()
{
	if (HasAuthority())
	{
		MulticastReload();
	}
	else
	{
		ServerReload();
	}
}

void AMP_ShooterCharacter::ServerReload_Implementation()
{
	MulticastReload();
}

bool AMP_ShooterCharacter::ServerReload_Validate()
{
	return true;
}

void AMP_ShooterCharacter::MulticastReload_Implementation()
{
	PerformReload();
}

void AMP_ShooterCharacter::PerformReload()
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

void AMP_ShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Ensure All Replicated variable to be replicated
	DOREPLIFETIME(AMP_ShooterCharacter, ReplicatedControlRotation);
	DOREPLIFETIME(AMP_ShooterCharacter, ShooterViewPointLocation);
	DOREPLIFETIME(AMP_ShooterCharacter, ShooterViewPointRotation);
	DOREPLIFETIME(AMP_ShooterCharacter, Health);
}
