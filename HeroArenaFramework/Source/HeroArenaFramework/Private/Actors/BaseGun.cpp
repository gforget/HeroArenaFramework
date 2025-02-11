// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/BaseGun.h"
#include "Actors/BaseShooterCharacter.h"

// Sets default values
ABaseGun::ABaseGun()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	// Bind the spread update delegate once
	FireTimerTimerDel.BindUFunction(this, FName("Fire"));
	SpreadUpdateTimerDel.BindUFunction(this, FName("UpdateSpread"));
}

// Called when the game starts or when spawned
void ABaseGun::BeginPlay()
{
	Super::BeginPlay();
	Ammo = MaxAmmo;
	CurrentBulletSpreadRadius = BaseBulletSpreadRadius;
}

void ABaseGun::Fire()
{
	if (ABaseShooterCharacter* ShooterCharacter = Cast<ABaseShooterCharacter>(GetOwner())) 
	{
		bool bIsReloading = ShooterCharacter->GetIsReloading();
		// Check if spread update timer is not active and restart it
		if (!GetWorld()->GetTimerManager().IsTimerActive(SpreadUpdateHandle) && !bIsReloading && Ammo > 0)
		{
			
			GetWorld()->GetTimerManager().SetTimer(
				SpreadUpdateHandle,
				SpreadUpdateTimerDel,
				0.016f, // ~60fps
				true
			);
		}
	}
}

void ABaseGun::PullTrigger()
{
	if (!TriggerPulled)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				FireTimerHandle,
				FireTimerTimerDel,
				TimeBetweenRound,
				true,
				0.0f
			);
		}

		if (!GetWorld()->GetTimerManager().IsTimerActive(SpreadUpdateHandle))
		{
			// Start updating spread
			GetWorld()->GetTimerManager().SetTimer(
				SpreadUpdateHandle,
				SpreadUpdateTimerDel,
				0.016f, // ~60fps
				true
			);
		}
		
		TriggerPulled = true;
	}
}

void ABaseGun::ReleaseTrigger()
{
	if (TriggerPulled)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
		TriggerPulled = false;
	}
}

void ABaseGun::UpdateSpread()
{
	if (ABaseShooterCharacter* ShooterCharacter = Cast<ABaseShooterCharacter>(GetOwner())) 
	{
		bool bIsReloading = ShooterCharacter->GetIsReloading();
	
		if (TriggerPulled && Ammo > 0 && !bIsReloading)  // Only increase spread if we have ammo and not reloading
		{
			CurrentBulletSpreadRadius = FMath::Min(
				CurrentBulletSpreadRadius + (SpreadIncreaseRate * 0.016f),
				MaxBulletSpreadRadius
			);
		}
		else
		{
			CurrentBulletSpreadRadius = FMath::Max(
				CurrentBulletSpreadRadius - (SpreadRecoveryRate * 0.016f),
				BaseBulletSpreadRadius
			);
			
			// Stop updating if we've recovered
			if (CurrentBulletSpreadRadius <= BaseBulletSpreadRadius)
			{
				GetWorld()->GetTimerManager().ClearTimer(SpreadUpdateHandle);
			}
		}

		if (bDebugBulletSpread)
		{
			// Log the current bullet spread radius
			UE_LOG(LogTemp, Warning, TEXT("Current Bullet Spread Radius: %f"), CurrentBulletSpreadRadius);
		}
	}
}

int ABaseGun::Reload(int AmmoAmount)
{
	int LeftOver = 0;
	
	if (Ammo + AmmoAmount > MaxAmmo)
	{
		LeftOver = (Ammo + AmmoAmount) - MaxAmmo;
		Ammo += AmmoAmount-LeftOver;
	}
	else
	{
		Ammo += AmmoAmount;
	}

	return LeftOver;
}

FString ABaseGun::GetAmmoRatio() const
{
	return FString::FromInt(Ammo) + "/" + FString::FromInt(MaxAmmo); 
}

float ABaseGun::GetAmmoPercent() const
{
	return static_cast<float>(Ammo)/static_cast<float>(MaxAmmo);
}

int ABaseGun::GetAmmoAmount() const
{
	return Ammo;
}

int ABaseGun::GetMaxAmmo() const
{
	return MaxAmmo;
}

bool ABaseGun::UseAmmo()
{
	if (Ammo-1 > 0)
	{
		Ammo--;
		return true;
	}
	else
	{
		Ammo = 0;
		return false;
	}
}

AController* ABaseGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	
	return OwnerPawn->GetController();
}

