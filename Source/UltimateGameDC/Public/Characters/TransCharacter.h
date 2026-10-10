// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "TransCharacter.generated.h"

class UCapsuleComponent;
class USkeletalMesh;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UAnimInstance;
class UAnimMontage;
class AWeapon;
class AItem;

UCLASS()
class ULTIMATEGAMEDC_API ATransCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATransCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE bool IsFormB() const { return bIsFormB; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetEquippedWeapon(AWeapon* NewWeapon) { EquippedWeapon = NewWeapon; }

protected:
	virtual void BeginPlay() override;

	//void Move(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* CharacterMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TransformAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transformation")
	USkeletalMesh* FormAMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transformation")
	TSubclassOf<UAnimInstance> FormAAnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transformation")
	USkeletalMesh* FormBMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transformation")
	TSubclassOf<UAnimInstance> FormBAnimBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transformation")
	bool bIsFormB = false;

	UPROPERTY(EditAnywhere, Category = "Transformation | Echo")
	float FormAWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Transformation | Gideon")
	float FormBWalkSpeed = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Transformation | Gideon")
	float CurrentManaPool = 0.f;

	UPROPERTY(EditAnywhere, Category = "Transformation | Gideon")
	float MaxManaPool = 100.f;

	UPROPERTY(EditAnywhere, Category = "Transformation | Gideon")
	float ManaRegenRate = 5.f;

	UPROPERTY(EditAnywhere, Category = "Transformation | Gideon")
	float RegenWaitTime = 15.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AWeapon* EquippedWeapon = nullptr;

	/**
	* Input Callbacks
	*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void TransformCharacter();
	void Interact();
	void Attack();

	void ManaRegeneration(float DeltaTime);

	/**
	* Play Montage Functions
	*/

	void PlayMeleeAttackMontage();
	void PlayMagicAttackMontage();
	void PlayEquipMontage(FName SectionName);

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void CastEnd();
	UFUNCTION(BlueprintCallable)
	void Disarm();
	UFUNCTION(BlueprintCallable)
	void Rearm();
	UFUNCTION(BlueprintCallable)
	void EquipEnd();

	bool CanAttack();
	bool CanCast();
	bool CanDisarm();
	bool CanArm();

private:

	EFemaleState FemaleState = EFemaleState::EFS_UnEquiped;
	EMaleState MaleState = EMaleState::EMS_Depowered;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* EyeBrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/**
	* Animation Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* MeleeAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* MagicAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; };

	FORCEINLINE bool GetIsFormB() const { return bIsFormB; }

	FORCEINLINE EFemaleState GetFemaleState() const { return FemaleState; }
	FORCEINLINE EMaleState GetMaleState() const { return MaleState; }
};
	