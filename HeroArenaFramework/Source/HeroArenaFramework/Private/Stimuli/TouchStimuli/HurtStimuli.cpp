// Fill out your copyright notice in the Description page of Project Settings.


#include "Stimuli/TouchStimuli/HurtStimuli.h"

#include "Actors/SinglePlayer/SP_HeroCharacter.h"
#include "Controllers/SinglePlayer/SP_HeroAIController.h"

void UHurtStimuli::Initialize(ABaseHeroAIController* OwnerControllerRef, ABaseHeroCharacter* OwnerCharacterRef)
{
	OwnerController = OwnerControllerRef;
	
	OwnerCharacter = OwnerCharacterRef;
	OwnerCharacter->OnTakeAnyDamage.AddDynamic(this, &UHurtStimuli::OnTakeDamage);
}

void UHurtStimuli::SetDamageDone(float DamageDoneRef)
{
	DamageDone = DamageDoneRef;
}

float UHurtStimuli::GetDamageDone()
{
	return DamageDone;
}

void UHurtStimuli::SetDamageDealer(ABaseHeroCharacter* DamageDealerRef)
{
	DamageDealer = DamageDealerRef;
}

ABaseHeroCharacter* UHurtStimuli::GetDamageDealer()
{
	return DamageDealer;
}

void UHurtStimuli::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	DamageDealer = Cast<ABaseHeroCharacter>(DamageCauser);
	DamageDone = Damage;
}
