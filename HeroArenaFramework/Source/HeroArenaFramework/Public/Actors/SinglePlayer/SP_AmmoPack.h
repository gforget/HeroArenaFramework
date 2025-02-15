// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseAmmoPack.h"
#include "GameFramework/Actor.h"
#include "SP_AmmoPack.generated.h"

class ASP_HeroCharacter;
UCLASS()
class HEROARENAFRAMEWORK_API ASP_AmmoPack : public ABaseAmmoPack
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
