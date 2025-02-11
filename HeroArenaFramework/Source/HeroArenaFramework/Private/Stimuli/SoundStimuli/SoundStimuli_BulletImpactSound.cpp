// Fill out your copyright notice in the Description page of Project Settings.


#include "Stimuli/SoundStimuli/SoundStimuli_BulletImpactSound.h"


// Sets default values
ASoundStimuli_BulletImpactSound::ASoundStimuli_BulletImpactSound()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASoundStimuli_BulletImpactSound::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoundStimuli_BulletImpactSound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoundStimuli_BulletImpactSound::SetShootingOriginPosition(FVector ShootingOriginPositionValue)
{
	ShootingOriginPosition = ShootingOriginPositionValue;
}

FVector ASoundStimuli_BulletImpactSound::GetShootingOriginPosition()
{
	return ShootingOriginPosition;
}

