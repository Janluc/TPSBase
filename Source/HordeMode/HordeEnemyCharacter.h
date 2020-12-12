// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "ProceduralMeshComponent.h"
#include "GameFramework/Character.h"
#include "HordeEnemyCharacter.generated.h"

class UProceduralMeshComponent;
class AGunBase;
UCLASS()
class HORDEMODE_API AHordeEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	FHitResult HitReaction(FDamageEvent const& DamageEvent, AGunBase* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void CopySkeletalMeshToProcedural(USkeletalMeshComponent* SkeletalMeshComponent, int32 LODIndex, UProceduralMeshComponent* ProcMeshComponent);


public:
	// Sets default values for this character's properties
	AHordeEnemyCharacter();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	TArray<FVector> GetVertices();
	UFUNCTION(BlueprintPure)
    TArray<FVector> GetNormals();
	UFUNCTION(BlueprintPure)
	TArray<FProcMeshTangent> GetTangents();
	UFUNCTION(BlueprintPure)
	TArray<FVector2D> GetUVs();
};
