// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseGun.h"
#include "GameFramework/Actor.h"
#include "MP_Gun.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API AMP_Gun : public ABaseGun
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	AMP_Gun();
	virtual void Fire() override;
private:
	virtual bool GunTrace(FHitResult& Hit, FVector& ShotDirection) override;
	
};


