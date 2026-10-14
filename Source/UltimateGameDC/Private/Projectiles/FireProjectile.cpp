// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/FireProjectile.h"
#include "Combat/StatusEffectComponent.h"

AFireProjectile::AFireProjectile()
{
	Damage = 8.f;
}

void AFireProjectile::OnProjectileImpact_Implementation(AActor* OtherActor, const FHitResult& Hit)
{
	Super::OnProjectileImpact_Implementation(OtherActor, Hit); // direct hit damage

	// The hit may have killed and destroyed the target
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (UStatusEffectComponent* Status = OtherActor->FindComponentByClass<UStatusEffectComponent>())
	{
		Status->ApplyBurn(BurnDamagePerTick, BurnDuration, GetInstigatorController(), this);
	}
}
