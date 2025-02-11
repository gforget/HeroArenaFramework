// Fill out your copyright notice in the Description page of Project Settings.

#include "Stimuli/VisualStimuli/VisualStimuli_ShooterCharacter.h"
#include "Actors/SinglePlayer/SP_ShooterCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AVisualStimuli_ShooterCharacter::AVisualStimuli_ShooterCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AVisualStimuli_ShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVisualStimuli_ShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ABaseShooterCharacter* AVisualStimuli_ShooterCharacter::GetShooterCharacterRef()
{
	return ShooterCharacterRef;
}

void AVisualStimuli_ShooterCharacter::SetShooterCharacterRef(ABaseShooterCharacter* ShooterCharacterRefValue)
{
	ShooterCharacterRef = ShooterCharacterRefValue;
	
	CapsuleComponent->SetCapsuleHalfHeight(ShooterCharacterRef->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	CapsuleComponent->SetCapsuleRadius(ShooterCharacterRef->GetCapsuleComponent()->GetUnscaledCapsuleRadius());
}

