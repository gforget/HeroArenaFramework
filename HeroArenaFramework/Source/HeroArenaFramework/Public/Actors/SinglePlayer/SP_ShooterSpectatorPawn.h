// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseShooterSpectatorPawn.h"
#include "SP_ShooterSpectatorPawn.generated.h"

class ASP_ShooterCharacter;

UCLASS()
class HEROARENAFRAMEWORK_API ASP_ShooterSpectatorPawn : public ABaseShooterSpectatorPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASP_ShooterSpectatorPawn();

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
	
	void SetPlayerShooterCharacter(ASP_ShooterCharacter* PlayerShooterCharacterRef);
	
private:
	
	UPROPERTY()
	ASP_ShooterCharacter* PlayerShooterCharacter;
};
