// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Multiplayer/MP_HeroCharacter.h"

#include "PlayMontageCallbackProxy.h"
#include "Components/SphereComponent.h"
#include "Controllers/Multiplayer/MP_HeroPlayerController.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_BaseGameState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMP_HeroCharacter::AMP_HeroCharacter()
{
	bReplicates = true;
}

// Called when the game starts or when spawned
void AMP_HeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	//TODO: add overhead healtbar for bot, they are locally controlled by the server so that condition is flawed
	if (!IsLocallyControlled())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		Cast<AMP_HeroPlayerController>(PlayerController)->AddOHHealthBar(this);
	}
	
	if (AMP_BaseGameState* GameState = Cast<AMP_BaseGameState>(GetWorld()->GetGameState()))
	{
		GameState->AddShooterCharacterCount(this);
		GameState->RegisterEvent(this);
	}
	
	// Set initial collision sphere size
	HeadCollision->SetSphereRadius(HeadshotRadius-5.0f);
	UpdateHeadCollision();
}

// Called every frame
void AMP_HeroCharacter::Tick(float DeltaTime)
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
void AMP_HeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMP_HeroCharacter::LookUp);
}

//---- Look UP RPC ----/
void AMP_HeroCharacter::LookUp(float AxisValue)
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

void AMP_HeroCharacter::ServerSetControlRotation_Implementation(const FRotator& NewRotation)
{
	UpdateControlRotation(NewRotation); // Update the server's rotation
}

bool AMP_HeroCharacter::ServerSetControlRotation_Validate(const FRotator& NewRotation)
{
	return true;
}

void AMP_HeroCharacter::UpdateControlRotation(const FRotator& NewRotation)
{
	ReplicatedControlRotation = NewRotation;

	if (Controller)
	{
		Controller->SetControlRotation(NewRotation);
	}
}

void AMP_HeroCharacter::OnRep_ControlRotation()
{
	if (!IsLocallyControlled())
	{
		UpdateControlRotation(ReplicatedControlRotation);
	}
}

//---- [END] Look UP RPC ----/

float AMP_HeroCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
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

void AMP_HeroCharacter::MulticastDeath_Implementation()
{
	Death();
}

//---- Reload ----/
void AMP_HeroCharacter::Reload()
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

void AMP_HeroCharacter::ServerReload_Implementation()
{
	MulticastReload();
}

bool AMP_HeroCharacter::ServerReload_Validate()
{
	return true;
}

void AMP_HeroCharacter::MulticastReload_Implementation()
{
	PerformReload();
}

void AMP_HeroCharacter::PerformReload()
{
	if (!IsReloading && GetAmmoMagazinePercent() < 1.0f )
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

void AMP_HeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Ensure All Replicated variable to be replicated
	DOREPLIFETIME(AMP_HeroCharacter, ReplicatedControlRotation);
	DOREPLIFETIME(AMP_HeroCharacter, ShooterViewPointLocation);
	DOREPLIFETIME(AMP_HeroCharacter, ShooterViewPointRotation);
	DOREPLIFETIME(AMP_HeroCharacter, Health);
}
