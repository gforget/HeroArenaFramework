// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stimuli/TouchStimuli.h"
#include "HurtStimuli.generated.h"

class ABaseShooterCharacter;
class ABaseShooterAIController;
/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UHurtStimuli : public UTouchStimuli
{
	GENERATED_BODY()

public :
	void Initialize(ABaseShooterAIController* OwnerControllerRef, ABaseShooterCharacter* OwnerCharacterRef);
	
	void SetDamageDone(float DamageDoneRef);
	float GetDamageDone();
	
	void SetDamageDealer(ABaseShooterCharacter* DamageDealerRef);
	ABaseShooterCharacter* GetDamageDealer();

private:
	UPROPERTY()
	ABaseShooterAIController* OwnerController;

	UPROPERTY()
	ABaseShooterCharacter* OwnerCharacter;
	
	UPROPERTY()
	float DamageDone;
	
	UPROPERTY()
	ABaseShooterCharacter* DamageDealer;

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};
