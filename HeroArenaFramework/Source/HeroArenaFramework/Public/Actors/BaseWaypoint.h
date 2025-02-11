// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWaypoint.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API ABaseWaypoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWaypoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
