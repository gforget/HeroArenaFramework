// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stimuli/SoundStimuli.h"
#include "SoundStimuli_BulletImpactSound.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API ASoundStimuli_BulletImpactSound : public ASoundStimuli
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundStimuli_BulletImpactSound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetShootingOriginPosition(FVector ShootingOriginPositionValue);

	FVector GetShootingOriginPosition();

private:
	
	UPROPERTY()
	FVector ShootingOriginPosition;
};
