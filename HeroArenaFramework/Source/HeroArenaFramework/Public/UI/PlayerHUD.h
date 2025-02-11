// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Main Events")
	void OnSpectatorModeEvent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Main Events")
	void OnPlayerModeEvent();
};
