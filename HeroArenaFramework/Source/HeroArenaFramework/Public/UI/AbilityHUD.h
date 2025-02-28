// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseHeroCharacter.h"
#include "Blueprint/UserWidget.h"
#include "AbilityHUD.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UAbilityHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	UTexture2D* AbilityIconBrush;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	int32 AbilityProfileIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	EAbilityEnum AbilityEnum;
	
	// Blueprint event that will be called when cooldown is activated
	UFUNCTION(BlueprintImplementableEvent, Category="Cooldown")
	void OnAbilityCooldownActivated();
	
	// New Blueprint event for when cooldown is cleared
	UFUNCTION(BlueprintImplementableEvent, Category="Cooldown")
	void OnAbilityCooldownCleared();
	
	// Bind to the ability's cooldown events
	UFUNCTION(BlueprintCallable, Category="Cooldown")
	bool BindToCooldownEvent();
	
	// Callback function for when cooldown is activated
	UFUNCTION()
	void HandleCooldownActivated();
	
	// New callback function for when cooldown is cleared
	UFUNCTION()
	void HandleCooldownCleared();

private:
	// Timer handle for ability binding attempts
	FTimerHandle BindingTimerHandle;
	
	// Time when binding attempts started
	float BindingStartTime;
	
	// Function to try binding to cooldown event repeatedly
	void TryBindToCooldownEvent();
};
