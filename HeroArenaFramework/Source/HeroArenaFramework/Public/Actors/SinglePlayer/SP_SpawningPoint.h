// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseSpawningPoint.h"
#include "GameFramework/Actor.h"
#include "SP_SpawningPoint.generated.h"

class ASP_HeroCharacter;

UCLASS()
class HEROARENAFRAMEWORK_API ASP_SpawningPoint : public ABaseSpawningPoint
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASP_SpawningPoint();
};
