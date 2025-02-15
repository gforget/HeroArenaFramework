// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stimuli/TouchStimuli.h"
#include "HurtStimuli.generated.h"

class ABaseHeroCharacter;
class ABaseHeroAIController;
/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UHurtStimuli : public UTouchStimuli
{
	GENERATED_BODY()

public :
	void Initialize(ABaseHeroAIController* OwnerControllerRef, ABaseHeroCharacter* OwnerCharacterRef);
	
	void SetDamageDone(float DamageDoneRef);
	float GetDamageDone();
	
	void SetDamageDealer(ABaseHeroCharacter* DamageDealerRef);
	ABaseHeroCharacter* GetDamageDealer();

private:
	UPROPERTY()
	ABaseHeroAIController* OwnerController;

	UPROPERTY()
	ABaseHeroCharacter* OwnerCharacter;
	
	UPROPERTY()
	float DamageDone;
	
	UPROPERTY()
	ABaseHeroCharacter* DamageDealer;

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};
