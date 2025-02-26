// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/BaseHeroAbility.h"

#include "Actors/BaseHeroCharacter.h"


// Sets default values for this component's properties
UBaseHeroAbility::UBaseHeroAbility()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UBaseHeroAbility::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}


// Called every frame
void UBaseHeroAbility::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBaseHeroAbility::StartAbility_Implementation()
{
	
}

void UBaseHeroAbility::EndAbility_Implementation()
{
	
}

void UBaseHeroAbility::Cancel_Implementation()
{
	
}

// Add a helper method to get the owning hero character
ABaseHeroCharacter* UBaseHeroAbility::GetOwningHeroCharacter() const
{
	return Cast<ABaseHeroCharacter>(GetOwner());
}

FString UBaseHeroAbility::GetAmmoRatio() const
{
	ABaseHeroCharacter* OwningHero = GetOwningHeroCharacter();
	if (AmmoPoolIndex >= 0 && OwningHero && OwningHero->AmmoPools.IsValidIndex(AmmoPoolIndex))
	{
		return FString::FromInt(OwningHero->AmmoPools[AmmoPoolIndex].Ammo) + "/" + 
			   FString::FromInt(OwningHero->AmmoPools[AmmoPoolIndex].MaxAmmo);
	}
	
	return "0/0";
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
