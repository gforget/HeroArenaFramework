// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseHeroAbility.generated.h"

UCLASS(ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class HEROARENAFRAMEWORK_API UBaseHeroAbility : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseHeroAbility();
	
	UFUNCTION(BlueprintNativeEvent)
	void Execute();
	
	virtual void Execute_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Cancel();
	
	virtual void Cancel_Implementation();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
