// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Multiplayer/MP_ShooterPlayerState.h"

#include "Net/UnrealNetwork.h"

AMP_ShooterPlayerState::AMP_ShooterPlayerState()
{
	bReplicates = true;
}

void AMP_ShooterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMP_ShooterPlayerState, Team);
}
