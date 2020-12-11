// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"


#include "HordeEnemyCharacter.h"
#include "HordeModeCharacter.h"
#include "GameFramework/Character.h"


// Sets default values
AGunBase::AGunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunBase::PullTrigger()
{
	Shoot();
	GetWorldTimerManager().SetTimer(FireRateHandle, this, &AGunBase::Shoot, FireRate, true);
}

void AGunBase::ReleaseTrigger()
{
	GetWorldTimerManager().ClearTimer(FireRateHandle);
}

float AGunBase::GetWeaponKnockback()
{
	return WeaponKnockback;
}

float AGunBase::GetWeaponLimbKnockback()
{
	return WeaponLimbKnockback;
}


void AGunBase::Shoot()
{

	Owner = Cast<AHordeModeCharacter>(GetOwner());

	
	if (Owner == nullptr)
		return;

	if(!Owner->bIsAiming)
	{
		ReleaseTrigger();
		return;
	}
	Owner->PlayAnimMontage(FireAnim);
	FVector LookFrom;
	FRotator LookTo;
	Owner->GetController()->GetPlayerViewPoint(LookFrom, LookTo);
	FVector ShotDirection = -LookTo.Vector();

	FVector EndPoint = LookFrom + LookTo.Vector() * MaxRange;

	const FName TraceTag("MyTraceTag");
	 
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams QueryArgs(TraceTag, true, this);
	QueryArgs.AddIgnoredActor(GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, LookFrom, EndPoint, ECC_GameTraceChannel1, QueryArgs);
	if (!Hit.GetActor())
		return;
	AHordeEnemyCharacter* HitActor = Cast<AHordeEnemyCharacter>(Hit.GetActor());
	if(HitActor)
	{
		TSubclassOf<UDamageType> TempDamage;
		FPointDamageEvent PointDamage(WeaponDamage, Hit, ShotDirection, TempDamage);
		HitActor->TakeDamage(WeaponDamage, PointDamage, GetOwner()->GetInstigatorController(), this);
	}
}
