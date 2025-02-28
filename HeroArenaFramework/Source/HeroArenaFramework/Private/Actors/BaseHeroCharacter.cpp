﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseHeroCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/BaseHeroAbility.h"
#include "Actors/BaseSpectatorPawn.h"
#include "Actors/RotationViewPointRef.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Controllers/BaseHeroPlayerController.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Stimuli/VisualStimuli/VisualStimuli_ShooterCharacter.h"
#include "Utility/NavMeshUtility.h"

// Sets default values
ABaseHeroCharacter::ABaseHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Create and setup head collision sphere
	HeadCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollision"));
	HeadCollision->SetupAttachment(GetMesh());
	HeadCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	HeadCollision->SetGenerateOverlapEvents(true);
	
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->NavAgentProps.bCanWalk = true;
	MovementComp->NavAgentProps.bCanJump = true;
	MovementComp->NavAgentProps.bCanFly = true;
	MovementComp->NavAgentProps.bCanSwim = false;
	MovementComp->NavAgentProps.bCanCrouch = false;

	MovementComp->GravityScale = 2.0f;
	MovementComp->JumpZVelocity = 840.0f;
}

FVector ABaseHeroCharacter::GetHeadAnchorLocation() const
{
	if (USkeletalMeshComponent* LocalMesh = GetMesh())
	{
		FTransform BoneTransform = LocalMesh->GetSocketTransform(HeadBoneName);
		return BoneTransform.TransformPosition(HeadAnchorOffset);
	}
	
	return GetActorLocation();
}

bool ABaseHeroCharacter::IsDead() const
{
	return Dead;
}

bool ABaseHeroCharacter::GetIsReloading() const
{
	return IsReloading;
}

float ABaseHeroCharacter::GetHealth() const
{
	return Health;
}

float ABaseHeroCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void ABaseHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;

	//Visual stimuli
	VSShooterCharacter = GetWorld()->SpawnActor<AVisualStimuli_ShooterCharacter>(
		VisualStimuli_ShooterCharacterClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator
	);

	VSShooterCharacter->SetShooterCharacterRef(this);
	VSShooterCharacter->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);

	//Navmesh Utility
	NavMeshUtility = NewObject<UNavMeshUtility>(GetTransientPackage(), UNavMeshUtility::StaticClass());

	//RotationViewPointRef 
	RotationViewPointRef = GetWorld()->SpawnActor<ARotationViewPointRef>(
		RotationViewPointRefClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator
	);

	RotationViewPointRef->SetOwnerController(GetController());
	RotationViewPointRef->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);

	// Set initial collision sphere size
	HeadCollision->SetSphereRadius(HeadshotRadius - 5.0f);
	UpdateHeadCollision();

	// Create All Ability Profiles
	TArray<TMap<EAbilityEnum, TSubclassOf<UBaseHeroAbility>>> AbilityProfiles;
	AbilityProfiles.Add(AbilityProfile1);
	AbilityProfiles.Add(AbilityProfile2);
	AbilityProfiles.Add(AbilityProfile3);
	AbilityProfiles.Add(AbilityProfile4);
	AbilityProfiles.Add(AbilityProfile5);

	AllAbilityProfiles.Empty();

	for (const TMap<EAbilityEnum, TSubclassOf<UBaseHeroAbility>>& Profile : AbilityProfiles)
	{
		TMap<EAbilityEnum, UBaseHeroAbility*> InstantiatedProfile;
		for (const TPair<EAbilityEnum, TSubclassOf<UBaseHeroAbility>>& AbilityPair : Profile)
		{
			if (AbilityPair.Value)
			{
				UBaseHeroAbility* AbilityObject = NewObject<UBaseHeroAbility>(this, AbilityPair.Value);
				if (AbilityObject)
				{
					AddInstanceComponent(AbilityObject);
					AbilityObject->RegisterComponent();

					InstantiatedProfile.Add(AbilityPair.Key, AbilityObject);

					if (AbilityObject->GetMaxAmmo() > 0)
					{
						AllAmmoAbilities.Add(AbilityObject);
					}
				}
			}
		}
		AllAbilityProfiles.Add(InstantiatedProfile);
	}
}

void ABaseHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateHeadCollision();
	
	if (bShowHeadshotDebug)
	{
		DrawDebugSphere(
			GetWorld(),
			GetHeadAnchorLocation(),
			HeadshotRadius,
			24,
			FColor::Green,
			false,
			-1.0f,
			0,
			1.0f
		);
	}

	if (!GetCharacterMovement()->IsFalling())
	{
		CustomJumpCount = 0;
	}
	
}

void ABaseHeroCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	// Bind input function
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction != nullptr) EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::MoveInput);
		if (LookAction != nullptr) EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::LookInput);
		if (LookRateAction != nullptr) EnhancedInputComponent->BindAction(LookRateAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::LookRateInput);
		if (JumpAction != nullptr) EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::JumpInput);
		if (GoUpAction != nullptr) EnhancedInputComponent->BindAction(GoUpAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::GoUpInput);
		if (GoDownAction != nullptr) EnhancedInputComponent->BindAction(GoDownAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::GoDownInput);
		if (ReloadAction != nullptr) EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::ReloadInput);
		if (QuickMeleeAttackAction != nullptr) EnhancedInputComponent->BindAction(QuickMeleeAttackAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::QuickMeleeAttackInput);
		if (UltimateAction != nullptr) EnhancedInputComponent->BindAction(UltimateAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::UltimateInput);
		if (Ability1PressAction != nullptr) EnhancedInputComponent->BindAction(Ability1PressAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::Ability1PressInput);
		if (Ability1HoldAction != nullptr) EnhancedInputComponent->BindAction(Ability1HoldAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::Ability1HoldInput);
		if (Ability2PressAction != nullptr) EnhancedInputComponent->BindAction(Ability2PressAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::Ability2PressInput);
		if (Ability2HoldAction != nullptr) EnhancedInputComponent->BindAction(Ability2HoldAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::Ability2HoldInput);
		if (Ability3PressAction != nullptr) EnhancedInputComponent->BindAction(Ability3PressAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::Ability3PressInput);
		if (Ability3HoldAction != nullptr) EnhancedInputComponent->BindAction(Ability3HoldAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::Ability3HoldInput);
		if (Ability4PressAction != nullptr) EnhancedInputComponent->BindAction(Ability4PressAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::Ability4PressInput);
		if (Ability4HoldAction != nullptr) EnhancedInputComponent->BindAction(Ability4HoldAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::Ability4HoldInput);
		if (ShiftAbilityPressAction != nullptr) EnhancedInputComponent->BindAction(ShiftAbilityPressAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::ShiftAbilityPressInput);
		if (ShiftAbilityHoldAction != nullptr) EnhancedInputComponent->BindAction(ShiftAbilityHoldAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::ShiftAbilityHoldInput);
		if (JumpAbilityAction != nullptr) EnhancedInputComponent->BindAction(JumpAbilityAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::JumpAbilityInput);
	}
}

float ABaseHeroCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
        
		// Get current head location and check distance
		float DistanceToHead = FVector::Distance(GetHeadAnchorLocation(), ImpactPoint);
        
		if (DistanceToHead <= HeadshotRadius)
		{
			DamageAmount *= HeadshotMultiplier;

			if (bShowHeadshotDebug && GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
					FString::Printf(TEXT("Headshot! Distance: %f, Damage: %f"), 
					DistanceToHead, DamageAmount));

				UE_LOG(LogTemp, Warning, TEXT("Headshot! Distance: %f, Damage: %f"), DistanceToHead, DamageAmount);
				
				DrawDebugSphere(GetWorld(), ImpactPoint, 5.0f, 12, FColor::Red, 
					false, 2.0f, 0, 1.0f);
                
				DrawDebugLine(GetWorld(), GetHeadAnchorLocation(), ImpactPoint, 
					FColor::Yellow, false, 2.0f, 0, 1.0f);
			}
		}
	}
	
	float DamageToApply =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageToApply = FMath::Min(Health, DamageToApply);
	if (bShowHeadshotDebug && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
		FString::Printf(TEXT("Health before damage applied:  %f"), 
		Health));
		UE_LOG(LogTemp, Warning, TEXT("Health before damage applied: %f"), Health);
	}
	
	Health -= DamageToApply;
	
	if (bShowHeadshotDebug && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
		FString::Printf(TEXT("Final Health :  %f"), 
		Health));
		
		UE_LOG(LogTemp, Warning, TEXT("Final Health: %f"), Health);
	}
	
	return DamageToApply;
}

UBaseHeroAbility* ABaseHeroCharacter::GetHeroAbility(int ProfileIndex, EAbilityEnum AbilityEnum)
{
	if (ProfileIndex >= AllAbilityProfiles.Num())
	{
		return nullptr;	
	}
	
	if (!AllAbilityProfiles[ProfileIndex].Contains(AbilityEnum))
	{
		return nullptr;
	}

	return AllAbilityProfiles[ProfileIndex][AbilityEnum];
}

void ABaseHeroCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorRightVector(), MovementVector.X);
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
}

void ABaseHeroCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D RotationVector = Value.Get<FVector2D>();
	AddControllerYawInput(RotationVector.X);
	AddControllerPitchInput(RotationVector.Y);
}

void ABaseHeroCharacter::LookRateInput(const FInputActionValue& Value)
{
	FVector2D RotationVector = Value.Get<FVector2D>();
	AddControllerYawInput(RotationVector.X * RotationRate * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(RotationVector.Y * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ABaseHeroCharacter::JumpInput(const FInputActionValue& Value)
{
	const FString MovementMode = GetCharacterMovement()->GetMovementName();
	if (MovementMode == "Walking")
	{
		CustomJumpCount++;
		ACharacter::Jump();
	}
}

void ABaseHeroCharacter::GoUpInput(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->GetMovementName() == "Flying")
	{
	}
}

void ABaseHeroCharacter::GoDownInput(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->GetMovementName() == "Flying")
	{
	}
}

void ABaseHeroCharacter::UltimateInput(const FInputActionValue& Value)
{
}

void ABaseHeroCharacter::QuickMeleeAttackInput(const FInputActionValue& Value)
{
}

void ABaseHeroCharacter::ReloadInput(const FInputActionValue& Value)
{
	Reload();
}

void ABaseHeroCharacter::Reload()
{
	// Don't allow reload if there is no ability who uses ammo
	if (AllAmmoAbilities.IsEmpty())
	{
		return;
	}
}

void ABaseHeroCharacter::OnReloadAnimationCompleted(FName NotifyName)
{
	IsReloading = false;
	for (int i=0; i<AllAmmoAbilities.Num(); i++)
	{
		AllAmmoAbilities[i]->ReloadAmmo();
	}
}

void ABaseHeroCharacter::OnReloadAnimationInterrupted(FName NotifyName)
{
 	IsReloading = false;
}

float ABaseHeroCharacter::GetLowestAmmoPercent()
{
	float lowestPercent = 1.0f;
	for (int32 i=0; i<AllAmmoAbilities.Num(); i++)
	{
		if (AllAmmoAbilities[i]->GetAmmoPercent() < lowestPercent)
		{
			lowestPercent = AllAmmoAbilities[i]->GetAmmoPercent();
		}
	}

	return lowestPercent;
}

// Helper function for press-type abilities.
void ABaseHeroCharacter::TriggerAbilityPress(EAbilityEnum AbilityKey, const FInputActionValue& Value)
{
	if (AllAbilityProfiles.IsValidIndex(CurrentAbilityProfileIndex))
	{
		UBaseHeroAbility* Ability = AllAbilityProfiles[CurrentAbilityProfileIndex].FindRef(AbilityKey);
		if (Ability)
		{
			Ability->CallStartAbility();
		}
	}
}

// Helper function for hold-type abilities.
void ABaseHeroCharacter::TriggerAbilityHold(EAbilityEnum AbilityKey, const FInputActionValue& Value)
{
	if (AllAbilityProfiles.IsValidIndex(CurrentAbilityProfileIndex))
	{
		UBaseHeroAbility* Ability = AllAbilityProfiles[CurrentAbilityProfileIndex].FindRef(AbilityKey);
		if (Ability)
		{
			// For hold inputs, the value indicates if it is pressed (true) or released (false).
			bool bPressed = Value.Get<bool>();
			if (bPressed)
			{
				Ability->CallStartAbility();
			}
			else
			{
				Ability->CallEndAbility();
			}
		}
	}
}

// --- Ability 1 ---
void ABaseHeroCharacter::Ability1PressInput(const FInputActionValue& Value)
{
	TriggerAbilityPress(EAbilityEnum::Ability1Press, Value);
}

void ABaseHeroCharacter::Ability1HoldInput(const FInputActionValue& Value)
{
	TriggerAbilityHold(EAbilityEnum::Ability1Hold, Value);
}

void ABaseHeroCharacter::Ability2PressInput(const FInputActionValue& Value)
{
	TriggerAbilityPress(EAbilityEnum::Ability2Press, Value);
}

void ABaseHeroCharacter::Ability2HoldInput(const FInputActionValue& Value)
{
	TriggerAbilityHold(EAbilityEnum::Ability2Hold, Value);
}

void ABaseHeroCharacter::Ability3PressInput(const FInputActionValue& Value)
{
	TriggerAbilityPress(EAbilityEnum::Ability3Press, Value);
}

void ABaseHeroCharacter::Ability3HoldInput(const FInputActionValue& Value)
{
	TriggerAbilityHold(EAbilityEnum::Ability3Hold, Value);
}

void ABaseHeroCharacter::Ability4PressInput(const FInputActionValue& Value)
{
	TriggerAbilityPress(EAbilityEnum::Ability4Press, Value);
}

void ABaseHeroCharacter::Ability4HoldInput(const FInputActionValue& Value)
{
	TriggerAbilityHold(EAbilityEnum::Ability4Hold, Value);
}

void ABaseHeroCharacter::ShiftAbilityPressInput(const FInputActionValue& Value)
{
	TriggerAbilityPress(EAbilityEnum::ShiftAbilityPress, Value);
}

void ABaseHeroCharacter::ShiftAbilityHoldInput(const FInputActionValue& Value)
{
	TriggerAbilityHold(EAbilityEnum::ShiftAbilityHold, Value);
}

void ABaseHeroCharacter::JumpAbilityInput(const FInputActionValue& Value)
{
	// Only allow jump ability when in air.
	const FString MovementMode = GetCharacterMovement()->GetMovementName();
	if (MovementMode == "Falling")
	{
		TriggerAbilityPress(EAbilityEnum::JumpAbility, Value);
	}
}

void ABaseHeroCharacter::Death()
{
	if (!IsDead())
	{
		//TODO: will have to cancel every ability, this is a remnant of the shooter framework
		Ability1HoldInput(FInputActionValue(false));
		
		GetCharacterMovement()->GravityScale = 0.0f; //FOR MULTIPLAYER client for some reason pass through floor when no collision
		GetCharacterMovement()->Velocity = FVector::Zero();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		VSShooterCharacter->Destroy();
		RotationViewPointRef->Destroy();

		Dead = true;
		
		if (HasAuthority())// FOR MULTIPLAYER
		{
			//Become spectator - Only Server has authority to spawn actor and assign control
			ABaseSpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ABaseSpectatorPawn>(
				ShooterSpectatorPawnClass,
				GetActorLocation(),
				GetActorRotation()
			);
			SpectatorPawn->SetTeam(GetTeam());

			ABaseHeroPlayerController* ShooterPlayerController = Cast<ABaseHeroPlayerController>(GetController());
			DetachFromControllerPendingDestroy();
			if (ShooterPlayerController != nullptr)
			{
				SpectatorPawn->SetOwner(ShooterPlayerController);
				ShooterPlayerController->Possess(SpectatorPawn);
			}
		}
		
		OnDeadEvent.Broadcast(this);
	}
}

AVisualStimuli_ShooterCharacter* ABaseHeroCharacter::GetVSShooterCharacter()
{
	return VSShooterCharacter;
}

ARotationViewPointRef* ABaseHeroCharacter::GetRotationViewPointRef()
{
	return RotationViewPointRef;
}

#if WITH_EDITOR
void ABaseHeroCharacter::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateEditorAnchorPositionVisualisation();
}

void ABaseHeroCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GenerateEditorAnchorPositionVisualisation();
}

void ABaseHeroCharacter::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	GenerateEditorAnchorPositionVisualisation();
}

void ABaseHeroCharacter::GenerateEditorAnchorPositionVisualisation() const
{
	if (const UWorld* World = GetWorld())
	{
		if (World->WorldType == EWorldType::EditorPreview)
		{
			UKismetSystemLibrary::FlushPersistentDebugLines(this);
			const FVector ActorLocation = GetActorLocation();
			
			// HealtBar Anchor
			DrawDebugSphere(GetWorld(), ActorLocation + HealthBarAnchor, 5.0f, 12, FColor::Cyan, true, 0.0f, 0, 0.0f);
			DrawDebugSphere(GetWorld(), ActorLocation + FootPositionAnchor, 5.0f, 12, FColor::Purple, true, 0.0f, 0, 0.0f);
			DrawDebugSphere(GetWorld(), ActorLocation + BodyPositionAnchor, 5.0f, 12, FColor::Blue, true, 0.0f, 0, 0.0f);
		}
	}
}
#endif

void ABaseHeroCharacter::UpdateHeadCollision()
{
	// Update collision sphere location to match anchor point
	FVector NewLocation = GetHeadAnchorLocation();
	HeadCollision->SetWorldLocation(NewLocation);	
}

ETeam ABaseHeroCharacter::GetTeam() const
{
	return Team;
}

float ABaseHeroCharacter::GetHealthPercent() const
{
	return Health/MaxHealth;
}

float ABaseHeroCharacter::Heal(float HealAmount)
{
	if (Health+HealAmount <= MaxHealth)
	{
		Health += HealAmount;
		OnHealEvent.Broadcast();
		return HealAmount;
	}
	else
	{
		Health = MaxHealth;
		OnHealEvent.Broadcast();
		return (Health+HealAmount) - MaxHealth;
	}	
}

void ABaseHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseHeroCharacter, Health);
}



