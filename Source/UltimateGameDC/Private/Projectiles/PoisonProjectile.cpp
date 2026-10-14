// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/PoisonProjectile.h"
#include "Combat/StatusEffectComponent.h"

APoisonProjectile::APoisonProjectile()
{
	Damage = 5.f;
}

void APoisonProjectile::OnProjectileImpact_Implementation(AActor* OtherActor, const FHitResult& Hit)
{
	Super::OnProjectileImpact_Implementation(OtherActor, Hit); // direct hit damage

	// The hit may have killed and destroyed the target
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (UStatusEffectComponent* Status = OtherActor->FindComponentByClass<UStatusEffectComponent>())
	{
		Status->AddPoisonStack(PoisonDamagePerStack, MaxPoisonStacks, PoisonDuration, GetInstigatorController(), this);
	}
}
