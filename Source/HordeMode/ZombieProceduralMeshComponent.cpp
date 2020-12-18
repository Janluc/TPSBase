// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieProceduralMeshComponent.h"

void UZombieProceduralMeshComponent::BeginPlay()
{
	Super::BeginPlay();
	bUseAsyncCooking = true;
}
