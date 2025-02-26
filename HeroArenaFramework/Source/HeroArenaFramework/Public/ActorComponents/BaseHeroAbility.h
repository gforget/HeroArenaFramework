// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseHeroAbility.generated.h"

class ABaseHeroCharacter;

UCLASS(ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class HEROARENAFRAMEWORK_API UBaseHeroAbility : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseHeroAbility();
	
	UFUNCTION(BlueprintNativeEvent)
	void StartAbility();
	
	virtual void StartAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void EndAbility();
	
	virtual void EndAbility_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void Cancel();
	
	virtual void Cancel_Implementation();

	UFUNCTION(BlueprintCallable, Category="Ammo")
	FString GetAmmoRatio() const;

	UFUNCTION(BlueprintCallable, Category="Ammo")
	float GetAmmoPercent() const;

	UFUNCTION(BlueprintCallable, Category="Ammo")
	int GetAmmoAmount() const;

	UFUNCTION(BlueprintCallable, Category="Ammo")
	int GetMaxAmmo() const;

	UFUNCTION(BlueprintCallable, Category="Ammo")
	bool UseAmmo();

	UFUNCTION(BlueprintCallable, Category="Ammo")
	void ReloadAmmo();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	int AmmoPoolIndex = -1;
	
	ABaseHeroCharacter* GetOwningHeroCharacter() const;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
