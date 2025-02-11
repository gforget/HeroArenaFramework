// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseShooterSpectatorPawn.h"
#include "MP_ShooterSpectatorPawn.generated.h"

class AMP_ShooterCharacter;

UCLASS()
class HEROARENAFRAMEWORK_API AMP_ShooterSpectatorPawn : public ABaseShooterSpectatorPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMP_ShooterSpectatorPawn();
};
