// Fill out your copyright notice in the Description page of Project Settings.


#include "Stimuli/Stimuli.h"


// Sets default values
AStimuli::AStimuli()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStimuli::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStimuli::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

