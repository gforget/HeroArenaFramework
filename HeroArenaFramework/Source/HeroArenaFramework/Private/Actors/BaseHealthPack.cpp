// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseHealthPack.h"
#include "Actors/BaseHeroCharacter.h"

bool ABaseHealthPack::PackValidation(ABaseHeroCharacter* TargetShooterCharacter)
{
	return TargetShooterCharacter->GetHealthPercent() < 1.0f;
}

void ABaseHealthPack::GivePackTo(ABaseHeroCharacter* TargetShooterCharacter)
{
	Super::GivePackTo(TargetShooterCharacter);
	TargetShooterCharacter->Heal(HealAmount);
}

