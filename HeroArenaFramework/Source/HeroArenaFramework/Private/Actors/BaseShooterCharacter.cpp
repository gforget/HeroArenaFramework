// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseShooterCharacter.h"

#include "Actors/BaseGun.h"
#include "Actors/BaseShooterSpectatorPawn.h"
#include "Actors/RotationViewPointRef.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Controllers/BaseShooterPlayerController.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Stimuli/VisualStimuli/VisualStimuli_ShooterCharacter.h"
#include "Utility/NavMeshUtility.h"

// Sets default values
ABaseShooterCharacter::ABaseShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Create and setup head collision sphere
	HeadCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollision"));
	HeadCollision->SetupAttachment(GetMesh());
	HeadCollision->SetCollisionProfileName(TEXT("OverlapAll")); // Adjust profile as needed
	HeadCollision->SetGenerateOverlapEvents(true);
}

FVector ABaseShooterCharacter::GetHeadAnchorLocation() const
{
	if (USkeletalMeshComponent* LocalMesh = GetMesh())
	{
		FTransform BoneTransform = LocalMesh->GetSocketTransform(HeadBoneName);
		return BoneTransform.TransformPosition(HeadAnchorOffset);
	}
	
	return GetActorLocation();
}

bool ABaseShooterCharacter::IsDead() const
{
	return Dead;
}

bool ABaseShooterCharacter::GetIsReloading() const
{
	return IsReloading;
}

float ABaseShooterCharacter::GetAmmoTotalPercent() const
{
	int AmmoTotal = AmmoReserve + GetGunReference()->GetAmmoAmount();
	int AmmoMaxTotal = MaxAmmoReserve + GetGunReference()->GetMaxAmmo();
	
	return static_cast<float>(AmmoTotal)/static_cast<float>(AmmoMaxTotal);
}

FString ABaseShooterCharacter::GetAmmoReserveRatio() const
{
	return FString::FromInt(AmmoReserve) + "/" + FString::FromInt(MaxAmmoReserve); 
}

float ABaseShooterCharacter::GetHealth() const
{
	return Health;
}

float ABaseShooterCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void ABaseShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<ABaseGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);
	
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);

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

void ABaseShooterCharacter::Tick(float DeltaTime)
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

void ABaseShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ABaseShooterCharacter::LookUpRate);
	
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABaseShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &ABaseShooterCharacter::LookRightRate);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump); // already define in character class
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &ABaseShooterCharacter::PullTrigger);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &ABaseShooterCharacter::ReleaseTrigger);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ABaseShooterCharacter::Reload);
}

float ABaseShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
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

void ABaseShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector()*AxisValue);
}

void ABaseShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector()*AxisValue);
}

void ABaseShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ABaseShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ABaseShooterCharacter::PullTrigger()
{
}

void ABaseShooterCharacter::ReleaseTrigger()
{
}

void ABaseShooterCharacter::Reload()
{
}

void ABaseShooterCharacter::OnReloadAnimationCompleted(FName NotifyName)
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

void ABaseShooterCharacter::Death()
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
			ABaseShooterSpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ABaseShooterSpectatorPawn>(
				ShooterSpectatorPawnClass,
				GetActorLocation(),
				GetActorRotation()
			);
			SpectatorPawn->SetTeam(GetTeam());

			ABaseShooterPlayerController* ShooterPlayerController = Cast<ABaseShooterPlayerController>(GetController());
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

AVisualStimuli_ShooterCharacter* ABaseShooterCharacter::GetVSShooterCharacter()
{
	return VSShooterCharacter;
}

ARotationViewPointRef* ABaseShooterCharacter::GetRotationViewPointRef()
{
	return RotationViewPointRef;
}

#if WITH_EDITOR
void ABaseShooterCharacter::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateEditorAnchorPositionVisualisation();
}

void ABaseShooterCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GenerateEditorAnchorPositionVisualisation();
}

void ABaseShooterCharacter::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	GenerateEditorAnchorPositionVisualisation();
}

void ABaseShooterCharacter::GenerateEditorAnchorPositionVisualisation() const
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

void ABaseShooterCharacter::UpdateHeadCollision()
{
	// Update collision sphere location to match anchor point
	FVector NewLocation = GetHeadAnchorLocation();
	HeadCollision->SetWorldLocation(NewLocation);	
}

float ABaseShooterCharacter::GetAmmoReservePercent() const
{
	return static_cast<float>(AmmoReserve)/static_cast<float>(MaxAmmoReserve);
}

int ABaseShooterCharacter::AddAmmoReserve(int AmmoAmount)
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

ETeam ABaseShooterCharacter::GetTeam() const
{
	return Team;
}

float ABaseShooterCharacter::GetHealthPercent() const
{
	return Health/MaxHealth;
}

float ABaseShooterCharacter::Heal(float HealAmount)
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

ABaseGun* ABaseShooterCharacter::GetGunReference() const
{
	return Gun;
}

void ABaseShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseShooterCharacter, Health);
}



