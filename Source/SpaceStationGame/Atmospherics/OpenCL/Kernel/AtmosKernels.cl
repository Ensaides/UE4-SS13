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

__kernel void ComputeAtmospherics(__global AtmosVoxel* AtmosVoxels, unsigned int count)
{
	int i = get_global_id(0);

	if(i < count)
	{
		AtmosVoxel WorkerVoxel = AtmosVoxels[i];
	}
}

