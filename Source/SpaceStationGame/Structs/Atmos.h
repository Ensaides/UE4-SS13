#pragma once
#include "Kernels/Includes/AtmosVoxel.h"
#include "Atmos.generated.h"

USTRUCT(BlueprintType)
struct FGases
{
	GENERATED_BODY()

	UPROPERTY()
		float s1;

	UPROPERTY()
		float s2;

	UPROPERTY()
		float s3;

	UPROPERTY()
		float s4;

	UPROPERTY()
		float s5;

	UPROPERTY()
		float s6;

	UPROPERTY()
		float s7;

	UPROPERTY()
		float s8;

	UPROPERTY()
		float s9;

	UPROPERTY()
		float s10;

	UPROPERTY()
		float sA;

	UPROPERTY()
		float sB;

	UPROPERTY()
		float sC;

	UPROPERTY()
		float sD;

	UPROPERTY()
		float sE;

	UPROPERTY()
		float sF;

	cl_float16 ToFloat16()
	{
		return *(cl_float16*)this;
	}
};

// Wrapper for the OpenCL type AtmosVoxel
USTRUCT(BlueprintType)
struct FAtmosVoxel
{
	GENERATED_BODY()

	Atmospherics::AtmosVoxel Voxel;

	// Functions
	FAtmosVoxel()
	{
		Voxel.bValidVoxel = true;
	}

	static FAtmosVoxel EmptyVoxel()
	{
		FAtmosVoxel ReturnVoxel;

		ReturnVoxel.Voxel.bValidVoxel = false;

		return ReturnVoxel;
	}

	FGases GetFGases()
	{
		// These types should be convertible
		return *(FGases*)&Voxel.Gases;
	}
};