// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/TeamManager.h"
#include "GameFramework/Actor.h"
#include "BaseSpawningPoint.generated.h"

class ABaseHeroCharacter;

UCLASS()
class HEROARENAFRAMEWORK_API ABaseSpawningPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseSpawningPoint();

	UPROPERTY(EditInstanceOnly)
	TEnumAsByte<ETeam> Team = ETeam::BlueTeam; 

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* BlueTeamIconTexture;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseHeroCharacter> BlueTeamShooterCharacterClass;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* RedTeamIconTexture;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseHeroCharacter> RedTeamShooterCharacterClass;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* NoTeamIconTexture;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseHeroCharacter> NoTeamShooterCharacterClass;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostActorCreated() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void ChangeBillboardTexture();
	
	UPROPERTY()
	UBillboardComponent* Billboard; 
};
