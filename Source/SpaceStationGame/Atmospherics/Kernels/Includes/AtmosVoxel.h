#pragma once

#ifdef __cplusplus
#include "OpenCL.h"

namespace Atmospherics
{

#endif


typedef struct
{
	unsigned char Additive : 1;
} AtmosVoxelFlags;

enum AtmosWriteOperation
{
	Add,
	Subtract,
	Set,
	SetAdditive,
	SetSubtractive
};


#ifdef __cplusplus

	typedef struct AtmosVoxel
	{
		// Gases
		cl_float16 Gases;

		// Index
		cl_long Index;

		// ID
		cl_int AdjacentVoxels[26];

		// State
		bool bValidVoxel;

		// The flags for the state of this voxel.
		AtmosVoxelFlags Flags;

		// THIS IS ONLY FOR MEMORY ALIGNMENT
		// To do: replace this with some proper alignment somehow
		cl_int MemShit[3];

		// Functions
		AtmosVoxel()
		{
			bValidVoxel = true;
		}

		static AtmosVoxel EmptyVoxel()
		{
			AtmosVoxel ReturnVoxel;

			ReturnVoxel.bValidVoxel = false;

			return ReturnVoxel;
		}
	};

#else

typedef struct
{
	// Gasses
	float16 Gases;

	long Index;

	// ID
	int AdjacentVoxels[26];

	// State
	bool bValidVoxel;

	AtmosVoxelFlags Flags;

} AtmosVoxel;

#endif



#ifdef __cplusplus

	typedef struct AtmosVoxelWrite
	{
		// Value
		cl_float16 Value;

		// Index
		cl_long Index;

		bool bValidOperation;
	};

#else

typedef struct
{
	// Value
	float16 Value;

	// Index
	long Index;

	bool bValidOperation;
} AtmosVoxelWrite;

#endif

#ifdef __cplusplus
};
#endif