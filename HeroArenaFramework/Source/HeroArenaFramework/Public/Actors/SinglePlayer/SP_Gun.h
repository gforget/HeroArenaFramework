// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseGun.h"
#include "GameFramework/Actor.h"
#include "SP_Gun.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API ASP_Gun : public ABaseGun
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Sets default values for this actor's properties
	ASP_Gun();
	virtual void Fire() override;

private:
	virtual bool GunTrace(FHitResult& Hit, FVector& ShotDirection) override;
};


