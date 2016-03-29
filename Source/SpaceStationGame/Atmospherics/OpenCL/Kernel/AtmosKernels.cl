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
	unsigned char bNeedsUpdate : 1;
	unsigned char bValueChanged : 1;
	unsigned char bBlocked : 1;
	unsigned char bValidVoxel : 1;
} AtmosVoxel;

__kernel void square( __global float *Input, __global float *Output, __global AtmosVoxel* AtmosVoxels, int count)
{
	int i = get_global_id(0);
	if(i < count)
	{
		Output[i] = Input[i] * Input[i];
	}
}

