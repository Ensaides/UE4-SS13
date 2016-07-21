#pragma once

#include "OpenCL.h"
#include "AtmosVoxel.generated.h"

// float16 type that works with UE4
USTRUCT(Blueprintable)
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

USTRUCT()
struct FAtmosVoxel
{
	GENERATED_BODY()

	// Put these members in larger to smaller order for memory alignment

	// Gases
	cl_float16 Gases;

	// ID
	cl_int AdjacentVoxels[26];

	// State
	bool bValidVoxel;


	// THIS IS ONLY FOR MEMORY ALIGNMENT
	// To do: replace this with some proper alignment somehow
	cl_int MemShit[5];


	// Functions
	FAtmosVoxel()
	{
		bValidVoxel = true;
	}

	static FAtmosVoxel EmptyVoxel()
	{
		FAtmosVoxel ReturnVoxel;

		ReturnVoxel.bValidVoxel = false;

		return ReturnVoxel;
	}

	FGases GetFGases()
	{
		// These types should be convertible
		return *(FGases*)&Gases;
	}
};