// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponBase.h"


#include "HordeEnemyCharacter.h"
#include "GameFramework/Character.h"

// Sets default values
AMeleeWeaponBase::AMeleeWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;

	WeaponTrace = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace"));
	WeaponTrace->SetupAttachment(WeaponMesh);
	WeaponTrace1 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace1"));
	WeaponTrace1->SetupAttachment(WeaponMesh);
	WeaponTrace2 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace2"));
	WeaponTrace2->SetupAttachment(WeaponMesh);
	WeaponTrace3 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace3"));
	WeaponTrace3->SetupAttachment(WeaponMesh);
	WeaponTrace4 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace4"));
	WeaponTrace4->SetupAttachment(WeaponMesh);
	WeaponTrace5 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace5"));
	WeaponTrace5->SetupAttachment(WeaponMesh);
	WeaponTrace6 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace6"));
	WeaponTrace6->SetupAttachment(WeaponMesh);
	WeaponTrace7 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace7"));
	WeaponTrace7->SetupAttachment(WeaponMesh);
	WeaponTrace8 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace8"));
	WeaponTrace8->SetupAttachment(WeaponMesh);
	WeaponTrace9 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace9"));
	WeaponTrace9->SetupAttachment(WeaponMesh);
	WeaponTrace10 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Trace10"));
	WeaponTrace10->SetupAttachment(WeaponMesh);

	WeaponTraces.Add(WeaponTrace);
	WeaponTraces.Add(WeaponTrace1);
	WeaponTraces.Add(WeaponTrace2);
	WeaponTraces.Add(WeaponTrace3);
	WeaponTraces.Add(WeaponTrace4);
	WeaponTraces.Add(WeaponTrace5);
	WeaponTraces.Add(WeaponTrace6);
	WeaponTraces.Add(WeaponTrace7);
	WeaponTraces.Add(WeaponTrace8);
	WeaponTraces.Add(WeaponTrace9);
	WeaponTraces.Add(WeaponTrace10);

	WeaponTraceLocations.Add(WeaponTraceLocation);
	WeaponTraceLocations.Add(WeaponTraceLocation2);
	WeaponTraceLocations.Add(WeaponTraceLocation3);
	WeaponTraceLocations.Add(WeaponTraceLocation4);
	WeaponTraceLocations.Add(WeaponTraceLocation5);
	WeaponTraceLocations.Add(WeaponTraceLocation6);
	WeaponTraceLocations.Add(WeaponTraceLocation7);
	WeaponTraceLocations.Add(WeaponTraceLocation8);
	WeaponTraceLocations.Add(WeaponTraceLocation9);
	WeaponTraceLocations.Add(WeaponTraceLocation10);
}

void AMeleeWeaponBase::SwingWeapon(int32 Counter)
{
	WeaponComboCounter = Counter;
	GetOwner()->GetInstigatorController()->GetCharacter()->PlayAnimMontage(Combos[Counter].AttackAnim);
}

void AMeleeWeaponBase::AttackStatus(bool On)
{
	TracesTurnedOn = On;
	EnterAttack = On;
}

void AMeleeWeaponBase::ClearListOfActors()
{
	ListOfHitActors.Empty();
}


// Called when the game starts or when spawned
void AMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMeleeWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TracesTurnedOn)
	{
		
		if (EnterAttack)
		{
			for(int i = 0; i < WeaponTraces.Num() - 1; i++)
			{
				WeaponTraceLocations[i] = WeaponTraces[i]->GetComponentLocation(); 
			}
			EnterAttack = false;
		}

		for(int i = 0; i < WeaponTraces.Num() - 1; i++)
		{
			const FName TraceTag("MyTraceTag");
			GetWorld()->DebugDrawTraceTag = TraceTag;
			FCollisionQueryParams QueryArgs(TraceTag, false, this);
			QueryArgs.AddIgnoredActor(GetOwner());
			FHitResult Hit;
			GetWorld()->LineTraceSingleByChannel(Hit, WeaponTraceLocations[i], WeaponTraces[i]->GetComponentLocation(),ECC_GameTraceChannel2, QueryArgs);
			WeaponTraceLocations[i] = WeaponTraces[i]->GetComponentLocation();
			AHordeEnemyCharacter* HitActor;
			HitActor = Cast<AHordeEnemyCharacter>(Hit.Actor);
			if(!HitActor)
			{
				continue;
			}
			if (ListOfHitActors.Contains(HitActor))
			{
				return;
			}
			HitActor->TakeMeleeDamage(Combos[WeaponComboCounter]);
			ListOfHitActors.Add(HitActor);
			
		}
		
	}
}

