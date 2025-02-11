// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComponents/BTService_CreateEngagementLocation.h"
#include "AIController.h"
#include "Actors/BaseShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/NavMeshUtility.h"

UBTService_CreateEngagementLocation::UBTService_CreateEngagementLocation()
{
	NodeName = TEXT("Create Engagement Location");
}

void UBTService_CreateEngagementLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBehaviorTreeComponent* OwnerCompPtr = &OwnerComp;
	const ABaseShooterCharacter* EnemyInSight = Cast<ABaseShooterCharacter>(OwnerCompPtr->GetBlackboardComponent()->GetValueAsObject(FName("EnemyInSight")));
	const ABaseShooterCharacter* AICharacter = Cast<ABaseShooterCharacter>(Cast<ABaseShooterCharacter>(OwnerCompPtr->GetAIOwner()->GetPawn()));
	
	if (EnemyInSight != nullptr)
	{
		TArray<FVector> AllValidPositions;
		FVector StartPoint = OwnerCompPtr->GetAIOwner()->GetPawn()->GetActorLocation();
		
		const FVector BotForward = EnemyInSight->GetActorLocation() - OwnerCompPtr->GetAIOwner()->GetPawn()->GetActorLocation();
		FVector2D BotForward2D = FVector2D(BotForward.X, BotForward.Y);
		BotForward2D.Normalize();
		FVector BotForwardXY = FVector(BotForward2D.X, BotForward2D.Y, 0.0f);
		FVector BotRightXY = BotForwardXY.RotateAngleAxis(90.0f, FVector::UpVector);
		
		// Draw the debug line
		if (bDebug) DrawDebugLine(GetWorld(), StartPoint, StartPoint + BotForwardXY*200.0f, FColor::Green,false,0.25f,0,1.0f);
		if (bDebug) DrawDebugLine(GetWorld(), StartPoint, StartPoint + BotRightXY*200.0f, FColor::Green,false,0.25f,0,1.0f);

		FHitResult Hit;
		FHitResult Hit2;
		FHitResult Hit3;
		
		//TODO: will have to find another way to ignore shooter character later
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(OwnerCompPtr->GetAIOwner()->GetPawn());
		Params.AddIgnoredActor(EnemyInSight);

		// Get the delta position of the eyes from the actor position, so we can add it to the evaluated position
		FVector Location;
		FRotator Rotation;
		OwnerCompPtr->GetAIOwner()->GetPlayerViewPoint(Location, Rotation);
		FVector DeltaEyesPosition = Location - OwnerCompPtr->GetAIOwner()->GetPawn()->GetActorLocation();

		// Current Max Distance to target
		float MaxDistanceFromTarget = 0.0f;
		
		for (int i=0; i<nbVerticalPoints; i++)
		{
			FVector PointPosition = StartPoint + BotForwardXY*(distanceBetweenPoints*(i-offset)) - BotRightXY*(distanceBetweenPoints*(nbHorizontalPoints/2));
			for (int j=0; j<nbHorizontalPoints; j++)
			{
				//Get the ceiling position
				if (GetWorld()->LineTraceSingleByChannel(Hit, PointPosition, PointPosition + FVector::UpVector*99999.9f, ECollisionChannel::ECC_GameTraceChannel1, Params))
				{
					FVector CeilPosition = Hit.Location;
					CeilPosition.Z -= 50.0f;

					//Get the floor position
					if (GetWorld()->LineTraceSingleByChannel(Hit2, CeilPosition, CeilPosition + FVector::UpVector*-99999.9f, ECollisionChannel::ECC_GameTraceChannel1, Params))
					{
						FVector GroundLocation = Hit2.Location;
						GroundLocation.Z += GroundZOffset;

						// Check if no Obstacle is set between the point and the bot
						if (!GetWorld()->LineTraceSingleByChannel(Hit3, GroundLocation, StartPoint, ECollisionChannel::ECC_GameTraceChannel1, Params))
						{
							//Check if the point is on the navmesh
							if (AICharacter->NavMeshUtility->IsPointOnNavmesh(GroundLocation, GetWorld()))
							{
								//Check if the bot could still see his target from the new position
								FVector EyesFutureLocation = GroundLocation + DeltaEyesPosition;
								EyesFutureLocation.Z += Cast<ABaseShooterCharacter>(OwnerCompPtr->GetAIOwner()->GetPawn())->FootPositionAnchor.Z*-1.0f;

								if (!GetWorld()->LineTraceSingleByChannel(Hit, EyesFutureLocation, EnemyInSight->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel1, Params))
								{
									if (FVector::Distance(GroundLocation, EnemyInSight->GetActorLocation() + EnemyInSight->FootPositionAnchor) > MinDistanceToTarget)
									{
										if (bDebug)DrawDebugSphere(GetWorld(), GroundLocation, 5.0f, 12, FColor::Green, false, 0.25f, 0, 1.0);

										FVector DeltaToEnemy = EnemyInSight->GetActorLocation() - GroundLocation;
										DeltaToEnemy.Z = 0;
										float DistancePointXYToEnemy = DeltaToEnemy.Length();

										if (MaxDistanceFromTarget < DistancePointXYToEnemy) MaxDistanceFromTarget = DistancePointXYToEnemy;
										
										AllValidPositions.Add(GroundLocation);
									}
									else
									{
										// position is too close
										if (bDebug)DrawDebugSphere(GetWorld(), GroundLocation, 5.0f, 12, FColor::Purple, false, 0.25f, 0, 1.0);
									}
								}
								else
								{
									// can't shoot the target from this position
									if (bDebug)DrawDebugSphere(GetWorld(), GroundLocation, 5.0f, 12, FColor::Yellow, false, 0.25f, 0, 1.0);
								}
							}
							else
							{
								// point is not on the navmesh
								if (bDebug)DrawDebugSphere(GetWorld(), GroundLocation, 5.0f, 12, FColor::Blue, false, 0.25f, 0, 1.0);
							}
						}
						else
						{
							// obstacle between the bot and the point (likely to be a wall)
							if (bDebug)DrawDebugSphere(GetWorld(), Hit2.Location, 5.0f, 12, FColor::Red, false, 0.25f, 0, 1.0);
						}
						
					}
				}
				PointPosition += BotRightXY*distanceBetweenPoints;
			}
		}
		
		if (AllValidPositions.Num() > 0)
		{
			//Evaluate which point is the most suitable to go to
			float HighestScore = 0.0f;
			int IndexValidPosition = 0;
			
			for (int i=0; i<AllValidPositions.Num(); i++)
			{
				FVector FootAnchorPosition = Cast<ABaseShooterCharacter>(OwnerCompPtr->GetAIOwner()->GetPawn())->FootPositionAnchor;
			
				FVector DeltaToEnemy = EnemyInSight->GetActorLocation() - AllValidPositions[i];
				float ZDistance = DeltaToEnemy.Z*-1.0f;
				DeltaToEnemy.Z = 0;
				float DistancePointXYToEnemy = DeltaToEnemy.Length();
			
				FVector DeltaToPosition = AllValidPositions[i] - (OwnerCompPtr->GetAIOwner()->GetPawn()->GetActorLocation() + FootAnchorPosition);
				float DistanceToNewPosition = DeltaToPosition.Length();

				//Desired Distance Score
				float DesiredXYScore = 1.0f - (FMath::Min(FMath::Abs(DistancePointXYToEnemy - DesiredXYDistance)/ThresholdDistance, 1));
				
				//all the point return 0 because the enemy is too far or to close, take the closest point or farthest point known so far so you get closer to the enemy
				float DistanceToEnemy = 0.0f;
				if (DesiredXYDistance - MaxDistanceFromTarget > 0) // MaxDistanceFromTarget is the highest distance among the valid point
				{
					DistanceToEnemy = DistancePointXYToEnemy/MaxDistanceFromTarget; // Favor point that are far away from the target
				}
				else
				{
					DistanceToEnemy = 1.0f-(DistancePointXYToEnemy/MaxDistanceFromTarget); // Favor point that are closer to the target
				}
				
				float AggregatedDistanceToEnemyScore = DesiredXYScore*(1.0f-WeightFactorCurrentMaxDistanceToEnemy) + DistanceToEnemy*(WeightFactorCurrentMaxDistanceToEnemy);
				AggregatedDistanceToEnemyScore = AggregatedDistanceToEnemyCurve.GetRichCurveConst()->Eval(AggregatedDistanceToEnemyScore);

				//Higher Ground Score
				float HigherGroundScore = FMath::Min(FMath::Max(0.0f, ZDistance), HigherGroundDistance)/HigherGroundDistance;
				HigherGroundScore = HigherGroundDistanceCurve.GetRichCurveConst()->Eval(HigherGroundScore);

				//Distance from current position score
				float DistanceFromCurrentPositionScore = FMath::Min(DistanceToNewPosition, MinDistanceFromCurrentPosition) / MinDistanceFromCurrentPosition;
				DistanceFromCurrentPositionScore = MinDistanceFromCurrentPositionCurve.GetRichCurveConst()->Eval(DistanceFromCurrentPositionScore);
				
				float AggregatedScore = ScoreAggregation(3, 1.0f * AggregatedDistanceToEnemyScore * HigherGroundScore * DistanceFromCurrentPositionScore);
				AggregatedScore = FMath::Min(AggregatedScore + FMath::RandRange(0.0f, MaxRandomBonusScore), 1.0f);
				if (bDebug) DrawDebugString(GetWorld(), AllValidPositions[i], FString::Printf(TEXT("%f"), AggregatedScore), 0, FColor::Red, 0.25f, false, 1);
				
				if (AggregatedScore > HighestScore)
				{
					HighestScore = AggregatedScore;
					IndexValidPosition = i;
				}
			}
			
			OwnerCompPtr->GetBlackboardComponent()->SetValueAsVector(FName("EngagementLocation"), AllValidPositions[IndexValidPosition]);
			if (bDebug)DrawDebugSphere(GetWorld(), AllValidPositions[IndexValidPosition], 20.0f, 12, FColor::Cyan, false, 0.25f, 0, 1.0);
		}
		else
		{
			OwnerCompPtr->GetBlackboardComponent()->ClearValue(FName("EngagementLocation"));
		}
	}
	else
	{
		OwnerCompPtr->GetBlackboardComponent()->ClearValue(FName("EngagementLocation"));
	}
}

float UBTService_CreateEngagementLocation::ScoreAggregation(int NbConsiderations, float OriginalScore)
{
	const float modFactor = 1 - (1 / NbConsiderations);
	const float makeupValue = (1 - OriginalScore) * modFactor;
	const float FinalScore = OriginalScore + (makeupValue * OriginalScore);
	
	return FinalScore;
}
