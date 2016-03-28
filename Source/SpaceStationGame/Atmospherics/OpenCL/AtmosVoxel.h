#pragma once

#include "OpenCL.h"

#define ATMOS_VOXEL_ARRAY_SIZE 32000

namespace Atmospherics
{
	typedef struct
	{
		// Put these members in larger to smaller order for memory alignment

		// Gasses
		cl_float16 Gasses;
		cl_float16 DeltaGasses;

		// Pos
		cl_float XPos;
		cl_float YPos;
		cl_float ZPos;

		// ID
		cl_int AdjacentVoxels[8];

		// State
		unsigned char bNeedsUpdate : 1;
		unsigned char bValueChanged : 1;
		unsigned char bBlocked : 1;
		unsigned char bValidVoxel : 1;
	} AtmosVoxel;

	AtmosVoxel AtmosVoxels[ATMOS_VOXEL_ARRAY_SIZE];

	static int GetFirstInvalidVoxel() {
		for (int i = 0; i < ATMOS_VOXEL_ARRAY_SIZE; i++)
		{
			if (AtmosVoxels[i].bValidVoxel == false)
			{
				return i;
			}
		}
		return -1; // If there is no more space for new voxels
	};

	static void AddVoxel(AtmosVoxel NewVoxel) {
		int FirstVoxel = GetFirstInvalidVoxel();

		if (FirstVoxel > -1)
		{
			AtmosVoxels[FirstVoxel] = NewVoxel;
			AtmosVoxels[FirstVoxel].bValidVoxel = true;
		}
	};

	static void RemoveVoxel(int Index) {
		if (Index <= ATMOS_VOXEL_ARRAY_SIZE && Index >= 0)
		{
			AtmosVoxels[Index].bValidVoxel = false;
		}
	};
}