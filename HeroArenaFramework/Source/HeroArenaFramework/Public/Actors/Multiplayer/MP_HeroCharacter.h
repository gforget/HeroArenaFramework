// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseHeroCharacter.h"
#include "MP_HeroCharacter.generated.h"


UCLASS()
class HEROARENAFRAMEWORK_API AMP_HeroCharacter : public ABaseHeroCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMP_HeroCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:

	// Look Up functionality
	void LookUp(float AxisValue);

	virtual void PullTrigger() override;
	virtual void ReleaseTrigger() override;
	virtual void Reload() override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintPure)
	FRotator GetReplicatedControlRotation() const { return ReplicatedControlRotation; }

	UPROPERTY(Replicated)
	FVector ShooterViewPointLocation;
	
	UPROPERTY(Replicated)
	FRotator ShooterViewPointRotation;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	
	//Pull Trigger RPC
	// Server RPC - executed on the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPullTrigger();

	// Multicast RPC - executed on all clients
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPullTrigger();

	// Function to perform the actual shooting logic
	void PerformPullTrigger();

	//Release Trigger RPC
	// Server RPC - executed on the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReleaseTrigger();

	// Multicast RPC - executed on all clients
	UFUNCTION(NetMulticast, Reliable)
	void MulticastReleaseTrigger();

	// Function to perform the actual shooting logic
	void PerformReleaseTrigger();

	//Reload Trigger RPC
	// Server RPC - executed on the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	// Multicast RPC - executed on all clients
	UFUNCTION(NetMulticast, Reliable)
	void MulticastReload();

	// Function to perform the actual Reload logic
	void PerformReload();

	// Death RPC
	// Multicast RPC - executed on all clients
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDeath();
	
	//ControlRotation Replication
	//Replicated Variable
	UPROPERTY(ReplicatedUsing = OnRep_ControlRotation)
	FRotator ReplicatedControlRotation;

	UFUNCTION()
	void OnRep_ControlRotation();
	
	// Server RPC - executed on the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetControlRotation(const FRotator& NewRotation);
	
	// Function to directly modify the control rotation
	void UpdateControlRotation(const FRotator& NewRotation);
	
};
