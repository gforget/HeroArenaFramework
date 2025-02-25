// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/BaseHeroAbility.h"


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
	if (MaxAmmo > 0)
	{
		Ammo = MaxAmmo;
	}
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

FString UBaseHeroAbility::GetAmmoRatio() const
{
	return FString::FromInt(Ammo) + "/" + FString::FromInt(MaxAmmo); 
}

float UBaseHeroAbility::GetAmmoPercent() const
{
	return static_cast<float>(Ammo)/static_cast<float>(MaxAmmo);
}

int UBaseHeroAbility::GetAmmoAmount() const
{
	return Ammo;
}

int UBaseHeroAbility::GetMaxAmmo() const
{
	return MaxAmmo;
}

bool UBaseHeroAbility::UseAmmo()
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

void UBaseHeroAbility::ReloadAmmo()
{
	Ammo = MaxAmmo;
}
