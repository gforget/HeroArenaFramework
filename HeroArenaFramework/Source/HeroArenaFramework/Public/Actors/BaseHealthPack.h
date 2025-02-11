// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItemPack.h"
#include "BaseHealthPack.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API ABaseHealthPack : public ABaseItemPack
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float HealAmount = 10.0f;

private:
	
	virtual bool PackValidation(ABaseShooterCharacter* TargetShooterCharacter) override;
	virtual void GivePackTo(ABaseShooterCharacter* TargetShooterCharacter) override;
};
