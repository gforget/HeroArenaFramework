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
	UE_LOG(LogTemp, Warning, TEXT("Hero Ability Component '%s' initialized"), *GetName());
}


// Called every frame
void UBaseHeroAbility::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UE_LOG(LogTemp, Warning, TEXT("Skill activated"));
}

void UBaseHeroAbility::Execute_Implementation()
{
	// Base implementation - can be overridden in Blueprint or C++
	UE_LOG(LogTemp, Warning, TEXT("Base Ability Execute called on %s"), *GetName());
}

void UBaseHeroAbility::Cancel_Implementation()
{
	// Base implementation - can be overridden in Blueprint or C++
	UE_LOG(LogTemp, Warning, TEXT("Base Ability Cancel called on %s"), *GetName());
}
