// Fill out your copyright notice in the Description page of Project Settings.


#include "Stimuli/TouchStimuli/HurtStimuli.h"

#include "Actors/SinglePlayer/SP_ShooterCharacter.h"
#include "Controllers/SinglePlayer/SP_ShooterAIController.h"

void UHurtStimuli::Initialize(ABaseShooterAIController* OwnerControllerRef, ABaseShooterCharacter* OwnerCharacterRef)
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

void UHurtStimuli::SetDamageDealer(ABaseShooterCharacter* DamageDealerRef)
{
	DamageDealer = DamageDealerRef;
}

ABaseShooterCharacter* UHurtStimuli::GetDamageDealer()
{
	return DamageDealer;
}

void UHurtStimuli::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	DamageDealer = Cast<ABaseShooterCharacter>(DamageCauser);
	DamageDone = Damage;
}
