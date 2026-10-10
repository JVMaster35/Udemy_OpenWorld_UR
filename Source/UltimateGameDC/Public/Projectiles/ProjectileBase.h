// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(BlueprintType, Blueprintable)
class ULTIMATEGAMEDC_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	//Call when object collides with another object
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Mesh of the projectile in the world 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Mesh")
	UStaticMeshComponent* ProjectileMesh;

	// Despawn after 5 seconds by default
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Lifespan")
	float ProjectileLifespan = 5.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile | Components")
	USphereComponent* CollisionComponent;

	// Projectile movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile | Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Projectile | Variables")
	float SphereRadius = 5.f;

	UPROPERTY(EditAnywhere, Category = "Projectile | Variables")
	float MaxSpeed = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Projectile | Variables")
	float InitialSpeed = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Projectile | Variables")
	float GravityStrength = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
