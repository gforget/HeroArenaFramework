// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTService_UpdateStimuliInfo.h"

#include "AIController.h"
#include "Actors/BaseShooterCharacter.h"
#include "Stimuli/SoundStimuli.h"
#include "Stimuli/VisualStimuli.h"
#include "Stimuli/SoundStimuli/SoundStimuli_BulletImpactSound.h"
#include "Stimuli/SoundStimuli/SoundStimuli_ShootingSound.h"
#include "Stimuli/VisualStimuli/VisualStimuli_ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/BaseShooterAIController.h"
#include "GameMode/Multiplayer/ShooterGameMode/MP_ShooterGameMode.h"
#include "GameMode/SinglePlayer/SP_ShooterGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Stimuli/TouchStimuli/HurtStimuli.h"
#include "Utility/NavMeshUtility.h"

UBTService_UpdateStimuliInfo::UBTService_UpdateStimuliInfo()
{
	NodeName = TEXT("Update Stimuli Info");
}

void UBTService_UpdateStimuliInfo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	OwnerCompPtr = &OwnerComp;

	//initiate blackboard value
	if (ABaseShooterAIController* AIController = Cast<ABaseShooterAIController>(OwnerCompPtr->GetAIOwner()))
	{
		if (!AIController->bStimuliServiceInitiated)
		{
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("TimeSeenAnEnemy"), 9999999.9f);
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("TimeHeardSomething"), 9999999.9f);
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("TimeGotHurt"), 9999999.9f);
			AIController->bStimuliServiceInitiated = true;
		}
	}
	
	if (OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("TimeSeenAnEnemy")) > MaxTimeSeenAnEnemy)
	{
		OwnerCompPtr->GetBlackboardComponent()->ClearValue(FName("LastKnownEnemyLocation"));
	}
	
	ABaseShooterAIController* ShooterAIController = Cast<ABaseShooterAIController>(OwnerCompPtr->GetAIOwner());
	ABaseShooterCharacter* ShooterAICharacter = Cast<ABaseShooterCharacter>(OwnerCompPtr->GetAIOwner()->GetPawn());
	
	if (OwnerCompPtr != nullptr && ShooterAICharacter != nullptr && ShooterAIController != nullptr)
	{
		// Touch Stimuli - Hurt Stimuli
		if (ShooterAIController->GetHurtStimuli()->GetDamageDealer() != nullptr)
		{
			const FVector EnemyLocation = ShooterAIController->GetHurtStimuli()->GetDamageDealer()->GetActorLocation();
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsVector(
				FName("LastKnownEnemyLocation"),
				EnemyLocation
				);
			
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("TimeGotHurt"), 0.0f);
			ShooterAIController->GetHurtStimuli()->SetDamageDealer(nullptr);
		}
		else
		{
			float TimeGotHurt = OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("TimeGotHurt"));
			TimeGotHurt += DeltaSeconds;
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("TimeGotHurt"), TimeGotHurt);
		}
		
		// Visual Stimuli collection
		TArray<AActor*> VSFound;
		
		TArray<AActor*> VSToIgnore;
		VSToIgnore.Add(ShooterAICharacter->GetVSShooterCharacter());

		const TSubclassOf<AActor> filterVS = AVisualStimuli::StaticClass();
		
		TArray<TEnumAsByte<EObjectTypeQuery>> VSQuery;
		VSQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));
		
		bool bVSHasHit = UKismetSystemLibrary::SphereOverlapActors(
			GetWorld(),
			ShooterAIController->GetPawn()->GetActorLocation(),
			ShooterAIController->GetSightRange(),
			VSQuery,
			filterVS,
			VSToIgnore,
			VSFound);
		
		bool bHaveSeenAnEnemy = false;
		if (bVSHasHit)
		{
			const ABaseShooterCharacter* ShooterInSight = Cast<ABaseShooterCharacter> (OwnerCompPtr->GetBlackboardComponent()->GetValueAsObject(FName("EnemyInSight")));
			
			for (int i=0; i<VSFound.Num(); i++)
			{
				//VS of ShooterCharacter
				AVisualStimuli_ShooterCharacter* VS_Shooter = Cast<AVisualStimuli_ShooterCharacter> (VSFound[i]);
				if (VS_Shooter != nullptr) 
				{
					// do not reset target if you already have a target
					if (ShooterInSight != nullptr)
					{
						if (ShooterInSight->GetUniqueID() != VS_Shooter->GetShooterCharacterRef()->GetUniqueID())
						{
							continue;
						}
					}
					
					//Check if it is an enemy
					
					//Since SP and MP GameMode do not inherit from the same base, we need to check both
					ASP_ShooterGameMode* SP_GameMode = GetWorld()->GetAuthGameMode<ASP_ShooterGameMode>();
					AMP_ShooterGameMode* MP_GameMode = GetWorld()->GetAuthGameMode<AMP_ShooterGameMode>();

					if (SP_GameMode == nullptr && MP_GameMode == nullptr)
					{
						return;
					}
					
					ETeamRelation TargetTeamRelation;
					if (SP_GameMode != nullptr)
					{
						TargetTeamRelation = SP_GameMode->FactionManagerComponent->GetTeamRelation(VS_Shooter->GetShooterCharacterRef()->GetTeam(), ShooterAICharacter->GetTeam());
					}
					else
					{
						TargetTeamRelation = MP_GameMode->FactionManagerComponent->GetTeamRelation(VS_Shooter->GetShooterCharacterRef()->GetTeam(), ShooterAICharacter->GetTeam());
					}

					if (TargetTeamRelation == ETeamRelation::Ally)
					{
						continue;
					}
					
					//check if you have line of sight first
					if (OwnerCompPtr->GetAIOwner()->LineOfSightTo(VS_Shooter->GetShooterCharacterRef()))
					{
						FVector EyesLocation;
						FRotator EyesRotation;
						OwnerCompPtr->GetAIOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotation);

						const FVector EyesForward = EyesRotation.Vector();
						FVector EyesToOtherShooterCharacter = VS_Shooter->GetShooterCharacterRef()->GetActorLocation() - EyesLocation;
						EyesToOtherShooterCharacter.Normalize();

						//vision cone
						// factor of 0 to 1, 0 = 180 degree vision, 1 = 0 degree vision, 0.5 = 90 (45 split from the middle)
						if (FVector::DotProduct(EyesForward, EyesToOtherShooterCharacter) > (1-(ShooterAIController->GetFieldOfView()/180.0f))) 
						{
							OwnerCompPtr->GetBlackboardComponent()->SetValueAsObject(FName("EnemyInSight"), VS_Shooter->GetShooterCharacterRef());
							OwnerCompPtr->GetBlackboardComponent()->SetValueAsVector(FName("LastKnownEnemyLocation"), VS_Shooter->GetShooterCharacterRef()->GetActorLocation());
							OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("TimeSeenAnEnemy"), 0.0f);
							bHaveSeenAnEnemy = true;
						}
					}
				}
			}
		}

		if (!bHaveSeenAnEnemy)
		{
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsObject(FName("EnemyInSight"), nullptr);
			float TimeRegistered = OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("TimeSeenAnEnemy"));
			TimeRegistered += DeltaSeconds;
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("TimeSeenAnEnemy"), TimeRegistered); 
		}
		
		// Sound Stimuli Collection
		TArray<AActor*> SSFound;
		TArray<AActor*> SSToIgnore;

		const TSubclassOf<AActor> filterSS = ASoundStimuli::StaticClass();
		
		TArray<TEnumAsByte<EObjectTypeQuery>> SSQuery;
		SSQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));
		
		bool bSSHasHit = UKismetSystemLibrary::SphereOverlapActors(
			GetWorld(),
			ShooterAIController->GetPawn()->GetActorLocation(),
			ShooterAIController->GetHearingRange(),
			SSQuery,
			filterSS,
			SSToIgnore,
			SSFound);
		
		bool bJustHeardSomething = false;
		if (bSSHasHit)
		{
			for (int i=0; i<SSFound.Num(); i++)
			{
				//Skip the analysis if the sound owner is the ShooterCharacter
				ASoundStimuli* SoundStimuli = Cast<ASoundStimuli> (SSFound[i]);
				if (SoundStimuli->GetSoundOwner()->GetUniqueID() == ShooterAICharacter->GetUniqueID())
				{
					continue;
				}
				
				//SS of ShootingSound
				if (ASoundStimuli_ShootingSound* SS_ShootingSound = Cast<ASoundStimuli_ShootingSound> (SSFound[i]))
				{
					if (OwnerCompPtr->GetAIOwner()->LineOfSightTo(SS_ShootingSound) &&
						OwnerCompPtr->GetBlackboardComponent()->GetValueAsInt("SoundPriorityLevel") <= SS_ShootingSound->PriorityLevel)
					{

						FVector ValidPosition;
						if (ShooterAICharacter->NavMeshUtility->FindValidNavmeshPosition(SS_ShootingSound->GetActorLocation(), GetWorld(),ValidPosition))
						{
							OwnerCompPtr->GetBlackboardComponent()->SetValueAsVector(FName("SoundHeardLocation"), ValidPosition);
							OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("TimeHeardSomething"), 0.0f);
							OwnerCompPtr->GetBlackboardComponent()->SetValueAsInt(FName("SoundPriorityLevel"), SS_ShootingSound->PriorityLevel);
							bJustHeardSomething = true;
						}
					}
				}
				
				//SS of BulletImpact
				if (ASoundStimuli_BulletImpactSound* SS_BulletImpact= Cast<ASoundStimuli_BulletImpactSound> (SSFound[i]))
				{
					if (OwnerCompPtr->GetAIOwner()->LineOfSightTo(SS_BulletImpact) &&
						OwnerCompPtr->GetBlackboardComponent()->GetValueAsInt("SoundPriorityLevel") <= SS_BulletImpact->PriorityLevel)
					{
						FVector ValidPosition;
						if (ShooterAICharacter->NavMeshUtility->FindValidNavmeshPosition(SS_BulletImpact->GetActorLocation(), GetWorld(),ValidPosition))
						{
							OwnerCompPtr->GetBlackboardComponent()->SetValueAsVector(FName("SoundHeardLocation"), SS_BulletImpact->GetShootingOriginPosition());
							OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("TimeHeardSomething"), 0.0f);
							OwnerCompPtr->GetBlackboardComponent()->SetValueAsInt(FName("SoundPriorityLevel"), SS_BulletImpact->PriorityLevel);
							bJustHeardSomething = true;
						}
					}
				}
			}
		}
		
		if (!bJustHeardSomething)
		{
			float TimeRegistered = OwnerCompPtr->GetBlackboardComponent()->GetValueAsFloat(FName("TimeHeardSomething"));
			TimeRegistered += DeltaSeconds;
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsFloat(FName("TimeHeardSomething"), TimeRegistered); 
		}
	}
}
