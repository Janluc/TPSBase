// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AttackStruct.h"
#include "HordeEnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "MeleeWeaponBase.generated.h"



UCLASS()
class HORDEMODE_API AMeleeWeaponBase : public AActor
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;


	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace6;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace7;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace8;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace9;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Trace", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponTrace10;


	FVector WeaponTraceLocation;
	FVector WeaponTraceLocation1;
	FVector WeaponTraceLocation2;
	FVector WeaponTraceLocation3;
	FVector WeaponTraceLocation4;
	FVector WeaponTraceLocation5;
	FVector WeaponTraceLocation6;
	FVector WeaponTraceLocation7;
	FVector WeaponTraceLocation8;
	FVector WeaponTraceLocation9;
	FVector WeaponTraceLocation10;
	

	TArray<AHordeEnemyCharacter*>ListOfHitActors;
	TArray<USceneComponent*> WeaponTraces;
	TArray<FVector> WeaponTraceLocations;
	bool EnterAttack = false;
	bool TracesTurnedOn = false;
public:	
	// Sets default values for this actor's properties
	AMeleeWeaponBase();

	void SwingWeapon(int32 Counter);

	UFUNCTION(BlueprintCallable)
	void AttackStatus(bool On);
	UFUNCTION(BlueprintCallable)
	void ClearListOfActors();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Attacks")
	TArray<FAttackStruct> Combos;
	int32 WeaponComboCounter;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
