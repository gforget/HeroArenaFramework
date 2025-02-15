// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseOHHealthBar.generated.h"

class ABaseHeroCharacter;
class UOverlay;
class UProgressBar;

/**
 * 
 */
UCLASS()
class HEROARENAFRAMEWORK_API UBaseOHHealthBar : public UUserWidget
{
	GENERATED_BODY()

public :
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UOverlay* OHHealthBarOverlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UProgressBar* OHHealthBarProgressBar;
	
	UPROPERTY(BlueprintReadOnly)
	ABaseHeroCharacter* AssignedCharacter;

	UFUNCTION()
	void OnCharacterDeath(ABaseHeroCharacter* DeadCharacter);

	void InitializeAssignedCharacterAndPlayerController(ABaseHeroCharacter* AssignedCharacterRef);
};
