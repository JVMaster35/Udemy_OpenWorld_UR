// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat/StatusEffectComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"

UStatusEffectComponent::UStatusEffectComponent()
{
	// Timers drive the DoTs, so no per-frame tick is needed
	PrimaryComponentTick.bCanEverTick = false;
}

float UStatusEffectComponent::ApplyHitDamage(float RawDamage, bool bIgnoreDefence, AController* InstigatorController, AActor* DamageCauser)
{
	AActor* Owner = GetOwner();
	if (!Owner || RawDamage <= 0.f)
	{
		return 0.f;
	}

	const float FinalDamage = bIgnoreDefence ? RawDamage : RawDamage * (1.f - FMath::Clamp(Defence, 0.f, 1.f));
	if (FinalDamage > 0.f)
	{
		UGameplayStatics::ApplyDamage(Owner, FinalDamage, InstigatorController, DamageCauser, UDamageType::StaticClass());
	}
	return FinalDamage;
}

// ---------- Burn ----------

void UStatusEffectComponent::ApplyBurn(float DamagePerTick, float Duration, AController* InstigatorController, AActor* DamageCauser)
{
	UWorld* World = GetWorld();
	if (!World || DamagePerTick <= 0.f || Duration <= 0.f)
	{
		return;
	}

	const bool bWasBurning = IsBurning();

	// Refresh: keep whichever is stronger / longer
	BurnDamagePerTick = FMath::Max(BurnDamagePerTick, DamagePerTick);
	BurnTimeRemaining = FMath::Max(BurnTimeRemaining, Duration);
	BurnInstigator = InstigatorController;
	BurnCauser = DamageCauser;

	if (!World->GetTimerManager().IsTimerActive(BurnTimer))
	{
		World->GetTimerManager().SetTimer(BurnTimer, this, &UStatusEffectComponent::BurnTick, TickInterval, true);
	}

	if (!bWasBurning)
	{
		OnBurningChanged.Broadcast(true);
	}
}

void UStatusEffectComponent::BurnTick()
{
	DealDotDamage(BurnDamagePerTick, BurnInstigator.Get(), BurnCauser.Get());

	BurnTimeRemaining -= TickInterval;
	if (BurnTimeRemaining <= 0.f)
	{
		BurnTimeRemaining = 0.f;
		BurnDamagePerTick = 0.f;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(BurnTimer);
		}
		OnBurningChanged.Broadcast(false);
	}
}

// ---------- Poison ----------

void UStatusEffectComponent::AddPoisonStack(float DamagePerStackPerTick, int32 MaxStacks, float Duration, AController* InstigatorController, AActor* DamageCauser)
{
	UWorld* World = GetWorld();
	if (!World || DamagePerStackPerTick <= 0.f || Duration <= 0.f || MaxStacks <= 0)
	{
		return;
	}

	const int32 OldStacks = PoisonStacks;
	PoisonStacks = FMath::Min(PoisonStacks + 1, MaxStacks);
	PoisonDamagePerStack = FMath::Max(PoisonDamagePerStack, DamagePerStackPerTick);
	PoisonTimeRemaining = Duration; // each new stack refreshes the whole poison
	PoisonInstigator = InstigatorController;
	PoisonCauser = DamageCauser;

	if (!World->GetTimerManager().IsTimerActive(PoisonTimer))
	{
		World->GetTimerManager().SetTimer(PoisonTimer, this, &UStatusEffectComponent::PoisonTick, TickInterval, true);
	}

	if (PoisonStacks != OldStacks)
	{
		OnPoisonStacksChanged.Broadcast(PoisonStacks);
	}
}

void UStatusEffectComponent::PoisonTick()
{
	DealDotDamage(PoisonDamagePerStack * PoisonStacks, PoisonInstigator.Get(), PoisonCauser.Get());

	PoisonTimeRemaining -= TickInterval;
	if (PoisonTimeRemaining <= 0.f)
	{
		PoisonTimeRemaining = 0.f;
		PoisonStacks = 0;
		PoisonDamagePerStack = 0.f;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(PoisonTimer);
		}
		OnPoisonStacksChanged.Broadcast(0);
	}
}

// ---------- Shared ----------

void UStatusEffectComponent::DealDotDamage(float Amount, AController* InstigatorController, AActor* DamageCauser)
{
	AActor* Owner = GetOwner();
	if (Owner && Amount > 0.f)
	{
		UGameplayStatics::ApplyDamage(Owner, Amount, InstigatorController, DamageCauser, UDamageType::StaticClass());
	}
}

void UStatusEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(BurnTimer);
		World->GetTimerManager().ClearTimer(PoisonTimer);
	}
	Super::EndPlay(EndPlayReason);
}
