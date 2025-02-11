// Fill out your copyright notice in the Description page of Project Settings.


#include "Stimuli/SoundStimuli/SoundStimuli_ShootingSound.h"


// Sets default values
ASoundStimuli_ShootingSound::ASoundStimuli_ShootingSound()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASoundStimuli_ShootingSound::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoundStimuli_ShootingSound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

