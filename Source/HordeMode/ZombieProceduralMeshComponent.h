// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ZombieProceduralMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class HORDEMODE_API UZombieProceduralMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
