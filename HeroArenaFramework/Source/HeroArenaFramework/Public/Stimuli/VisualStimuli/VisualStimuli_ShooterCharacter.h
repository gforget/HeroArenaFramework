// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stimuli/VisualStimuli.h"
#include "VisualStimuli_ShooterCharacter.generated.h"

class UCapsuleComponent;
class ABaseHeroCharacter;
UCLASS()
class HEROARENAFRAMEWORK_API AVisualStimuli_ShooterCharacter : public AVisualStimuli
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVisualStimuli_ShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComponent;

	ABaseHeroCharacter* GetShooterCharacterRef();
	void SetShooterCharacterRef(class ABaseHeroCharacter* ShooterCharacterRefValue);
	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY()
	ABaseHeroCharacter* ShooterCharacterRef;
};
