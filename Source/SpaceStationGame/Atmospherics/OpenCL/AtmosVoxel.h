#pragma once

#include "OpenCL.h"
#include <array>
#include <utility>

#define ATMOS_VOXEL_ARRAY_SIZE 128000

namespace Atmospherics
{
	typedef struct AtmosVoxel
	{
		// Put these members in larger to smaller order for memory alignment

		// Gasses
		cl_float16 Gasses;

		// Pos
		cl_float XPos;
		cl_float YPos;
		cl_float ZPos;

		// ID
		cl_int AdjacentVoxels[8];
		cl_int UpdateVoxelIndex;

		// State
		bool bValidVoxel;

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

	static std::array<AtmosVoxel, ATMOS_VOXEL_ARRAY_SIZE> AtmosVoxels;

	static int AtmosVoxelsSize(ATMOS_VOXEL_ARRAY_SIZE);

	static std::array<int, ATMOS_VOXEL_ARRAY_SIZE> UpdateAtmosVoxels;

	static int UpdateAtmosVoxelsSize;

	static int GetFirstInvalidVoxel() 
	{
		for (int i = 0; i < ATMOS_VOXEL_ARRAY_SIZE; i++)
		{
			if (AtmosVoxels[i].bValidVoxel == false)
			{
				return i;
			}
		}
		return -1; // If there is no more space for new voxels
	};

	static void AddVoxel(AtmosVoxel NewVoxel) 
	{
		int FirstVoxel = GetFirstInvalidVoxel();

		if (FirstVoxel > -1)
		{
			AtmosVoxels[FirstVoxel] = NewVoxel;
		}
	};

	static void RemoveVoxel(int Index) 
	{
		if (Index <= ATMOS_VOXEL_ARRAY_SIZE && Index >= 0)
		{
			AtmosVoxel LocalVoxel = AtmosVoxels[Index];

			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (AtmosVoxels[i].AdjacentVoxels[j] == Index) AtmosVoxels[i].AdjacentVoxels[j] = -1; // Iterate through the adjacent voxels and remove the reference to this now deleted voxel
				}
			}

			if (LocalVoxel.UpdateVoxelIndex != -1) // If this voxel is referenced in the update voxel array
			{
				std::swap(UpdateAtmosVoxels[LocalVoxel.UpdateVoxelIndex], UpdateAtmosVoxels[UpdateAtmosVoxelsSize]);

				UpdateAtmosVoxels[UpdateAtmosVoxelsSize] = -1;
			}

			AtmosVoxels[Index] = AtmosVoxel::EmptyVoxel();
		}
	};
}