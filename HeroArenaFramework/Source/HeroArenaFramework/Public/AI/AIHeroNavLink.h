// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "AIHeroNavLink.generated.h"

UCLASS()
class HEROARENAFRAMEWORK_API AAIHeroNavLink : public ANavLinkProxy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAIHeroNavLink();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
	void OnNavLinkReach(AActor* MovingActor, const FVector& DestinationPoint);
};
