// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTService_Reload.h"

#include "AIController.h"
#include "Actors/BaseHeroCharacter.h"
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
	
	ABaseHeroCharacter* Character = Cast<ABaseHeroCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return;
	}
	
	const ABaseHeroCharacter* EnemyInSight = Cast<ABaseHeroCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("EnemyInSight")));
	
	if (!Character->GetIsReloading()
		&& (Character->GetLowestAmmoPercent() == 0.0f || (EnemyInSight == nullptr && Character->GetLowestAmmoPercent() < 1.0f)))
	{
		Character->Reload();
	}

}
