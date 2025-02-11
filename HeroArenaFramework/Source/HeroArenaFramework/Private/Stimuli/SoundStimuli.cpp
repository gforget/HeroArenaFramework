// Fill out your copyright notice in the Description page of Project Settings.


#include "Stimuli/SoundStimuli.h"
#include "Components/SphereComponent.h"


// Sets default values
ASoundStimuli::ASoundStimuli()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sound Range Component"));
	SphereComponent->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ASoundStimuli::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASoundStimuli::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoundStimuli::SetSoundOwner(AActor* OwnerRef)
{
	SoundOwner = OwnerRef;
}

AActor* ASoundStimuli::GetSoundOwner()
{
	return SoundOwner;
}

