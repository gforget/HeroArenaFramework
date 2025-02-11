// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseWaypoint.h"
#include "GameFramework/Actor.h"
#include "SP_Waypoint.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API ASP_Waypoint : public ABaseWaypoint
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASP_Waypoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
