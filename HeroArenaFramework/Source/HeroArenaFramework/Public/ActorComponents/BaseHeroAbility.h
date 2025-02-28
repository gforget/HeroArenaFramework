// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseHeroAbility.generated.h"

class ABaseHeroCharacter;

// Add this before the class declaration
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooldownActivatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooldownClearedDelegate);

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
	void CallStartAbility();

	UFUNCTION(BlueprintNativeEvent)
	void EndAbility();
	virtual void EndAbility_Implementation();
	void CallEndAbility();
	
	UFUNCTION(BlueprintNativeEvent)
	void CancelAbility();
	virtual void CancelAbility_Implementation();
	void CallCancelAbility();
	
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

	UFUNCTION(BlueprintCallable, Category="Cooldown")
	bool IsOnCooldown() const;
	
	UFUNCTION(BlueprintCallable, Category="Cooldown")
	float GetRemainingCooldown() const;
	
	UFUNCTION(BlueprintCallable, Category="Cooldown")
	float GetCooldownPercent() const;
	
	UFUNCTION(BlueprintCallable, Category="Cooldown")
	float GetCooldownScale() const;
	
	UFUNCTION(BlueprintCallable, Category="Cooldown")
	void SetCooldownScale(float NewScale);

	UFUNCTION(BlueprintCallable, Category="Cooldown")
	void ClearCoolDown();
	
	// Add this new event
	UPROPERTY(BlueprintAssignable, Category="Cooldown")
	FOnCooldownActivatedDelegate OnCooldownActivated;
	
	// Add this new event for cooldown cleared
	UPROPERTY(BlueprintAssignable, Category="Cooldown")
	FOnCooldownClearedDelegate OnCooldownCleared;
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Cooldown")
	void ActivateCooldown();
	
	UPROPERTY(EditDefaultsOnly, Category="General Properties")
	int AmmoPoolIndex = -1;
	
	UPROPERTY(EditDefaultsOnly, Category="General Properties")
	float Cooldown = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="General Properties")
	float CooldownScale = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category="General Properties")
	int Charges = 0;

	UPROPERTY()
	bool bIsOnCooldown = false;

	UPROPERTY()
	float RemainingCooldown = 0.0f;

	UPROPERTY()
	FTimerHandle CooldownTimerHandle;
	void ProcessCooldown();

	UPROPERTY()
	float CooldownTimerRate = 0.1f;
	
	ABaseHeroCharacter* GetOwningHeroCharacter() const;

	bool IsAbilityLocked() const;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
