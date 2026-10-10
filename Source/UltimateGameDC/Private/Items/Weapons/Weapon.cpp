// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/TransCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/EngineTypes.h"

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
		Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

bool AWeapon::Equip(bool isMaleForm, USceneComponent* InParent, FName InSocketName)
{
	if (isMaleForm)
	{
		return false;
	}

	itemMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	itemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	itemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	itemMesh->SetGenerateOverlapEvents(true);
	AttachMeshToSocket(InParent, InSocketName);
	SetActorTickEnabled(false);
	collisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	return true;
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	itemMesh->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetIncludingScale, InSocketName);
	ItemState = EItemState::EIS_Equipped;
}
