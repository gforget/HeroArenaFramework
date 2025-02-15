// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseSpectatorPawn.h"
#include "MP_SpectatorPawn.generated.h"

class AMP_HeroCharacter;

UCLASS()
class HEROARENAFRAMEWORK_API AMP_SpectatorPawn : public ABaseSpectatorPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMP_SpectatorPawn();
};
