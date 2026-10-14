// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/ArcaneProjectile.h"

AArcaneProjectile::AArcaneProjectile()
{
	Damage = 10.f;
	bIgnoresDefence = true; // the base class skips Defence when this is set
}
