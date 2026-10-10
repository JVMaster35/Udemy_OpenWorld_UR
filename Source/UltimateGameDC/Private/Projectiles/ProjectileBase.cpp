// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a casule as the collision representation
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	check(CollisionComponent != nullptr);

	CollisionComponent->InitSphereRadius(SphereRadius);

	// Create this projectile's mesh component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	check(ProjectileMesh != nullptr);

	// Attach the sphere component to the mesh component  
	ProjectileMesh->SetupAttachment(CollisionComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	check(ProjectileMovement != nullptr);

	// Set the collision profile to the "Projectile" collision preset
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");

	// Set up a notification for when this component hits something blocking
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	// Set as root component and UpdatedComponent
	RootComponent = CollisionComponent;

	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->ProjectileGravityScale = GravityStrength;

	// Disappear after 5.0 seconds by default.
	InitialLifeSpan = ProjectileLifespan;
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics object
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		Destroy();
	}
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

