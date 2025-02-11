// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SinglePlayer/SP_Waypoint.h"
#include "GameMode/SinglePlayer/SP_ShooterGameMode.h"

// Sets default values
ASP_Waypoint::ASP_Waypoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASP_Waypoint::BeginPlay()
{
	Super::BeginPlay();
	ASP_ShooterGameMode* GameModeBase = GetWorld()->GetAuthGameMode<ASP_ShooterGameMode>();
	if (GameModeBase != nullptr)
	{
		GameModeBase->AddWayPoint(this);
	}
}

