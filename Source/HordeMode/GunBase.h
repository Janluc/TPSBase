// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

class AHordeModeCharacter;
UCLASS()
class HORDEMODE_API AGunBase : public AActor
{
	GENERATED_BODY()

	void Shoot();
	UPROPERTY(EditAnywhere)
	float FireRate = 0.1;
	UPROPERTY(EditAnywhere)
	float MaxRange = 10000;
	UPROPERTY(EditAnywhere)
	float WeaponDamage = 20;
	UPROPERTY(EditAnywhere)
	float WeaponKnockback = 1000;
	UPROPERTY(EditAnywhere)
	float WeaponLimbKnockback = 200; 
	FTimerHandle FireRateHandle;

	
	UPROPERTY(EditAnywhere)
	UAnimMontage* FireAnim;
	AHordeModeCharacter* Owner;

public:	
	// Sets default values for this actor's properties
	AGunBase();
	void PullTrigger();
	void ReleaseTrigger();
	float GetWeaponKnockback();
	float GetWeaponLimbKnockback();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
