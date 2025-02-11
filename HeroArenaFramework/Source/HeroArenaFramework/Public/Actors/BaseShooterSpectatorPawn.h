// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/TeamManager.h"
#include "GameFramework/SpectatorPawn.h"
#include "BaseShooterSpectatorPawn.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API ABaseShooterSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseShooterSpectatorPawn();
	
	ETeam GetTeam();
	void SetTeam(ETeam TeamValue);
	
private:
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETeam> Team = ETeam::NoTeam;
};
