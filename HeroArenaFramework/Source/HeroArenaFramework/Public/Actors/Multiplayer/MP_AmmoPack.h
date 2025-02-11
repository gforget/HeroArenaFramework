// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseAmmoPack.h"
#include "MP_AmmoPack.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API AMP_AmmoPack : public ABaseAmmoPack
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
