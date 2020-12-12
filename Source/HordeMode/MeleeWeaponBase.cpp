// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponBase.h"

#include "GameFramework/Character.h"

// Sets default values
AMeleeWeaponBase::AMeleeWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;
}

void AMeleeWeaponBase::SwingWeapon()
{
	GetOwner()->GetInstigatorController()->GetCharacter()->PlayAnimMontage(ComboAnimMontages[0]);
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

}

