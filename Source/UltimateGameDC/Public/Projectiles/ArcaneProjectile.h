// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "ArcaneProjectile.generated.h"

// Hit damage that ignores the target's Defence
UCLASS(BlueprintType, Blueprintable)
class ULTIMATEGAMEDC_API AArcaneProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AArcaneProjectile();
};
