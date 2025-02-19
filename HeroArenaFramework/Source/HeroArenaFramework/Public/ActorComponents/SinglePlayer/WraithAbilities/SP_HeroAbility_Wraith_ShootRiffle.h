// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/SinglePlayer/SP_HeroAbility.h"
#include "SP_HeroAbility_Wraith_ShootRiffle.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEROARENAFRAMEWORK_API USP_HeroAbility_Wraith_ShootRiffle : public USP_HeroAbility
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USP_HeroAbility_Wraith_ShootRiffle();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Execute_Implementation() override;
};
