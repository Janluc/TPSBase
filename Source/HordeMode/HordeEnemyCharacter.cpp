// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeEnemyCharacter.h"

#include "ProceduralMeshComponent.h"
#include "GunBase.h"
#include "Rendering/MultiSizeIndexContainer.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Rendering/SkeletalMeshRenderData.h"


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
		float ImpulseAmount = HitResult.BoneName == "spine_02" ? DamageCauser->GetWeaponKnockback() : DamageCauser->GetWeaponLimbKnockback();
		GetMesh()->AddImpulseAtLocation(ShotDirection * ImpulseAmount, HitResult.ImpactPoint, HitResult.BoneName);
	}
	return HitResult;
}

void AHordeEnemyCharacter::CopySkeletalMeshToProcedural(USkeletalMeshComponent* SkeletalMeshComponent, int32 LODIndex,
	UProceduralMeshComponent* ProcMeshComponent)
{
	FSkeletalMeshRenderData* SkMeshRenderData = SkeletalMeshComponent->GetSkeletalMeshRenderData();
     const FSkeletalMeshLODRenderData& DataArray = SkMeshRenderData->LODRenderData[LODIndex];
     FSkinWeightVertexBuffer& SkinWeights = *SkeletalMeshComponent->GetSkinWeightBuffer(LODIndex);
 
     TArray<FVector> VerticesArrayTemp;
     TArray<FVector> NormalsTemp;
     TArray<FVector2D> UVTemp;
     TArray<int32> TrisTemp;
     TArray<FColor> ColorsTemp;
     TArray<FProcMeshTangent> TangentsTemp;
 
    
     //get num vertices
     int32 NumSourceVertices = DataArray.RenderSections[0].NumVertices;
 
     for (int32 i = 0; i < NumSourceVertices; i++)
     {
         //get skinned vector positions
         FVector SkinnedVectorPos = USkeletalMeshComponent::GetSkinnedVertexPosition(SkeletalMeshComponent, i, DataArray, SkinWeights);
         VerticesArrayTemp.Add(SkinnedVectorPos);
 
         //Calc normals and tangents from the static version instead of the skeletal one
         FVector ZTangentStatic = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(i);
         FVector XTangentStatic = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentX(i);
 
         //add normals from the static mesh version instead because using the skeletal one doesn't work right.
         NormalsTemp.Add(ZTangentStatic);

         //add tangents
         TangentsTemp.Add(FProcMeshTangent(XTangentStatic, false));
 
         //get UVs
         FVector2D uvs = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(i, 0);
         UVTemp.Add(uvs);
 
         //dummy vertex colors
         ColorsTemp.Add(FColor(0, 0, 0, 255));
     }
 
 
     //get index buffer
     FMultiSizeIndexContainerData indicesData;
     DataArray.MultiSizeIndexContainer.GetIndexBuffer(indicesData.Indices);
 
     //iterate over num indices and add traingles
     for (int32 i = 0; i < indicesData.Indices.Num(); i++)
     {
         uint32 a = 0;
         a = indicesData.Indices[i];
         TrisTemp.Add(a);
     }
 
     //Create the procedural mesh
     ProcMeshComponent->CreateMeshSection(0, VerticesArrayTemp, TrisTemp, NormalsTemp, UVTemp, ColorsTemp, TangentsTemp, true);
}


TArray<FVector> AHordeEnemyCharacter::GetVertices()
{
	FSkeletalMeshRenderData* SkMeshRenderData = GetMesh()->GetSkeletalMeshRenderData();
	const FSkeletalMeshLODRenderData& DataArray = SkMeshRenderData->LODRenderData[0];
	FSkinWeightVertexBuffer& SkinWeights = *GetMesh()->GetSkinWeightBuffer(0);

	TArray<FVector> VerticesArrayTemp;

	
	
	int32 NumSourceVertices = DataArray.RenderSections[0].NumVertices;

	for (int32 i = 0; i < NumSourceVertices; i++)
	{
		FVector SkinnedVectorPos = USkeletalMeshComponent::GetSkinnedVertexPosition(GetMesh(), i, DataArray, SkinWeights);
		VerticesArrayTemp.Add(SkinnedVectorPos);
	}

	return VerticesArrayTemp;
	
}

TArray<FVector> AHordeEnemyCharacter::GetNormals()
{
	FSkeletalMeshRenderData* SkMeshRenderData = GetMesh()->GetSkeletalMeshRenderData();
	const FSkeletalMeshLODRenderData& DataArray = SkMeshRenderData->LODRenderData[0];

	TArray<FVector> NormalsTemp;

	
	
	int32 NumSourceVertices = DataArray.RenderSections[0].NumVertices;

	for (int32 i = 0; i < NumSourceVertices; i++)
	{
		//Calc normals and tangents from the static version instead of the skeletal one
		FVector ZTangentStatic = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(i);
 
		//add normals from the static mesh version instead because using the skeletal one doesn't work right.
		NormalsTemp.Add(ZTangentStatic);
	}

	return NormalsTemp;
	
}


TArray<FProcMeshTangent> AHordeEnemyCharacter::GetTangents()
{
	FSkeletalMeshRenderData* SkMeshRenderData = GetMesh()->GetSkeletalMeshRenderData();
	const FSkeletalMeshLODRenderData& DataArray = SkMeshRenderData->LODRenderData[0];


	TArray<FProcMeshTangent> TangentsTemp;

	
	
	int32 NumSourceVertices = DataArray.RenderSections[0].NumVertices;

	for (int32 i = 0; i < NumSourceVertices; i++)
	{
		//Calc normals and tangents from the static version instead of the skeletal one

		FVector XTangentStatic = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentX(i);
 
		//add tangents
		TangentsTemp.Add(FProcMeshTangent(XTangentStatic, false));
	}

	return TangentsTemp;
}

TArray<FVector2D> AHordeEnemyCharacter::GetUVs()
{
	FSkeletalMeshRenderData* SkMeshRenderData = GetMesh()->GetSkeletalMeshRenderData();
	const FSkeletalMeshLODRenderData& DataArray = SkMeshRenderData->LODRenderData[0];

	TArray<FVector2D> UVTemp;

	//get UVs

	
	int32 NumSourceVertices = DataArray.RenderSections[0].NumVertices;

	for (int32 i = 0; i < NumSourceVertices; i++)
	{
		FVector2D uvs = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(i, 0);
		UVTemp.Add(uvs);
	}

	return UVTemp;
}
