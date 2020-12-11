// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeEnemyCharacter.h"

#include "GunBase.h"


// Sets default values
AHordeEnemyCharacter::AHordeEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHordeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

float AHordeEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if(!DamageEvent.IsOfType(1))
	{
		return 0;
	}
	AGunBase* GunHitBy = Cast<AGunBase>(DamageCauser);
	if(!GunHitBy)
		return 0;
	
	FHitResult HitResult = HitReaction(DamageEvent, GunHitBy);
	UE_LOG(LogTemp, Warning, TEXT("Enemy Hit at %s"), *HitResult.BoneName.ToString());
	return 0;
}

// Called every frame
void AHordeEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FHitResult AHordeEnemyCharacter::HitReaction(FDamageEvent const& DamageEvent, AGunBase* DamageCauser)
{
	const FPointDamageEvent* HitPoint;
	HitPoint = static_cast<const FPointDamageEvent*>(&DamageEvent);
	FHitResult HitResult = HitPoint->HitInfo;
	if (HitResult.BoneName != "pelvis")
	{
		FVector ShotDirection = HitPoint->ShotDirection + FVector(0,0, 50);
		float ImpulseAmount = HitResult.BoneName == "spine_02" ? DamageCauser->GetWeaponKnockback() : 200;
		GetMesh()->AddImpulseAtLocation(ShotDirection * ImpulseAmount, HitResult.ImpactPoint, HitResult.BoneName);
	}
	return HitResult;
}
