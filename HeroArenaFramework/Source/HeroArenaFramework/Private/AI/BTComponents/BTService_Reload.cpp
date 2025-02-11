// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTService_Reload.h"

#include "AIController.h"
#include "Actors/BaseGun.h"
#include "Actors/BaseShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Reload::UBTService_Reload()
{
	NodeName = TEXT("Reload");
}

void UBTService_Reload::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}
	
	ABaseShooterCharacter* Character = Cast<ABaseShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return;
	}

	const ABaseGun* Gun = Character->GetGunReference();
	if (Character == nullptr)
	{
		return;
	}

	const ABaseShooterCharacter* EnemyInSight = Cast<ABaseShooterCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("EnemyInSight")));
	
	if (!Character->GetIsReloading()
		&& (Gun->GetAmmoPercent() == 0.0f || (EnemyInSight == nullptr && Gun->GetAmmoPercent() < 1.0f))
		&& Character->GetAmmoReservePercent() > 0.0f)
	{
		Character->Reload();
	}

}
