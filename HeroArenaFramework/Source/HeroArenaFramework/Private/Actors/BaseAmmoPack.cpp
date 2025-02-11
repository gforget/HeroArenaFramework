// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseAmmoPack.h"
#include "Actors/BaseShooterCharacter.h"

bool ABaseAmmoPack::PackValidation(ABaseShooterCharacter* TargetShooterCharacter)
{
	return TargetShooterCharacter->GetAmmoReservePercent() < 1.0f;
}

void ABaseAmmoPack::GivePackTo(ABaseShooterCharacter* TargetShooterCharacter)
{
	Super::GivePackTo(TargetShooterCharacter);
	TargetShooterCharacter->AddAmmoReserve(AmmoAmount);
}
