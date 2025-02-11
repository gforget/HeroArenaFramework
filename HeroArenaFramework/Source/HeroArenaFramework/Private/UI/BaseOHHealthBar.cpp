// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseOHHealthBar.h"

#include "Actors/BaseShooterCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "GameFramework/SpectatorPawn.h"

bool UBaseOHHealthBar::Initialize()
{
	return Super::Initialize();
}

void UBaseOHHealthBar::InitializeAssignedCharacterAndPlayerController(ABaseShooterCharacter* AssignedCharacterRef)
{
	AssignedCharacter = AssignedCharacterRef;
	AssignedCharacter->OnDeadEvent.AddDynamic(this, &UBaseOHHealthBar::OnCharacterDeath);
}

void UBaseOHHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (AssignedCharacter != nullptr)
	{
		const float ViewPortScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	
		if (AssignedCharacter->GetMesh()->WasRecentlyRendered())
		{
			FHitResult Hit;
			
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(AssignedCharacter);
			
			if (ASpectatorPawn* SpectatorPawn = Cast<ASpectatorPawn>(GetOwningPlayer()->GetPawn()))
			{
				Params.AddIgnoredActor(SpectatorPawn);
			}
			
			FVector ViewPortLocation;
			FRotator ViewPortRotation;
			GetOwningPlayer()->GetPlayerViewPoint(ViewPortLocation, ViewPortRotation);

			//Confirm there is no obstacle between the camera of the player and the 3D position of the healthbar anchor.
			//The reason is that WasRecentlyRendered is less costly than a raycast, but is imperfect
			if (!GetWorld()->LineTraceSingleByChannel(Hit, AssignedCharacter->HealthBarAnchor + AssignedCharacter->GetActorLocation(), ViewPortLocation, ECollisionChannel::ECC_GameTraceChannel1, Params))
			{
				FVector2d ScreenLocation;
				const FVector UnitLocation = AssignedCharacter->GetActorLocation() + AssignedCharacter->HealthBarAnchor;
	
				GetOwningPlayer()->ProjectWorldLocationToScreen(UnitLocation, ScreenLocation, true);

				//Prevent offsetting caused by the UMG system
				ScreenLocation = ScreenLocation/ViewPortScale;

				Cast<UCanvasPanelSlot>(OHHealthBarOverlay->Slot)->SetPosition(ScreenLocation);
				OHHealthBarOverlay->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				OHHealthBarOverlay->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			OHHealthBarOverlay->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UBaseOHHealthBar::OnCharacterDeath(ABaseShooterCharacter* DeadCharacter)
{
	RemoveFromParent();
}


