// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "PoisonProjectile.generated.h"

// Hit damage plus one poison stack; poison damage scales with stacks
UCLASS(BlueprintType, Blueprintable)
class ULTIMATEGAMEDC_API APoisonProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	APoisonProjectile();

protected:
	virtual void OnProjectileImpact_Implementation(AActor* OtherActor, const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Poison")
	float PoisonDamagePerStack = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Poison", meta = (ClampMin = "1"))
	int32 MaxPoisonStacks = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Poison")
	float PoisonDuration = 6.f;
};
