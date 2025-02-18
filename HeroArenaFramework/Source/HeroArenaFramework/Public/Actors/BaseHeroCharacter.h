// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/TeamManager.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BaseHeroCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UBaseHeroAbility;
class ABaseSpectatorPawn;
class ABaseGun;
class USphereComponent;
class UNavMeshUtility;
class ARotationViewPointRef;
class AVisualStimuli_ShooterCharacter;
class UPlayMontageCallbackProxy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMP_HealEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeadEvent, ABaseHeroCharacter*, DeadShooterCharacter);

UENUM(BlueprintType)
enum class EAbilityEnum : uint8
{
	Ability1     UMETA(DisplayName = "Ability 1"),
	Ability2     UMETA(DisplayName = "Ability 2"),
	Ability3     UMETA(DisplayName = "Ability 3"),
	Ability4     UMETA(DisplayName = "Ability 4"),
	ShiftAbility UMETA(DisplayName = "Shift Ability"),
	JumpAbility  UMETA(DisplayName = "Jump Ability")
};

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookRateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* GoUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* GoDownAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* UltimateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* QuickMeleeAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShiftAbilityPressAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShiftAbilityHoldAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAbilityAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Ability1PressAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Ability1HoldAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Ability2PressAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Ability2HoldAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Ability3PressAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Ability3HoldAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Ability4PressAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Ability4HoldAction;
	
	TArray<TMap<EAbilityEnum, TSubclassOf<UBaseHeroAbility>>> AllProfiles;
	
protected:

	UPROPERTY(EditAnywhere, Category="Ability")
	TMap<EAbilityEnum, TSubclassOf<UBaseHeroAbility>> AbilityProfile1 = {
		{EAbilityEnum::Ability1, nullptr},
		{EAbilityEnum::Ability2, nullptr},
		{EAbilityEnum::Ability3, nullptr},
		{EAbilityEnum::Ability4, nullptr},
		{EAbilityEnum::ShiftAbility, nullptr},
		{EAbilityEnum::JumpAbility, nullptr}
	};

	UPROPERTY(EditAnywhere, Category="Ability")
	TMap<EAbilityEnum, TSubclassOf<UBaseHeroAbility>> AbilityProfile2 = {
		{EAbilityEnum::Ability1, nullptr},
		{EAbilityEnum::Ability2, nullptr},
		{EAbilityEnum::Ability3, nullptr},
		{EAbilityEnum::Ability4, nullptr},
		{EAbilityEnum::ShiftAbility, nullptr},
		{EAbilityEnum::JumpAbility, nullptr}
	};

	UPROPERTY(EditAnywhere, Category="Ability")
	TMap<EAbilityEnum, TSubclassOf<UBaseHeroAbility>> AbilityProfile3 = {
		{EAbilityEnum::Ability1, nullptr},
		{EAbilityEnum::Ability2, nullptr},
		{EAbilityEnum::Ability3, nullptr},
		{EAbilityEnum::Ability4, nullptr},
		{EAbilityEnum::ShiftAbility, nullptr},
		{EAbilityEnum::JumpAbility, nullptr}
	};

	UPROPERTY(EditAnywhere, Category="Ability")
	TMap<EAbilityEnum, TSubclassOf<UBaseHeroAbility>> AbilityProfile4 = {
		{EAbilityEnum::Ability1, nullptr},
		{EAbilityEnum::Ability2, nullptr},
		{EAbilityEnum::Ability3, nullptr},
		{EAbilityEnum::Ability4, nullptr},
		{EAbilityEnum::ShiftAbility, nullptr},
		{EAbilityEnum::JumpAbility, nullptr}
	};

	UPROPERTY(EditAnywhere, Category="Ability")
	TMap<EAbilityEnum, TSubclassOf<UBaseHeroAbility>> AbilityProfile5 = {
		{EAbilityEnum::Ability1, nullptr},
		{EAbilityEnum::Ability2, nullptr},
		{EAbilityEnum::Ability3, nullptr},
		{EAbilityEnum::Ability4, nullptr},
		{EAbilityEnum::ShiftAbility, nullptr},
		{EAbilityEnum::JumpAbility, nullptr}
	};
	
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
	void GoUpInput(const FInputActionValue& Value);
	void GoDownInput(const FInputActionValue& Value);
	void ReloadInput(const FInputActionValue& Value);
	void UltimateInput(const FInputActionValue& Value);
	void QuickMeleeAttackInput(const FInputActionValue& Value);
	void ShiftAbilityPressInput(const FInputActionValue& Value);
	void ShiftAbilityHoldInput(const FInputActionValue& Value);
	void JumpAbilityInput(const FInputActionValue& Value);
	void Ability1PressInput(const FInputActionValue& Value);
	void Ability1HoldInput(const FInputActionValue& Value);
	void Ability2PressInput(const FInputActionValue& Value);
	void Ability2HoldInput(const FInputActionValue& Value);
	void Ability3PressInput(const FInputActionValue& Value);
	void Ability3HoldInput(const FInputActionValue& Value);
	void Ability4PressInput(const FInputActionValue& Value);
	void Ability4HoldInput(const FInputActionValue& Value);
	
	UPROPERTY()
	UPlayMontageCallbackProxy* ProxyReloadPlayMontage;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* ReloadMontage;
	
	void Death();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseSpectatorPawn> ShooterSpectatorPawnClass;
	
};


