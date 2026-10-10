// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TransCharacter.h"

#include "Items/Weapons/Weapon.h"
#include "Items/Item.h"

#include "Projectiles/ProjectileBase.h"

#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GroomComponent.h"

// Sets default values
ATransCharacter::ATransCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(82.f);
	GetCapsuleComponent()->SetCapsuleRadius(32.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 350.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	EyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	EyeBrows->SetupAttachment(GetMesh());
	EyeBrows->AttachmentName = FString("head");
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ATransCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}

	CurrentManaPool = MaxManaPool;
}

void ATransCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFormB && CurrentManaPool < MaxManaPool) {
		ManaRegeneration(DeltaTime);
	}

	if(bIsFormB && CurrentManaPool <= 0.f)
	{
		MaleState = EMaleState::EMS_Depowered;
	}
	else 
	{
		MaleState = EMaleState::EMS_Empowered;
	}
}

void ATransCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ATransCharacter::Move);
		EnhancedInputComponent->BindAction(LookingAction, ETriggerEvent::Triggered, this, &ATransCharacter::Look);
		EnhancedInputComponent->BindAction(TransformAction, ETriggerEvent::Started, this, &ATransCharacter::TransformCharacter);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATransCharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ATransCharacter::Interact);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ATransCharacter::Attack);
	}

	//PlayerInputComponent->BindAxis("Move", this, &ATransCharacter::Move);
}

void ATransCharacter::Move(const FInputActionValue& Value)
{
	if(ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(FowardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ATransCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ATransCharacter::TransformCharacter()
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	bIsFormB = !bIsFormB;

	USkeletalMesh* NewMesh = bIsFormB ? FormBMesh : FormAMesh;
	TSubclassOf<UAnimInstance> NewAnim = bIsFormB ? FormBAnimBP : FormAAnimBP;
	float NewSpeed = bIsFormB ? FormBWalkSpeed : FormAWalkSpeed;

	FemaleState = EquippedWeapon && !bIsFormB ? EFemaleState::EFS_EquippedOneHandWeapon : EFemaleState::EFS_UnEquiped;
	MaleState = CurrentManaPool > 0.f ? EMaleState::EMS_Empowered : EMaleState::EMS_Depowered;

	if (EquippedWeapon)
	{
		// Weapon only wielded in Form B (female)
		EquippedWeapon->SetActorHiddenInGame(bIsFormB);
		EquippedWeapon->SetActorEnableCollision(!bIsFormB);
	}

	if (NewMesh && NewAnim)
	{
		// Swap mesh and re-initialise the anim instance
		GetMesh()->SetSkeletalMesh(NewMesh);
		GetMesh()->SetAnimInstanceClass(NewAnim);
		GetMesh()->InitAnim(true); // <-- critical: forces anim BP to reinitialise

		// Swap movement stats
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}

void ATransCharacter::Interact()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	if (OverlappingWeapon && OverlappingWeapon->Equip(bIsFormB, GetMesh(), FName("rightHandSocket")))
	{
		FemaleState = EFemaleState::EFS_EquippedOneHandWeapon;
		OverlappingItem = nullptr;
		SetEquippedWeapon(OverlappingWeapon);
	}
	else {
		if(CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			ActionState = EActionState::EAS_Equipping;
			FemaleState = EFemaleState::EFS_UnEquiped;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			ActionState = EActionState::EAS_Equipping;
			FemaleState = EFemaleState::EFS_EquippedOneHandWeapon;
		}
	}
}

void ATransCharacter::Attack()
{
	if (!bIsFormB)
	{
		if (!CanAttack()) return;

		PlayMeleeAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	else
	{
		if (!CanCast()) return;

		PlayMagicAttackMontage();
		ActionState = EActionState::EAS_Casting;
	}
}

bool ATransCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		EquippedWeapon && 
		FemaleState == EFemaleState::EFS_EquippedOneHandWeapon;
}

bool ATransCharacter::CanCast()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CurrentManaPool > 0.f;
}

bool ATransCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		FemaleState != EFemaleState::EFS_UnEquiped;
}

bool ATransCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		FemaleState == EFemaleState::EFS_UnEquiped && 
		EquippedWeapon;
}

void ATransCharacter::ManaRegeneration(float DeltaTime)
{
	CurrentManaPool += ManaRegenRate * DeltaTime;
	CurrentManaPool = FMath::Clamp(CurrentManaPool, 0.f, MaxManaPool);
}

void ATransCharacter::PlayMeleeAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (CanAttack() && AnimInstance && MeleeAttackMontage)
	{
		AnimInstance->Montage_Play(MeleeAttackMontage);
		const int32 SectionIndex = FMath::RandRange(0, 1);
		FName SectionName = FName();

		switch (SectionIndex)
		{
		case 0:
			SectionName = FName("MeleeAttack1");
			break;
		case 1:
			SectionName = FName("MeleeAttack2");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, MeleeAttackMontage);
	}
}

void ATransCharacter::PlayMagicAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (CanCast() && AnimInstance && MagicAttackMontage)
	{
		AnimInstance->Montage_Play(MagicAttackMontage);
		const int32 SectionIndex = FMath::RandRange(0, 1);
		FName SectionName = FName();

		switch (SectionIndex)
		{
		case 0:
			SectionName = FName("MagicAttack1");
			break;
		case 1:
			SectionName = FName("MagicAttack2");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, MagicAttackMontage);

		//// Fire projectile
		//if (ProjectileClass)
		//{
		//	FVector MuzzleLocation = GetMesh()->GetSocketLocation(FName("rightHandSocket"));
		//	FRotator MuzzleRotation = GetControlRotation();
		//	FActorSpawnParameters SpawnParams;
		//	SpawnParams.Owner = this;
		//	SpawnParams.Instigator = GetInstigator();
		//	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		//	if (Projectile)
		//	{
		//		Projectile->SetOwner(this);
		//		CurrentManaPool -= Projectile->ManaCost;
		//		CurrentManaPool = FMath::Clamp(CurrentManaPool, 0.f, MaxManaPool);
		//	}
		//}
	}
}

void ATransCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ATransCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ATransCharacter::CastEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ATransCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("spineSocket"));
	}
}

void ATransCharacter::Rearm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("rightHandSocket"));
	}
}

void ATransCharacter::EquipEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

