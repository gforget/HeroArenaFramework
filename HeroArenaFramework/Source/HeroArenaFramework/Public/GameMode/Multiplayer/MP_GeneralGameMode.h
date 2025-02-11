// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MP_GeneralGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API AMP_GeneralGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMP_GeneralGameMode();
	
	UFUNCTION(BlueprintCallable)
	void StartMultiplayerGame(FString LevelName);
};
