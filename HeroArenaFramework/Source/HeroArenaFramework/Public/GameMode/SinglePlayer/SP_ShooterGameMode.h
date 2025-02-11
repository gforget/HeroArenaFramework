// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/TeamManager.h"
#include "GameFramework/GameMode.h"
#include "SP_ShooterGameMode.generated.h"

/**
 * 
 */

class ABaseAmmoPack;
class ABaseHealthPack;
class ABaseShooterCharacter;
class UTeamManager;
class ABaseWaypoint;
class ABaseSpawningPoint;

UCLASS()
class HEROARENAFRAMEWORK_API ASP_ShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASP_ShooterGameMode();

	UFUNCTION()
	virtual void OnShooterCharacterDeath(ABaseShooterCharacter* DeadShooterCharacter);

	virtual void AddShooterCharacterCount(ABaseShooterCharacter* ShooterCharacterToRegister);

	UPROPERTY(EditDefaultsOnly)
	bool bBasicOHHealthBarAssociation = false;
	
	TArray<ABaseWaypoint*> GetAllWayPoints();
	void AddWayPoint(ABaseWaypoint* WayPoint);
	
	TArray<ABaseAmmoPack*> GetAllAmmoPacks();
	void AddAmmoPack(ABaseAmmoPack* AmmoPack);

	TArray<ABaseHealthPack*> GetAllHealthPacks();
	void AddHealthPack(ABaseHealthPack* HealthPack);

	void RegisterEvent (ABaseShooterCharacter* ShooterCharacterRef);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Global Object")
	UTeamManager* FactionManagerComponent;

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY()
	TArray<ABaseSpawningPoint*> AllSpawnPoints;

	UPROPERTY()
	TArray<ABaseSpawningPoint*> AllRedSpawnPoints;

	UPROPERTY()
	TArray<ABaseSpawningPoint*> AllBlueSpawnPoints;
	
	UPROPERTY()
	TArray<ABaseWaypoint*> AllWayPoints;
	
	UPROPERTY()
	TArray<ABaseAmmoPack*> AllAmmoPacks;
	
	UPROPERTY()
	TArray<ABaseHealthPack*> AllHealthPacks;
};
