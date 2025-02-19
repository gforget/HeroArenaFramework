// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGun.generated.h"

class ASoundStimuli_ShootingSound;
class ASoundStimuli_BulletImpactSound;

UCLASS(Abstract)
class HEROARENAFRAMEWORK_API ABaseGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseGun();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	UFUNCTION()
	virtual void Fire();
	FTimerDelegate FireTimerTimerDel;
	FTimerHandle FireTimerHandle;

	void PullTrigger();
	void ReleaseTrigger();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASoundStimuli_ShootingSound> SoundStimuli_ShootingSoundClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASoundStimuli_BulletImpactSound> SoundStimuli_BulletImpactSoundClass;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

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
	
	virtual bool GunTrace(FHitResult& Hit, FVector& ShotDirection)PURE_VIRTUAL(ABaseGun::GunTrace, return false;);

	AController* GetOwnerController() const;

	UFUNCTION()
	void UpdateSpread();
	FTimerHandle SpreadUpdateHandle;
	FTimerDelegate SpreadUpdateTimerDel;
};
