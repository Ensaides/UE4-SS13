#include "Atmos.h"

__kernel void ComputeAtmospherics(__global AtmosVoxel* InputAtmosVoxels, __global AtmosVoxel* OutputAtmosVoxels, signed long VoxelCount, float AvgMul)
{
	int i = get_global_id(0);

	if (i < 26)
	{
		// Load the voxels into private memory
		AtmosVoxel InputVoxel = InputAtmosVoxels[i];
		AtmosVoxel OutputVoxel = InputAtmosVoxels[i];

		// Load adjacent voxels into private memory
		AtmosVoxel AdjacentVoxels[26];
		for (int AdjVoxelIndex = 0; AdjVoxelIndex < 26; AdjVoxelIndex++)
		{
			if (InputVoxel.AdjacentVoxels[AdjVoxelIndex] != -1)
			{
				// Get the gas data
				AdjacentVoxels[AdjVoxelIndex] = InputAtmosVoxels[InputVoxel.AdjacentVoxels[AdjVoxelIndex]];

				if (AdjacentVoxels[AdjVoxelIndex].bValidVoxel)
				{
					// Find the difference and the divide by 26
					float16 Dif = ((InputVoxel.Gases - AdjacentVoxels[AdjVoxelIndex].Gases) / 2.f) / 26.f;

					// Subtract the result from the dif and scale it by the average multiplier
					OutputVoxel.Gases = OutputVoxel.Gases - (Dif * AvgMul);
				}
			}
		}

		// Push the output
		OutputAtmosVoxels[i] = OutputVoxel;
	}
}
