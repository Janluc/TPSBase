// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttackStruct.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EAttackSwing
{
	Center UMETA(DisplayName = "Center"),
    LeftToRightDiag UMETA(DisplayName ="Left To Right Diagonal"),
    RightToLeftDiag UMETA(DisplayName = "Right To Left Diagonal")
};

USTRUCT(BlueprintType)
struct FAttackStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EAttackSwing> AttackDirection;
	
};
