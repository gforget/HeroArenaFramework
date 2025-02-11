// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stimuli.h"
#include "SoundStimuli.generated.h"
class USphereComponent;

UCLASS()
class HEROARENAFRAMEWORK_API ASoundStimuli : public AStimuli
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundStimuli();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSoundOwner(AActor* OwnerRef);
	AActor* GetSoundOwner();

	UPROPERTY(EditDefaultsOnly)
	int PriorityLevel = 0;
	
private:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;

	UPROPERTY()
	AActor* SoundOwner;
};
