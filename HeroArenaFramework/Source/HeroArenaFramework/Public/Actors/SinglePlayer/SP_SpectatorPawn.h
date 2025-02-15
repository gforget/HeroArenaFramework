// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseSpectatorPawn.h"
#include "SP_SpectatorPawn.generated.h"

class ASP_HeroCharacter;

UCLASS()
class HEROARENAFRAMEWORK_API ASP_SpectatorPawn : public ABaseSpectatorPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASP_SpectatorPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PauseGame();
	void ReturnToPlayerMode();
	void SkipFrame();
	void StartFrameTimer();
	
	void SetPlayerShooterCharacter(ASP_HeroCharacter* PlayerShooterCharacterRef);
	
private:
	
	UPROPERTY()
	ASP_HeroCharacter* PlayerShooterCharacter;
};
