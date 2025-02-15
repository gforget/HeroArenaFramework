// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/TeamManager.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BaseHeroCharacter.generated.h"

class ABaseSpectatorPawn;
class ABaseGun;
class USphereComponent;
class UNavMeshUtility;
class ARotationViewPointRef;
class AVisualStimuli_ShooterCharacter;
class UPlayMontageCallbackProxy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMP_HealEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeadEvent, ABaseHeroCharacter*, DeadShooterCharacter);

UCLASS(Abstract)
class HEROARENAFRAMEWORK_API ABaseHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	ABaseHeroCharacter();

	UFUNCTION(BlueprintCallable)
	float GetAmmoReservePercent() const;

	int AddAmmoReserve(int AmmoAmount);
	
	UPROPERTY(EditDefaultsOnly, Category="Position Reference")
	FVector HealthBarAnchor = FVector(0.0f, 0.0f, 88.0f);

	UPROPERTY(EditDefaultsOnly, Category="Position Reference")
	FVector FootPositionAnchor = FVector(0.0f, 0.0f, 25.0f);

	UPROPERTY(EditDefaultsOnly, Category="Position Reference")
	FVector BodyPositionAnchor = FVector(0.0f, 0.0f, 50.0f);
	
	UPROPERTY()
	FMP_HealEvent OnHealEvent;
	
	UPROPERTY()
	FDeadEvent OnDeadEvent;

	UFUNCTION(BlueprintPure)
	ETeam GetTeam() const;
	
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	float Heal(float HealAmount);

	UFUNCTION(BlueprintCallable)
	ABaseGun* GetGunReference() const;
	
	UPROPERTY()
	UNavMeshUtility* NavMeshUtility;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	FVector GetHeadAnchorLocation() const;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool GetIsReloading() const;

	UFUNCTION(BlueprintCallable)
	float GetAmmoTotalPercent() const;

	UFUNCTION(BlueprintCallable)
	FString GetAmmoReserveRatio() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	AVisualStimuli_ShooterCharacter* GetVSShooterCharacter();
	
	ARotationViewPointRef* GetRotationViewPointRef();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnReloadAnimationCompleted(FName NotifyName);
	
	virtual void Reload();
	virtual void PullTrigger();
	virtual void ReleaseTrigger();

	/** The default input mapping context to add on BeginPlay */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	/** The input action for movement (configured as a 2D axis) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	/** The input action for movement (configured as a 2D axis) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookRateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* ReloadAction;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Stimuli")
	TSubclassOf<AVisualStimuli_ShooterCharacter> VisualStimuli_ShooterCharacterClass;
	
	UPROPERTY()
	AVisualStimuli_ShooterCharacter* VSShooterCharacter;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostActorCreated() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
	void GenerateEditorAnchorPositionVisualisation() const;
#endif
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARotationViewPointRef> RotationViewPointRefClass;

	UPROPERTY()
	ARotationViewPointRef* RotationViewPointRef;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	USphereComponent* HeadCollision;

	// Update collision sphere location
	void UpdateHeadCollision();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float HeadshotMultiplier = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	FName HeadBoneName = TEXT("head");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	FVector HeadAnchorOffset = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float HeadshotRadius = 15.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	bool bShowHeadshotDebug = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Alliances")
	TEnumAsByte<ETeam> Team = ETeam::NoTeam;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float MaxHealth = 100.0f;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Combat")
	float Health = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category="Gun")
	int MaxAmmoReserve = 100;

	UPROPERTY(EditDefaultsOnly, Category="Gun")
	int AmmoReserve = 20;

	UPROPERTY(EditDefaultsOnly, Category="Gun")
	TSubclassOf<ABaseGun> GunClass;

	UPROPERTY()
	ABaseGun* Gun;

	UPROPERTY()
	bool IsReloading = false;
	
	UPROPERTY()
	bool Dead = false;

	UPROPERTY(EditAnywhere)
	float RotationRate = 100.0f;
	
	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void LookRateInput(const FInputActionValue& Value);
	void JumpInput(const FInputActionValue& Value);
	void ShootInput(const FInputActionValue& Value);
	void ReloadInput(const FInputActionValue& Value);
	
	UPROPERTY()
	UPlayMontageCallbackProxy* ProxyReloadPlayMontage;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* ReloadMontage;
	
	void Death();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseSpectatorPawn> ShooterSpectatorPawnClass;


};