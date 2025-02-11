// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseSpawningPoint.h"
#include "GameFramework/Actor.h"
#include "MP_SpawningPoint.generated.h"

class AMP_ShooterCharacter;

UCLASS()
class HEROARENAFRAMEWORK_API AMP_SpawningPoint : public ABaseSpawningPoint
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_SpawningPoint();
};
