// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceStationGame.h"
#include "AtmosVolume.h"

AAtmosVolume::AAtmosVolume(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	VoxelSize = FVector(256.f, 256.f, 256.f);

	FVector NumOfVoxels = ( GetBrushComponent()->Bounds.BoxExtent * 2 ) / VoxelSize;

	uint32 iNumOfVoxelsX = FMath::FloorToInt(FMath::Abs(NumOfVoxels.X));
	uint32 iNumOfVoxelsY = FMath::FloorToInt(FMath::Abs(NumOfVoxels.Y));
	uint32 iNumOfVoxelsZ = FMath::FloorToInt(FMath::Abs(NumOfVoxels.Z));

	// For each of the 3 dimensions, spawn voxels
	for (uint32 it = 0; it != iNumOfVoxelsX; it++)
	{

	}
}

void AAtmosVolume::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BOX EXTENT XYZ: %s"), *( GetBrushComponent()->Bounds.BoxExtent * 2).ToString());

	UE_LOG(LogTemp, Warning, TEXT("ORIGIN XYZ: %s"), *GetBrushComponent()->GetComponentLocation().ToString());
}
