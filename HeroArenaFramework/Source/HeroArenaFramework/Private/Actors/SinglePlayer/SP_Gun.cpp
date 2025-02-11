// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/SinglePlayer/SP_Gun.h"

#include "Actors/RotationViewPointRef.h"
#include "Actors/SinglePlayer/SP_ShooterCharacter.h"
#include "Stimuli/SoundStimuli/SoundStimuli_BulletImpactSound.h"
#include "Stimuli/SoundStimuli/SoundStimuli_ShootingSound.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASP_Gun::ASP_Gun()
{
	
}

// Called when the game starts or when spawned
void ASP_Gun::BeginPlay()
{
	Super::BeginPlay();
}

void ASP_Gun::Fire()
{
	Super::Fire();
	ASP_ShooterCharacter* CharacterOwner = Cast<ASP_ShooterCharacter>(GetOwner());
	if (CharacterOwner == nullptr)
	{
		return;	
	}
	
	if (UseAmmo())
	{
		if (CharacterOwner->GetIsReloading()) return;
		UParticleSystemComponent* MuzzleFlashParticle = UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

		//Create Sound Stimuli for AI
		ASoundStimuli_ShootingSound* SoundStimuli_ShootingSound = GetWorld()->SpawnActor<ASoundStimuli_ShootingSound>(
			SoundStimuli_ShootingSoundClass,
			CharacterOwner->GetActorLocation(), //want a position that is relevant to the sound, the position of the shooter is more relevant
			FRotator(0.0f, 0.0f, 0.0f)
		);
		
		SoundStimuli_ShootingSound->SetSoundOwner(CharacterOwner);
		
		FHitResult Hit;
		FVector ShotDirection;
		bool bSuccess = GunTrace(Hit, ShotDirection);
		
		if (bSuccess)
		{
			UParticleSystemComponent* ImpactParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

			//Create Sound Stimuli for AI
			ASoundStimuli_BulletImpactSound* SoundStimuli_BulletImpactSound = GetWorld()->SpawnActor<ASoundStimuli_BulletImpactSound>(
				SoundStimuli_BulletImpactSoundClass,
				ImpactParticle->GetComponentLocation(),
				FRotator(0.0f, 0.0f, 0.0f)
			);
			
			SoundStimuli_BulletImpactSound->SetSoundOwner(CharacterOwner);
			SoundStimuli_BulletImpactSound->SetShootingOriginPosition(CharacterOwner->GetActorLocation()); //want a position that is relevant to the sound, the position of the shooter is more relevant
			
			AActor* HitActor = Hit.GetActor();
			if (HitActor != nullptr)
			{
				FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
				AController* OwnerController = GetOwnerController();
				HitActor->TakeDamage(Damage, DamageEvent, OwnerController, GetOwner());
			}
		}
	}
	else
	{
		//TODO : clip sound
	}
}

bool ASP_Gun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	const AController* OwnerController = GetOwnerController();
	ASP_ShooterCharacter* CharacterOwner = Cast<ASP_ShooterCharacter>(GetOwner());
	
	if (OwnerController == nullptr || CharacterOwner == nullptr) return false;
	
	FVector Location;
	FRotator Rotation;
	
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();
	
	FVector End = Location + Rotation.Vector()*MaxRange;
	
	//Random offset
	FVector2D result = FVector2D(FMath::VRand()); 
	result.Normalize();
	result *= FMath::RandRange(0.0f, CurrentBulletSpreadRadius);

	End += CharacterOwner->GetRotationViewPointRef()->GetActorRightVector()*result.X; //Offset Right
	End += CharacterOwner->GetRotationViewPointRef()->GetActorUpVector()*result.Y; //Offset Up
	//End Random offset
	
	// Debug line trace
	if (bDebugBulletSpread)
	{
		DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 1.0f, 0, 1.0f);
	}

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

