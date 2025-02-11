// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/Multiplayer/MP_Gun.h"

#include "Actors/Multiplayer/MP_ShooterCharacter.h"
#include "Actors/RotationViewPointRef.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Stimuli/SoundStimuli/SoundStimuli_BulletImpactSound.h"
#include "Stimuli/SoundStimuli/SoundStimuli_ShootingSound.h"

// Sets default values
AMP_Gun::AMP_Gun()
{
	bReplicates = false;
}

// Called when the game starts or when spawned
void AMP_Gun::BeginPlay()
{
	Super::BeginPlay();
}

void AMP_Gun::Fire()
{
	Super::Fire();
	AMP_ShooterCharacter* CharacterOwner = Cast<AMP_ShooterCharacter>(GetOwner());
	if (CharacterOwner == nullptr)
	{
		return;	
	}
	
	if (UseAmmo())
	{
		if (CharacterOwner->GetIsReloading()) return;
		
		UParticleSystemComponent* MuzzleFlashParticle = UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

		if (HasAuthority())
		{
			//Create Sound Stimuli for AI
			ASoundStimuli_ShootingSound* SoundStimuli_ShootingSound = GetWorld()->SpawnActor<ASoundStimuli_ShootingSound>(
				SoundStimuli_ShootingSoundClass,
				CharacterOwner->GetActorLocation(), //want a position that is relevant to the sound, the position of the shooter is more relevant
				FRotator(0.0f, 0.0f, 0.0f)
			);
		
			SoundStimuli_ShootingSound->SetSoundOwner(CharacterOwner);
		}
		
		FHitResult Hit;
		FVector ShotDirection;
		
		bool bSuccess = GunTrace(Hit, ShotDirection);
		
		if (bSuccess)
		{
			UParticleSystemComponent* ImpactParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
			if (HasAuthority())
			{
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
	}
	else
	{
		//TODO : clip sound
	}
}

bool AMP_Gun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AMP_ShooterCharacter* CharacterOwner = Cast<AMP_ShooterCharacter>(GetOwner());
	
	if (CharacterOwner == nullptr) return false;
	
	ShotDirection = -CharacterOwner->ShooterViewPointRotation.Vector();
	
	FVector End = CharacterOwner->ShooterViewPointLocation + CharacterOwner->ShooterViewPointRotation.Vector()*MaxRange;
	
	//Random offset //TODO: the random has to be replicated, or some bullet might miss on the client
	FVector2D result = FVector2D(FMath::VRand()); 
	result.Normalize();
	result *= FMath::RandRange(0.0f,CurrentBulletSpreadRadius);
	
	End += CharacterOwner->GetRotationViewPointRef()->GetActorRightVector()*result.X; //Offset Right
	End += CharacterOwner->GetRotationViewPointRef()->GetActorUpVector()*result.Y; //Offset Up
	//End Random offset
	
	// Debug line trace
	if (bDebugBulletSpread)
	{
		DrawDebugLine(GetWorld(), CharacterOwner->ShooterViewPointLocation, End, FColor::Red, false, 1.0f, 0, 1.0f);
	}
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	
	return GetWorld()->LineTraceSingleByChannel(Hit, CharacterOwner->ShooterViewPointLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}