// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/BaseHeroAbility.h"


// Sets default values for this component's properties
UBaseHeroAbility::UBaseHeroAbility()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBaseHeroAbility::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}


// Called every frame
void UBaseHeroAbility::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBaseHeroAbility::StartAbility_Implementation()
{
	
}

void UBaseHeroAbility::EndAbility_Implementation()
{
	
}

void UBaseHeroAbility::Cancel_Implementation()
{
	
}
