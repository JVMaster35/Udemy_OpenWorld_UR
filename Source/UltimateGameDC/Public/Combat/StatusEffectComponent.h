// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusEffectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBurningChanged, bool, bIsBurning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPoisonStacksChanged, int32, NewStacks);

/**
 * Add this to any enemy that should have Defence, and be able to burn and be poisoned.
 * Damage is delivered to the owner through the normal UE damage pipeline (TakeDamage /
 * OnTakeAnyDamage), so the enemy's own health code stays in charge of health.
 */
UCLASS(ClassGroup = (Combat), meta = (BlueprintSpawnableComponent))
class ULTIMATEGAMEDC_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectComponent();

	// Fraction of direct hit damage blocked: 0 = none, 0.5 = half, 1 = immune
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status | Defence", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Defence = 0.25f;

	// Seconds between damage-over-time ticks (burn and poison)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status", meta = (ClampMin = "0.05"))
	float TickInterval = 0.5f;

	// Deals a direct hit, reduced by Defence unless bIgnoreDefence. Returns the damage actually applied.
	UFUNCTION(BlueprintCallable, Category = "Status")
	float ApplyHitDamage(float RawDamage, bool bIgnoreDefence, AController* InstigatorController, AActor* DamageCauser);

	// Sets the target on fire. Re-applying refreshes the duration and keeps the stronger burn.
	UFUNCTION(BlueprintCallable, Category = "Status | Burn")
	void ApplyBurn(float DamagePerTick, float Duration, AController* InstigatorController, AActor* DamageCauser);

	// Adds one poison stack (up to MaxStacks) and refreshes the duration. Damage per tick = stacks * DamagePerStackPerTick.
	UFUNCTION(BlueprintCallable, Category = "Status | Poison")
	void AddPoisonStack(float DamagePerStackPerTick, int32 MaxStacks, float Duration, AController* InstigatorController, AActor* DamageCauser);

	UFUNCTION(BlueprintPure, Category = "Status | Burn")
	bool IsBurning() const { return BurnTimeRemaining > 0.f; }

	UFUNCTION(BlueprintPure, Category = "Status | Poison")
	int32 GetPoisonStacks() const { return PoisonStacks; }

	// Hook these up in the enemy Blueprint for fire / poison VFX and UI
	UPROPERTY(BlueprintAssignable, Category = "Status | Burn")
	FOnBurningChanged OnBurningChanged;

	UPROPERTY(BlueprintAssignable, Category = "Status | Poison")
	FOnPoisonStacksChanged OnPoisonStacksChanged;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void BurnTick();
	void PoisonTick();

	// Damage-over-time ignores Defence; change here if you want DoTs reduced too
	void DealDotDamage(float Amount, AController* InstigatorController, AActor* DamageCauser);

	FTimerHandle BurnTimer;
	float BurnDamagePerTick = 0.f;
	float BurnTimeRemaining = 0.f;
	TWeakObjectPtr<AController> BurnInstigator;
	TWeakObjectPtr<AActor> BurnCauser;

	FTimerHandle PoisonTimer;
	int32 PoisonStacks = 0;
	float PoisonDamagePerStack = 0.f;
	float PoisonTimeRemaining = 0.f;
	TWeakObjectPtr<AController> PoisonInstigator;
	TWeakObjectPtr<AActor> PoisonCauser;
};
