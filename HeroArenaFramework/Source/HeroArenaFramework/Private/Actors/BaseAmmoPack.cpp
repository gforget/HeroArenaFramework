// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseAmmoPack.h"
#include "Actors/BaseHeroCharacter.h"

bool ABaseAmmoPack::PackValidation(ABaseHeroCharacter* TargetShooterCharacter)
{
	return TargetShooterCharacter->GetAmmoReservePercent() < 1.0f;
}

void ABaseAmmoPack::GivePackTo(ABaseHeroCharacter* TargetShooterCharacter)
{
	Super::GivePackTo(TargetShooterCharacter);
	TargetShooterCharacter->AddAmmoReserve(AmmoAmount);
}
