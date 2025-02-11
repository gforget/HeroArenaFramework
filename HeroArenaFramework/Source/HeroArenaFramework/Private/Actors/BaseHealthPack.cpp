// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseHealthPack.h"
#include "Actors/BaseShooterCharacter.h"

bool ABaseHealthPack::PackValidation(ABaseShooterCharacter* TargetShooterCharacter)
{
	return TargetShooterCharacter->GetHealthPercent() < 1.0f;
}

void ABaseHealthPack::GivePackTo(ABaseShooterCharacter* TargetShooterCharacter)
{
	Super::GivePackTo(TargetShooterCharacter);
	TargetShooterCharacter->Heal(HealAmount);
}

