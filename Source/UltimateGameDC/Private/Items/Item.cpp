// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "UltimateGameDC/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/TransCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	itemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = itemMesh;

	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	collisionSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	collisionSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformSin()
{
	return FMath::Sin(runningTime * timeConstant) * amplitude;
}

float AItem::TransformCosin()
{
	return FMath::Cos(runningTime * timeConstant) * amplitude;
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATransCharacter* TransCharacter = Cast<ATransCharacter>(OtherActor);
	if (TransCharacter)
	{
		TransCharacter->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATransCharacter* TransCharacter = Cast<ATransCharacter>(OtherActor);
	if (TransCharacter)
	{
		TransCharacter->SetOverlappingItem(nullptr);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	runningTime += DeltaTime;
	
	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformSin()));
	}
}

