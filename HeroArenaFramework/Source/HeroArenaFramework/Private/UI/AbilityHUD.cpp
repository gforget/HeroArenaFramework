// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AbilityHUD.h"
#include "ActorComponents/BaseHeroAbility.h"
#include "Kismet/GameplayStatics.h"

void UAbilityHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Set up timer to wait for ability to be instantiated
	BindingStartTime = GetWorld()->GetTimeSeconds();
	
	// Create a timer that tries to bind every 0.1 seconds
	GetWorld()->GetTimerManager().SetTimer(
		BindingTimerHandle,
		this,
		&UAbilityHUD::TryBindToCooldownEvent,
		0.1f,
		true
	);
}

void UAbilityHUD::TryBindToCooldownEvent()
{
	if (BindToCooldownEvent())
	{
		GetWorld()->GetTimerManager().ClearTimer(BindingTimerHandle);
		return;
	}
	
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - BindingStartTime > 2.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityHUD: Failed to bind to ability cooldown event after 2 seconds. Profile Index: %d, Ability Type: %d"), 
			AbilityProfileIndex, static_cast<int32>(AbilityEnum));
		
		GetWorld()->GetTimerManager().ClearTimer(BindingTimerHandle);
	}
}

bool UAbilityHUD::BindToCooldownEvent()
{
	ABaseHeroCharacter* HeroCharacter = Cast<ABaseHeroCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!HeroCharacter)
	{
		return false;
	}
	
	UBaseHeroAbility* Ability = HeroCharacter->GetHeroAbility(AbilityProfileIndex, AbilityEnum);
	if (!Ability)
	{
		return false;
	}
	
	Ability->OnCooldownActivated.AddDynamic(this, &UAbilityHUD::HandleCooldownActivated);
	Ability->OnCooldownCleared.AddDynamic(this, &UAbilityHUD::HandleCooldownCleared);
	
	return true;
}

void UAbilityHUD::HandleCooldownActivated()
{
	// Call the blueprint implementable event
	OnAbilityCooldownActivated();
}

void UAbilityHUD::HandleCooldownCleared()
{
	// Call the blueprint implementable event
	OnAbilityCooldownCleared();
}


