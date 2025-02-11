// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseHealthPack.h"
#include "MP_HealthPack.generated.h"
class AMP_ShooterCharacter;
UCLASS()
class HEROARENAFRAMEWORK_API AMP_HealthPack : public ABaseHealthPack
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
