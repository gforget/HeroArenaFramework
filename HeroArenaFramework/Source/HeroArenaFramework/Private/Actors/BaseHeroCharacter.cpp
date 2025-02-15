// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseHeroCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/BaseGun.h"
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
	HeadCollision->SetCollisionProfileName(TEXT("OverlapAll")); // Adjust profile as needed
	HeadCollision->SetGenerateOverlapEvents(true);
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

float ABaseHeroCharacter::GetAmmoTotalPercent() const
{
	int AmmoTotal = AmmoReserve + GetGunReference()->GetAmmoAmount();
	int AmmoMaxTotal = MaxAmmoReserve + GetGunReference()->GetMaxAmmo();
	
	return static_cast<float>(AmmoTotal)/static_cast<float>(AmmoMaxTotal);
}

FString ABaseHeroCharacter::GetAmmoReserveRatio() const
{
	return FString::FromInt(AmmoReserve) + "/" + FString::FromInt(MaxAmmoReserve); 
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

	Gun = GetWorld()->SpawnActor<ABaseGun>(GunClass);
	// GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);
	//
	// Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	// Gun->SetOwner(this);

	VSShooterCharacter = GetWorld()->SpawnActor<AVisualStimuli_ShooterCharacter>(
		VisualStimuli_ShooterCharacterClass,
		FVector(0.0f, 0.0f, 0.0f),
		FRotator(0.0f, 0.0f, 0.0f)
	);

	VSShooterCharacter->SetShooterCharacterRef(this);
	VSShooterCharacter->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	
	NavMeshUtility = NewObject<UNavMeshUtility>(GetTransientPackage(), UNavMeshUtility::StaticClass());

	RotationViewPointRef = GetWorld()->SpawnActor<ARotationViewPointRef>(
		RotationViewPointRefClass,
		FVector(0.0f, 0.0f, 0.0f),
		FRotator(0.0f, 0.0f, 0.0f)
	);

	RotationViewPointRef->SetOwnerController(GetController());
	RotationViewPointRef->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);

	// Set initial collision sphere size
	HeadCollision->SetSphereRadius(HeadshotRadius-5.0f);
	UpdateHeadCollision();
}

void ABaseHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update collision sphere position every frame
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
}

void ABaseHeroCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the Player Controller and then the Local Player subsystem for Enhanced Input.
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				// Add the mapping context, at priority 0.
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	
	// Make sure to use UEnhancedInputComponent.
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind the MoveAction to our Move() callback when triggered.
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::MoveInput);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::LookInput);
		EnhancedInputComponent->BindAction(LookRateAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::LookRateInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::JumpInput);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::ShootInput);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ABaseHeroCharacter::ReloadInput);
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

void ABaseHeroCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
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
	ACharacter::Jump();
}

void ABaseHeroCharacter::ShootInput(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		PullTrigger();
	}
	else
	{
		ReleaseTrigger();
	}
}

void ABaseHeroCharacter::ReloadInput(const FInputActionValue& Value)
{
	Reload();
}

void ABaseHeroCharacter::PullTrigger()
{
	
}

void ABaseHeroCharacter::ReleaseTrigger()
{
}

void ABaseHeroCharacter::Reload()
{
}

void ABaseHeroCharacter::OnReloadAnimationCompleted(FName NotifyName)
{
	IsReloading = false;
	int ReloadAmount = Gun->GetMaxAmmo();
	int CurrentReloadAmount = ReloadAmount;
	if (AmmoReserve - ReloadAmount < 0)
	{
		CurrentReloadAmount = ReloadAmount - FMath::Abs(AmmoReserve-ReloadAmount);
	}
	
	AmmoReserve -= CurrentReloadAmount;
	const int LeftOver = Gun->Reload(CurrentReloadAmount);
	AmmoReserve += LeftOver;
}

void ABaseHeroCharacter::Death()
{
	if (!IsDead())
	{
		ReleaseTrigger();
		
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

float ABaseHeroCharacter::GetAmmoReservePercent() const
{
	return static_cast<float>(AmmoReserve)/static_cast<float>(MaxAmmoReserve);
}

int ABaseHeroCharacter::AddAmmoReserve(int AmmoAmount)
{
	if (AmmoReserve+AmmoAmount <= MaxAmmoReserve)
	{
		AmmoReserve += AmmoAmount;
		return AmmoAmount;
	}
	else
	{
		AmmoReserve = MaxAmmoReserve;
		return (AmmoReserve+AmmoAmount) - MaxAmmoReserve;
	}
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

ABaseGun* ABaseHeroCharacter::GetGunReference() const
{
	return Gun;
}

void ABaseHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseHeroCharacter, Health);
}



