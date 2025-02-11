// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/TeamManager.h"
#include "GameFramework/PlayerState.h"
#include "MP_ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API AMP_ShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMP_ShooterPlayerState();
	
	UPROPERTY(BlueprintReadWrite, Replicated)
	TEnumAsByte<ETeam> Team = ETeam::BlueTeam;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
