// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseItemPack.h"

#include "Actors/BaseShooterCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
ABaseItemPack::ABaseItemPack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ABaseItemPack::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ABaseItemPack::OnOverlapBegin);
}

// Called every frame
void ABaseItemPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (RechargeTimer > 0.0f)
	{
		RechargeTimer -= DeltaTime;
		if (RechargeTimer <= 0.0f)
		{
			//Reactivate the ammo pack
			Mesh->SetMaterial(0, ActiveMaterial);
			RechargeTimer = 0.0f;

			//Check next if there is still an shooter character within the box collision
			// Define the box bounds
			const FVector BoxCenter = GetActorLocation();
			const FVector BoxExtent((100.0f*GetActorScale().X)/2, (100.0f*GetActorScale().Y)/2, (100.0f*GetActorScale().Z)/2); // Half extents
			
			// Query all overlapping actors
			TArray<FOverlapResult> OverlapResults;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);  // Ignore the current actor

			// Perform overlap check
			const bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
				OverlapResults,
				BoxCenter,
				FQuat::Identity,
				FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
				FCollisionShape::MakeBox(BoxExtent),
				QueryParams
			);

			// Process the results
			if (bHasOverlap && OverlapResults.Num() > 0)
			{
				for (int i=0; i<OverlapResults.Num(); i++)
				{
					if (ABaseShooterCharacter* ShooterCharacter = Cast<ABaseShooterCharacter>(OverlapResults[i].GetActor()))
					{
						AttemptGivePackTo(ShooterCharacter);
					}
				}
			}
		}
	}
}

bool ABaseItemPack::IsRecharging()
{
	return RechargeTimer > 0.0f;
}

void ABaseItemPack::AttemptGivePackTo(ABaseShooterCharacter* TargetShooterCharacter)
{
	if (!IsRecharging() && PackValidation(TargetShooterCharacter))
	{
		GivePackTo(TargetShooterCharacter);
	}
}

void ABaseItemPack::GivePackTo(ABaseShooterCharacter* TargetShooterCharacter)
{
	RechargeTimer = TimeToRecharge;
	Mesh->SetMaterial(0, InactiveMaterial);
}

bool ABaseItemPack::PackValidation(ABaseShooterCharacter* TargetShooterCharacter)
{
	return true;
}

void ABaseItemPack::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ABaseShooterCharacter* ShooterCharacter = Cast<ABaseShooterCharacter>(OtherActor))
	{
		AttemptGivePackTo(ShooterCharacter);
	}
}