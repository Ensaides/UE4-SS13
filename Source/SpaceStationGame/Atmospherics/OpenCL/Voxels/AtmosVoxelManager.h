#pragma once

#include "Object.h"

#include "AtmosVoxel.h"

#define ATMOS_VOXEL_ARRAY_SIZE 32000;

class UAtmosVoxelManager
{
protected:
	static AtmosVoxel AtmosVoxels[ATMOS_VOXEL_ARRAY_SIZE];
	static bool ValidAtmosVoxels[ATMOS_VOXEL_ARRAY_SIZE];

public:
	static int GetFirstInvalidVoxel() {
		for (int i = 0; i < ATMOS_VOXEL_ARRAY_SIZE; i++)
		{
			if (ValidAtmosVoxels[i] == false)
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
			ValidAtmosVoxels[FirstVoxel] = true;
		}
	};

	static void RemoveVoxel(int Index) {
		if (Index <= ATMOS_VOXEL_ARRAY_SIZE)
		{
			ValidAtmosVoxels[Index] = false;
		}
	};

	static AtmosVoxel[ATMOS_VOXEL_ARRAY_SIZE]& GetVoxels() { return AtmosVoxels; };
};