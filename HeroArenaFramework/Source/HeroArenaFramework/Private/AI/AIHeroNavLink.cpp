// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AIHeroNavLink.h"

#include "AIController.h"
#include "Actors/BaseHeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AAIHeroNavLink::AAIHeroNavLink()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointLinks.Empty();
	bSmartLinkIsRelevant = true;
}

// Called when the game starts or when spawned
void AAIHeroNavLink::BeginPlay()
{
	Super::BeginPlay();
	OnSmartLinkReached.AddDynamic(this, &AAIHeroNavLink::OnNavLinkReach);
}

// Called every frame
void AAIHeroNavLink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAIHeroNavLink::OnNavLinkReach(AActor* MovingActor, const FVector& DestinationPoint)
{
	if (ABaseHeroCharacter* ShooterCharacter = Cast<ABaseHeroCharacter>(MovingActor))
	{
		ShooterCharacter->GetMovementComponent()->StopMovementImmediately();
		// Stop AI pathfinding to prevent interference
		if (AAIController* AIController = Cast<AAIController>(ShooterCharacter->GetController()))
		{
			AIController->StopMovement();
		}
		
		// Calculate direction to destination
		FVector DeltaToDestination = DestinationPoint - ShooterCharacter->GetActorLocation();
		DeltaToDestination.Z = 0.0f;
		DeltaToDestination.Normalize();
		
		// Apply horizontal velocity
		ShooterCharacter->GetCharacterMovement()->Velocity = DeltaToDestination * 500.0f;
		
		// Trigger jump
		ShooterCharacter->ACharacter::Jump();
	}
}

