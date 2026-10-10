#pragma once

UENUM(BlueprintType)
enum class EFemaleState :uint8
{
	EFS_UnEquiped UMETA(DisplayName = "UnEquiped"),
	EFS_EquippedOneHandWeapon UMETA(DisplayName = "Equipped One Hand Weapon"),
	EFS_EquippedTwoHandWeapon UMETA(DisplayName = "Equipped Two Hand Weapon")
};

UENUM(BlueprintType)
enum class EMaleState :uint8
{
	EMS_Depowered UMETA(DisplayName = "Depowered"),
	EMS_Empowered UMETA(DisplayName = "Empowered")
};

UENUM(BlueprintType)
enum class EActionState :uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Equipping UMETA(DisplayName = "Equipping"),
	EAS_Interacting UMETA(DisplayName = "Interacting"),
	EAS_Casting UMETA(DisplayName = "Casting")
};