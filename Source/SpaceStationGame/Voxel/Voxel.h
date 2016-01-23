// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Voxel.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTATIONGAME_API UVoxel : public UObject
{
	GENERATED_BODY()
	
	
	
	
};

UCLASS()
class SPACESTATIONGAME_API UAtmosVoxel : public UVoxel
{
	GENERATED_BODY()

	UPROPERTY()
		float fTemperature;

	UPROPERTY()
		TArray<UAtmosVoxel*> AdjacentVoxels;

};