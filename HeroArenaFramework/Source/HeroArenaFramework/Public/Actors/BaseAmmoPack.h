// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItemPack.h"
#include "BaseAmmoPack.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API ABaseAmmoPack : public ABaseItemPack
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	int AmmoAmount = 20;

private:
	virtual bool PackValidation(ABaseHeroCharacter* TargetShooterCharacter) override;
	virtual void GivePackTo(ABaseHeroCharacter* TargetShooterCharacter) override;
};
