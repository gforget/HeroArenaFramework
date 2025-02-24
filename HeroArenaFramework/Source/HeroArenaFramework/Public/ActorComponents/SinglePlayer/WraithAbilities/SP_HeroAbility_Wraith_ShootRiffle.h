// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/SinglePlayer/SP_HeroAbility.h"
#include "SP_HeroAbility_Wraith_ShootRiffle.generated.h"

class ASoundStimuli_ShootingSound;
class ASoundStimuli_BulletImpactSound;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEROARENAFRAMEWORK_API USP_HeroAbility_Wraith_ShootRiffle : public USP_HeroAbility
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USP_HeroAbility_Wraith_ShootRiffle();

	UFUNCTION()
	void Fire();
	FTimerDelegate FireTimerTimerDel;
	FTimerHandle FireTimerHandle;

	void PullTrigger();
	void ReleaseTrigger();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void StartAbility_Implementation() override;
	virtual void EndAbility_Implementation() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASoundStimuli_ShootingSound> SoundStimuli_ShootingSoundClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASoundStimuli_BulletImpactSound> SoundStimuli_BulletImpactSoundClass;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;
	
	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.0f;

	UPROPERTY(EditAnywhere)
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	float TimeBetweenRound = 0.15f;
	
	UPROPERTY(EditDefaultsOnly)
	float BaseBulletSpreadRadius = 200.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxBulletSpreadRadius = 800.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float SpreadIncreaseRate = 400.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float SpreadRecoveryRate = 600.0f;

	UPROPERTY(EditDefaultsOnly)
	bool bDebugBulletSpread;
	
	UPROPERTY()
	float CurrentBulletSpreadRadius;

	UPROPERTY()
	bool TriggerPulled = false;
	
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

	UFUNCTION()
	void UpdateSpread();
	FTimerHandle SpreadUpdateHandle;
	FTimerDelegate SpreadUpdateTimerDel;
};
