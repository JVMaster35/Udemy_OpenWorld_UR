// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/ProjectileBase.h"
#include "Combat/StatusEffectComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as the collision representation
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	check(CollisionComponent != nullptr);

	// Editor preview only - the real value is applied in PreInitializeComponents
	CollisionComponent->InitSphereRadius(SphereRadius);

	// Create this projectile's mesh component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	check(ProjectileMesh != nullptr);

	// Attach the mesh component to the sphere component
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

	// Speed / gravity / lifespan are applied in PreInitializeComponents so that
	// values set by child classes and Blueprints are respected.
}

void AProjectileBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	CollisionComponent->SetSphereRadius(SphereRadius);
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->ProjectileGravityScale = GravityStrength;
	InitialLifeSpan = ProjectileLifespan;
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		OnProjectileImpact(OtherActor, Hit);
		Destroy();
	}
}

void AProjectileBase::OnProjectileImpact_Implementation(AActor* OtherActor, const FHitResult& Hit)
{
	// Targets with a StatusEffectComponent get Defence applied (unless bIgnoresDefence);
	// anything else just takes the raw damage.
	//if (UStatusEffectComponent* Status = OtherActor->FindComponentByClass<UStatusEffectComponent>())
	//{
	//	Status->ApplyHitDamage(Damage, bIgnoresDefence, GetInstigatorController(), this);
	//}
	//else
	//{
	//	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
	//}
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
