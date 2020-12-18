// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "AttackStruct.h"
#include "GameFramework/Character.h"
#include "HordeEnemyCharacter.generated.h"

USTRUCT(BlueprintType)
struct FSkeletalCutPairs
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* TopHalf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicsAsset* TopHalfPhysics;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* BottomHalf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicsAsset* BottomHalfPhysics;
};

class UProceduralMeshComponent;
class AGunBase;
UCLASS()
class HORDEMODE_API AHordeEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	FHitResult HitReaction(FDamageEvent const& DamageEvent, AGunBase* DamageCauser);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliced Skeletal Meshes", meta = (AllowPrivateAccess = "true"))
	FSkeletalCutPairs RightToLeftDiagonal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliced Skeletal Meshes", meta = (AllowPrivateAccess = "true"))
	FSkeletalCutPairs LeftToRightDiagonal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliced Skeletal Meshes", meta = (AllowPrivateAccess = "true"))
	FSkeletalCutPairs Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	float Health;

public:
	// Sets default values for this character's properties
	AHordeEnemyCharacter();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void TakeMeleeDamage(FAttackStruct IncomingAttack);

	UFUNCTION(BlueprintImplementableEvent, Category="Horde Mode")
    void CutReaction(const TEnumAsByte<EAttackSwing>& AttackSwing);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category="Horde Mode")
	void ZombieDeath();

};
