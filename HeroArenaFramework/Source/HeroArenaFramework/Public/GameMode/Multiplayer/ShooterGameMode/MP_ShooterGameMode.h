// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/Multiplayer/MP_GeneralGameMode.h"
#include "ActorComponents/TeamManager.h"
#include "MP_ShooterGameMode.generated.h"

class ABaseSpawningPoint;
class ABaseHealthPack;
class ABaseAmmoPack;
class ABaseWaypoint;
class UTeamManager;
class ABaseShooterCharacter;
class AMP_ShooterAIController;

UCLASS()
class HEROARENAFRAMEWORK_API AMP_ShooterGameMode : public AMP_GeneralGameMode
{
	GENERATED_BODY()

public:
	AMP_ShooterGameMode();

	UFUNCTION()
	virtual void OnShooterCharacterDeath(ABaseShooterCharacter* DeadShooterCharacter);
	
	TArray<ABaseWaypoint*> GetAllWayPoints();
	void AddWayPoint(ABaseWaypoint* WayPoint);
	
	TArray<ABaseAmmoPack*> GetAllAmmoPacks();
	void AddAmmoPack(ABaseAmmoPack* AmmoPack);

	TArray<ABaseHealthPack*> GetAllHealthPacks();
	void AddHealthPack(ABaseHealthPack* HealthPack);

	void RegisterEvent (ABaseShooterCharacter* ShooterCharacterRef);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Global Object")
	UTeamManager* FactionManagerComponent;

	void CallLeaveSession();
	
protected:

	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	
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

	bool SpawningPointsHaveBeenFilled = false;
	void FillSpawningPoints();

	//Need to create an MP version of AIController
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMP_ShooterAIController> ShooterAIController;
	
private:
	FTimerHandle RestartTimer;
	
	UPROPERTY(EditDefaultsOnly)
	float RestartDelay = 5.0f;
	
	void LeaveSession();


};
