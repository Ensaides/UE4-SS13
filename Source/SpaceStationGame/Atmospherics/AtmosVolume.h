// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Volume.h"
#include "AtmosVolume.generated.h"

USTRUCT()
struct FVoxelZ
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		class UVoxel* Voxel;
};

USTRUCT()
struct FVoxelY
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		TArray<FVoxelZ> Z;
};

USTRUCT()
struct FVoxelX
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		TArray<FVoxelY> Y;
};

UCLASS()
class SPACESTATIONGAME_API AAtmosVolume : public AVolume
{
	GENERATED_BODY()

	AAtmosVolume(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = Voxels)
		FVector VoxelSize;

	// 3D array of voxels
	UPROPERTY()
		TArray<FVoxelX> X;
};