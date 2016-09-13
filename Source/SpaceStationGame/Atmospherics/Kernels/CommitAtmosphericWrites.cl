#include "AtmosVoxel.h"

__kernel void CommitAtmosphericWrites(__global AtmosVoxelWrite* AtmosVoxelWrites, unsigned long VoxelWriteCount)
{
	int i = get_global_id(0);

	// Make sure that it's a valid voxel
	if (i < VoxelWriteCount)
	{
		for (int j = 0; j < 5; j++)
		{
			AtmosVoxelWrite ThisWrite = AtmosVoxelWrites[(i * 5) + j];
			
			if (ThisWrite.bValidOperation)
			{
				switch (i)
				{
				case Add:

					break;
				case Subtract:

					break;
				case Set:

					break;
				case SetAdditive:

					break;
				case SetSubtractive:

					break;
				default:

					break;
				}
			}
		}
	}
}
