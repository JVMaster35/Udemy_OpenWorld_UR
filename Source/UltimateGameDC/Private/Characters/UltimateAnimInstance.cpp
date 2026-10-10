// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/UltimateAnimInstance.h"
#include "Characters/TransCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"

void UUltimateAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TransCharacter = Cast<ATransCharacter>(TryGetPawnOwner());
	if(TransCharacter)
	{
		TransCharacterMovement = TransCharacter->GetCharacterMovement();
	}

}

void UUltimateAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(TransCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(TransCharacterMovement->Velocity);
		IsFalling = TransCharacterMovement->IsFalling();

		if(TransCharacter->GetIsFormB())
		{
			MaleState = TransCharacter->GetMaleState();
		}
		else
		{
			FemaleState = TransCharacter->GetFemaleState();
		}
	}
}
