// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/SinglePlayer/WraithAbilities/SP_HeroAbility_Wraith_ShootRiffle.h"

#include "Actors/BaseHeroCharacter.h"
#include "Actors/RotationViewPointRef.h"
#include "Actors/SinglePlayer/SP_HeroCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Stimuli/SoundStimuli/SoundStimuli_BulletImpactSound.h"
#include "Stimuli/SoundStimuli/SoundStimuli_ShootingSound.h"


// Sets default values for this component's properties
USP_HeroAbility_Wraith_ShootRiffle::USP_HeroAbility_Wraith_ShootRiffle()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Bind the spread update delegate once
	FireTimerTimerDel.BindUFunction(this, FName("Fire"));
	SpreadUpdateTimerDel.BindUFunction(this, FName("UpdateSpread"));
}


// Called when the game starts
void USP_HeroAbility_Wraith_ShootRiffle::BeginPlay()
{
	Super::BeginPlay();
	CurrentBulletSpreadRadius = BaseBulletSpreadRadius;
}

void USP_HeroAbility_Wraith_ShootRiffle::StartAbility_Implementation()
{
	Super::StartAbility_Implementation();
	if (!TriggerPulled)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				FireTimerHandle,
				FireTimerTimerDel,
				TimeBetweenRound,
				true,
				0.0f
			);
		}

		if (!GetWorld()->GetTimerManager().IsTimerActive(SpreadUpdateHandle))
		{
			// Start updating spread
			GetWorld()->GetTimerManager().SetTimer(
				SpreadUpdateHandle,
				SpreadUpdateTimerDel,
				0.016f, // ~60fps
				true
			);
		}
		
		TriggerPulled = true;
	}
}

void USP_HeroAbility_Wraith_ShootRiffle::EndAbility_Implementation()
{
	Super::EndAbility_Implementation();
	if (TriggerPulled)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
		TriggerPulled = false;
	}
}

void USP_HeroAbility_Wraith_ShootRiffle::Fire()
{
	if (ABaseHeroCharacter* ShooterCharacter = Cast<ABaseHeroCharacter>(GetOwner())) 
	{
		bool bIsReloading = ShooterCharacter->GetIsReloading();
		// Check if spread update timer is not active and restart it
		if (!GetWorld()->GetTimerManager().IsTimerActive(SpreadUpdateHandle) && !bIsReloading && GetAmmoAmount() > 0)
		{
			
			GetWorld()->GetTimerManager().SetTimer(
				SpreadUpdateHandle,
				SpreadUpdateTimerDel,
				0.016f, // ~60fps
				true
			);
		}
	}
	
	// was in the SP version of the gun, might need to change
	ASP_HeroCharacter* CharacterOwner = Cast<ASP_HeroCharacter>(GetOwner());
	if (CharacterOwner == nullptr)
	{
		return;	
	}
	
	if (CharacterOwner->GetIsReloading()) return;

	if (UseAmmo())
	{
		UParticleSystemComponent* MuzzleFlashParticle = UGameplayStatics::SpawnEmitterAttached(
			MuzzleFlash, 
			CharacterOwner->GetMesh(), 
			TEXT("MuzzleFlashSocket"), 
			FVector::ZeroVector, 
			FRotator::ZeroRotator, 
			EAttachLocation::SnapToTarget, 
			true   // bAutoDestroy
		);
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, CharacterOwner->GetMesh(), TEXT("MuzzleFlashSocket"));

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

bool USP_HeroAbility_Wraith_ShootRiffle::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	const AController* OwnerController = GetOwnerController();
	ASP_HeroCharacter* CharacterOwner = Cast<ASP_HeroCharacter>(GetOwner());
	
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
	Params.AddIgnoredActor(GetOwner());
	
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void USP_HeroAbility_Wraith_ShootRiffle::UpdateSpread()
{
	if (ABaseHeroCharacter* ShooterCharacter = Cast<ABaseHeroCharacter>(GetOwner())) 
	{
		bool bIsReloading = ShooterCharacter->GetIsReloading();
	
		if (TriggerPulled && GetAmmoAmount() > 0 && !bIsReloading)  // Only increase spread if we have ammo and not reloading
		{
			CurrentBulletSpreadRadius = FMath::Min(
				CurrentBulletSpreadRadius + (SpreadIncreaseRate * 0.016f),
				MaxBulletSpreadRadius
			);
		}
		else
		{
			CurrentBulletSpreadRadius = FMath::Max(
				CurrentBulletSpreadRadius - (SpreadRecoveryRate * 0.016f),
				BaseBulletSpreadRadius
			);
			
			// Stop updating if we've recovered
			if (CurrentBulletSpreadRadius <= BaseBulletSpreadRadius)
			{
				GetWorld()->GetTimerManager().ClearTimer(SpreadUpdateHandle);
			}
		}

		if (bDebugBulletSpread)
		{
			// Log the current bullet spread radius
			UE_LOG(LogTemp, Warning, TEXT("Current Bullet Spread Radius: %f"), CurrentBulletSpreadRadius);
		}
	}
}

AController* USP_HeroAbility_Wraith_ShootRiffle::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	
	return OwnerPawn->GetController();
}