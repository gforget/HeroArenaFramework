// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/BaseHeroAbility.h"

#include "Actors/BaseHeroCharacter.h"


// Sets default values for this component's properties
UBaseHeroAbility::UBaseHeroAbility()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBaseHeroAbility::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);	
}

void UBaseHeroAbility::ActivateCooldown()
{
	if (Cooldown > 0.0f)
	{
		bIsOnCooldown = true;
		RemainingCooldown = Cooldown;
		
		if (GetWorld()->GetTimerManager().IsTimerActive(CooldownTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
		}
		
		GetWorld()->GetTimerManager().SetTimer(
			CooldownTimerHandle,
			this,
			&UBaseHeroAbility::ProcessCooldown,
			CooldownTimerRate,
			true
		);
		
		// Broadcast the cooldown activated event
		OnCooldownActivated.Broadcast();
	}
}

void UBaseHeroAbility::ProcessCooldown()
{
	if (bIsOnCooldown)
	{
		RemainingCooldown -= CooldownTimerRate * CooldownScale;
		
		if (RemainingCooldown <= 0.0f)
		{
			ClearCoolDown();
		}
	}
}

void UBaseHeroAbility::ClearCoolDown()
{
	bIsOnCooldown = false;
	RemainingCooldown = 0.0f;
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	
	// Broadcast the cooldown cleared event
	OnCooldownCleared.Broadcast();
}

void UBaseHeroAbility::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBaseHeroAbility::StartAbility_Implementation()
{
	
}

void UBaseHeroAbility::CallStartAbility()
{
	if (!IsAbilityLocked()) StartAbility();
}

void UBaseHeroAbility::EndAbility_Implementation()
{
	
}

void UBaseHeroAbility::CallEndAbility()
{
	if (!IsAbilityLocked()) EndAbility();
}

void UBaseHeroAbility::CancelAbility_Implementation()
{
	
}

void UBaseHeroAbility::CallCancelAbility()
{
	CancelAbility();
}

ABaseHeroCharacter* UBaseHeroAbility::GetOwningHeroCharacter() const
{
	return Cast<ABaseHeroCharacter>(GetOwner());
}

bool UBaseHeroAbility::IsAbilityLocked() const
{
	//TODO: add other condition to see if the ability is locked
	return bIsOnCooldown;
}

FString UBaseHeroAbility::GetAmmoRatio() const
{
	ABaseHeroCharacter* OwningHero = GetOwningHeroCharacter();
	if (AmmoPoolIndex >= 0 && OwningHero && OwningHero->AmmoPools.IsValidIndex(AmmoPoolIndex))
	{
		return FString::FromInt(OwningHero->AmmoPools[AmmoPoolIndex].Ammo) + "/" + 
			   FString::FromInt(OwningHero->AmmoPools[AmmoPoolIndex].MaxAmmo);
	}
	
	return "";
}

float UBaseHeroAbility::GetAmmoPercent() const
{
	ABaseHeroCharacter* OwningHero = GetOwningHeroCharacter();
	if (AmmoPoolIndex >= 0 && OwningHero && OwningHero->AmmoPools.IsValidIndex(AmmoPoolIndex))
	{
		const FAmmoPool& AmmoPool = OwningHero->AmmoPools[AmmoPoolIndex];
		return AmmoPool.MaxAmmo > 0 ? static_cast<float>(AmmoPool.Ammo)/static_cast<float>(AmmoPool.MaxAmmo) : 0.0f;
	}
	
	return 0.0f;
}

int UBaseHeroAbility::GetAmmoAmount() const
{
	ABaseHeroCharacter* OwningHero = GetOwningHeroCharacter();
	if (AmmoPoolIndex >= 0 && OwningHero && OwningHero->AmmoPools.IsValidIndex(AmmoPoolIndex))
	{
		return OwningHero->AmmoPools[AmmoPoolIndex].Ammo;
	}
	
	return 0;
}

int UBaseHeroAbility::GetMaxAmmo() const
{
	ABaseHeroCharacter* OwningHero = GetOwningHeroCharacter();
	if (AmmoPoolIndex >= 0 && OwningHero && OwningHero->AmmoPools.IsValidIndex(AmmoPoolIndex))
	{
		return OwningHero->AmmoPools[AmmoPoolIndex].MaxAmmo;
	}
	
	return 0;
}

bool UBaseHeroAbility::UseAmmo()
{
	ABaseHeroCharacter* OwningHero = GetOwningHeroCharacter();
	if (AmmoPoolIndex >= 0 && OwningHero && OwningHero->AmmoPools.IsValidIndex(AmmoPoolIndex))
	{
		FAmmoPool& AmmoPool = OwningHero->AmmoPools[AmmoPoolIndex];
		if (AmmoPool.Ammo > 0)
		{
			AmmoPool.Ammo--;
			return true;
		}
		else
		{
			AmmoPool.Ammo = 0;
			return false;
		}
	}
	
	return false;
}

void UBaseHeroAbility::ReloadAmmo()
{
	ABaseHeroCharacter* OwningHero = GetOwningHeroCharacter();
	if (AmmoPoolIndex >= 0 && OwningHero && OwningHero->AmmoPools.IsValidIndex(AmmoPoolIndex))
	{
		FAmmoPool& AmmoPool = OwningHero->AmmoPools[AmmoPoolIndex];
		AmmoPool.Ammo = AmmoPool.MaxAmmo;
	}
}

bool UBaseHeroAbility::IsOnCooldown() const
{
	return bIsOnCooldown;
}

float UBaseHeroAbility::GetRemainingCooldown() const
{
	return RemainingCooldown;
}

float UBaseHeroAbility::GetCooldownPercent() const
{
	if (Cooldown <= 0.0f)
	{
		return 0.0f;
	}
	
	return FMath::Clamp(RemainingCooldown / Cooldown, 0.0f, 1.0f);
}

float UBaseHeroAbility::GetCooldownScale() const
{
	return CooldownScale;
}

void UBaseHeroAbility::SetCooldownScale(float NewScale)
{
	CooldownScale = NewScale;
}
