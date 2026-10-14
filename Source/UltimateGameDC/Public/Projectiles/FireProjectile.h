// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "FireProjectile.generated.h"

// Hit damage plus a burn (damage over time) on the target
UCLASS(BlueprintType, Blueprintable)
class ULTIMATEGAMEDC_API AFireProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AFireProjectile();

protected:
	virtual void OnProjectileImpact_Implementation(AActor* OtherActor, const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Fire")
	float BurnDamagePerTick = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Fire")
	float BurnDuration = 4.f;
};
