typedef struct
{
	// Gasses
	float16 Gasses;
	float16 DeltaGasses;

	// Pos
	float XPos;
	float YPos;
	float ZPos;
	
	// ID
	int AdjacentVoxels[8];
	
	// State
	bool bNeedsUpdate;
	bool bValueChanged;
	bool bBlocked;
	bool bValidVoxel;
} AtmosVoxel;

__kernel void ComputeAtmospherics(__global AtmosVoxel* AtmosVoxels, unsigned int count, float DeltaSeconds)
{
	int i = get_global_id(0);

	if(i < count)
	{
		AtmosVoxel WorkerVoxel = AtmosVoxels[i];

		if (WorkerVoxel.bNeedsUpdate && WorkerVoxel.bValidVoxel) // Get rid of this fucking branching
		{
			AtmosVoxel AdjacentVoxels[8];

			int intValidVoxels;
			bool ValidVoxels[8];

			for (int i = 0; i < 8; i++)
			{
				AdjacentVoxels[i] = AtmosVoxels[WorkerVoxel.AdjacentVoxels[i]];

				if (AdjacentVoxel->bValidVoxel)
				{
					intValidVoxels++;

					ValidVoxels[i] = true;
				}
			}

			for (int i = 0; i < 8; i++)
			{
				if (ValidVoxels[i])
				{
					float16 DeltaGasses = WorkerVoxel.Gasses - AdjacentVoxels[i].Gasses;

					for (int i = 0; i < 16; i++)
					{

					}
				}
			}
		}
	}
}

